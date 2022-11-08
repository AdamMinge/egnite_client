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
        for schema, path in schemas:
            header_path = destination / f"{path.stem}.{self._hpp_file_suffix}"
            src_path = destination / f"{path.stem}.{self._cpp_file_suffix}"
            
            with header_path.open("w") as header_stream, src_path.open("w") as src_stream:
                match schema:
                    case Client():
                        self._generate_client(
                            schema, header_stream, src_stream)
                    case Api():
                        self._generate_api(
                            schema, header_stream, src_stream)
                    case Model():
                        self._generate_model(
                            schema, header_stream, src_stream)

    @abc.abstractmethod
    def _generate_client(self,
                         client_schema: Client,
                         header_stream: TextIOWrapper,
                         src_stream: TextIOWrapper) -> None:
        pass

    @ abc.abstractmethod
    def _generate_api(self,
                      api_schema: Api,
                      header_stream: TextIOWrapper,
                      src_stream: TextIOWrapper) -> None:
        pass

    @ abc.abstractmethod
    def _generate_model(self,
                        model_schema: Model,
                        header_stream: TextIOWrapper,
                        src_stream: TextIOWrapper) -> None:
        pass
