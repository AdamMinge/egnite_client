"""Generator"""

import abc
from io import TextIOWrapper

from dataclasses import dataclass
from pathlib import Path
from typing import Iterable

from ..schema import Schema, ClientSchema, ApiSchema, ModelSchema


@dataclass
class SchemaFiles:
    schema: Schema
    header_path: Path
    src_path: Path


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
        for schema, path in schemas:
            header_path = destination / f"{path.stem}.{self._hpp_file_suffix}"
            src_path = destination / f"{path.stem}.{self._cpp_file_suffix}"
            
            with header_path.open("w") as header_stream, src_path.open("w") as src_stream:
                schema.includes.insert(0, f"{path.stem}.{self._hpp_file_suffix}")
                
                match schema:
                    case ClientSchema():
                        self._generate_client(
                            schema, header_stream, src_stream)
                    case ApiSchema():
                        self._generate_api(
                            schema, header_stream, src_stream)
                    case ModelSchema():
                        self._generate_model(
                            schema, header_stream, src_stream)

    @abc.abstractmethod
    def _generate_client(self,
                         client_schema: ClientSchema,
                         header_stream: TextIOWrapper,
                         src_stream: TextIOWrapper) -> None:
        pass

    @ abc.abstractmethod
    def _generate_api(self,
                      api_schema: ApiSchema,
                      header_stream: TextIOWrapper,
                      src_stream: TextIOWrapper) -> None:
        pass

    @ abc.abstractmethod
    def _generate_model(self,
                        model_schema: ModelSchema,
                        header_stream: TextIOWrapper,
                        src_stream: TextIOWrapper) -> None:
        pass
