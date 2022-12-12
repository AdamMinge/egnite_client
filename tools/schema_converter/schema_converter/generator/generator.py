"""Generator"""

import abc
from io import TextIOWrapper

from pathlib import Path
from typing import Iterable

from ..schema import Schema, Client, Api, Model


class Generator(abc.ABC):
    def __init__(self, *,
                 cpp_file_suffix="cpp",
                 hpp_file_suffix="h") -> None:
        super().__init__()
        self._cpp_file_suffix = cpp_file_suffix
        self._hpp_file_suffix = hpp_file_suffix

    def generate(self,
                 schemas: Iterable[tuple[Schema, Path]],
                 destination: Path) -> None:
        supported_schemas = self._supported_schemas()
        for schema, path in schemas:
            if not isinstance(schema, supported_schemas):
                continue
            
            header_path = destination / f"{path.stem}.{self._hpp_file_suffix}"
            src_path = destination / f"{path.stem}.{self._cpp_file_suffix}"
            
            with header_path.open("w") as header_stream, src_path.open("w") as src_stream:
                self._generate(schema, header_stream, src_stream)
                        
    @abc.abstractmethod
    def _supported_schemas(self) -> tuple[type, ...]:
        pass

    @abc.abstractmethod
    def _generate(self,
                  schema: Schema,
                  header_stream: TextIOWrapper,
                  src_stream: TextIOWrapper) -> None:
        pass
