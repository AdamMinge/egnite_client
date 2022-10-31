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
        schemas_files = self._get_schemas_files(schemas, destination)
        for schema_files in schemas_files:
            schema = schema_files.schema
            with schema_files.header_path.open("w") as header_stream, \
                    schema_files.src_path.open("w") as src_stream:
                match schema:
                    case ClientSchema():
                        self._generate_client(
                            schema, header_stream, src_stream, 
                            self._get_schemas_dependent_files(schema_files, schemas_files))
                    case ApiSchema():
                        self._generate_api(
                            schema, header_stream, src_stream,
                            self._get_schemas_dependent_files(schema_files, schemas_files))
                    case ModelSchema():
                        self._generate_model(
                            schema, header_stream, src_stream,
                            self._get_schemas_dependent_files(schema_files, schemas_files))

    def _get_schemas_files(self,
                           schemas: Iterable[tuple[Schema, Path]],
                           destination: Path) -> Iterable[SchemaFiles]:
        schemas_files: Iterable[SchemaFiles] = []

        for schema, path in schemas:
            schemas_files.append(SchemaFiles(
                schema=schema,
                header_path=destination /
                f"{path.stem}.{self._hpp_file_suffix}",
                src_path=destination / f"{path.stem}.{self._cpp_file_suffix}"))

        return schemas_files
    
    def _get_schemas_dependent_files(self,
                                     schema_files: SchemaFiles,
                                     schemas_files: Iterable[SchemaFiles]) -> Iterable[SchemaFiles]:
        dependent_schema_files: Iterable[SchemaFiles] = [schema_files]
        
        match schema_files.schema:
            case ClientSchema():
                dependent_schema_files.extend(filter(lambda x: isinstance(x.schema, ApiSchema), schemas_files))
            case ApiSchema():
                dependent_schema_files.extend(filter(lambda x: isinstance(x.schema, ModelSchema), schemas_files))
        
        return dependent_schema_files

    @abc.abstractmethod
    def _generate_client(self,
                         client_schema: ClientSchema,
                         header_stream: TextIOWrapper,
                         src_stream: TextIOWrapper,
                         schemas_dependent_files: Iterable[SchemaFiles]) -> None:
        pass

    @ abc.abstractmethod
    def _generate_api(self,
                      api_schema: ApiSchema,
                      header_stream: TextIOWrapper,
                      src_stream: TextIOWrapper,
                      schemas_dependent_files: Iterable[SchemaFiles]) -> None:
        pass

    @ abc.abstractmethod
    def _generate_model(self,
                        model_schema: ModelSchema,
                        header_stream: TextIOWrapper,
                        src_stream: TextIOWrapper,
                        schemas_dependent_files: Iterable[SchemaFiles]) -> None:
        pass
