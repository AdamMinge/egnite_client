"""QT Generator"""

from io import TextIOWrapper
from typing import Iterable
from pathlib import Path

from ..schema import ClientSchema, ApiSchema, ModelSchema
from .generator import Generator


class QtGenerator(Generator):
    def _generate_client(self,
                         client_schema: ClientSchema,
                         header_stream: TextIOWrapper,
                         src_stream: TextIOWrapper,
                         include_dependencies: Iterable[Path]) -> None:
        pass

    def _generate_api(self, api_schema: ApiSchema,
                      header_stream: TextIOWrapper,
                      src_stream: TextIOWrapper,
                      include_dependencies: Iterable[Path]) -> None:
        pass

    def _generate_model(self,
                        model_schema: ModelSchema,
                        header_stream: TextIOWrapper,
                        src_stream: TextIOWrapper) -> None:
        pass
