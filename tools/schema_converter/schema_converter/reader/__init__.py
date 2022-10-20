"""Module for schema reading from file"""

from pathlib import Path
from typing import Type

from ..schema import Schema
from .schema_reader import SchemaReader
from .xml_schema_reader import XmlSchemaReader, IncorrectXmlSchema


class UnsupportedSchemaFileExtension(Exception):
    pass


def read_schema(schema_file: Path) -> Schema:
    generator: Type(SchemaReader) = None
    match schema_file.suffix:
        case ".xml":
            reader = XmlSchemaReader()
        case _:
            raise UnsupportedSchemaFileExtension(
                f"given schema file: ({schema_file}) has unsupported extension")

    return reader.read(schema_file)


__all__ = [SchemaReader, XmlSchemaReader,
           IncorrectXmlSchema, UnsupportedSchemaFileExtension, read_schema]
