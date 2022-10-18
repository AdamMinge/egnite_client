"""Tools for reading data from xml,json schema file"""

import abc

from pathlib import Path
from typing import Type

from schema import Schema


class SchemaReader(abc.ABC):
    def __init__(self) -> None:
        super().__init__()

    def read(self, schema_file: Path) -> Schema:
        pass


class XmlSchemaReader(SchemaReader):
    pass


class JsonSchemaReader(SchemaReader):
    pass


class UnsupportedSchemaFile(Exception):
    pass


def read_schema(schema_file: Path) -> Schema:
    reader: Type[SchemaReader]

    match schema_file.suffix:
        case ".xml":
            reader = XmlSchemaReader()
        case ".json":
            reader = JsonSchemaReader()
        case _:
            raise UnsupportedSchemaFile(
                f"given schema file: ({schema_file}) has unsupported extension")

    return reader.read(schema_file)
