"""Generator"""

import abc
import enum

from pathlib import Path
from io import TextIOWrapper
from typing import Iterable
from xsdata.exceptions import ParserError
from xsdata.formats.dataclass.parsers import XmlParser
from xsdata.formats.dataclass.parsers.config import ParserConfig


class Interface(enum.Enum):
    QtInterface = enum.auto()
    QmlInterface = enum.auto()


class UnsupportedSchema(Exception):
    pass


class Generator(abc.ABC):
    def __init__(self, *,
                 cpp_file_suffix="cpp",
                 hpp_file_suffix="h") -> None:
        super().__init__()
        self._cpp_file_suffix = cpp_file_suffix
        self._hpp_file_suffix = hpp_file_suffix

    def generate(self,
                 paths: Iterable[Path],
                 destination: Path) -> None:
        for path in paths:
            schema = self._read_schema(path)
            header_path = destination / f"{path.stem}.{self._hpp_file_suffix}"
            src_path = destination / f"{path.stem}.{self._cpp_file_suffix}"
            
            with header_path.open("w") as header_stream, src_path.open("w") as src_stream:
                self._generate(schema, header_stream, src_stream)
                        
    def _read_schema(self, path: Path) -> object:
        config = ParserConfig(fail_on_unknown_properties=True, 
                              fail_on_unknown_attributes=True, 
                              fail_on_converter_warnings=True)
        
        for supported_schema in self._supported_schemas():
            parser = XmlParser(config=config)
            try:
                schema = parser.from_path(path, supported_schema)
                return schema
            except (ParserError, TypeError) as e:
                continue
        raise UnsupportedSchema(
            f"given schema: ({path}) isn't correct schema format")                 
                                        
    @abc.abstractmethod
    def _supported_schemas(self) -> tuple[type, ...]:
        pass

    @abc.abstractmethod
    def _generate(self,
                  schema: object,
                  header_stream: TextIOWrapper,
                  src_stream: TextIOWrapper) -> None:
        pass
