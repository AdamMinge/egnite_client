"""Module for schema generating to files"""

import enum

from pathlib import Path
from typing import Iterable

from ..schema import Schema

from .generator import Generator
from .qt_generator import QtGenerator
from .qml_generator import QmlGenerator


class Interface(enum.Enum):
    QtInterface = enum.auto()
    QmlInterface = enum.auto()


class UnsupportedInterface(Exception):
    pass


def generate_interfaces(schemas: Iterable[tuple[Schema, Path]],
                        destination: Path,
                        interfaces: Interface) -> None:

    generator: Generator
    match interfaces:
        case Interface.QtInterface:
            generator = QtGenerator()
        case Interface.QmlInterface:
            generator = QmlGenerator()
        case _:
            raise UnsupportedInterface(
                f"given interface ({interfaces}) is unsupported")

    generator.generate(schemas, destination)


__all__ = ["Generator", "QtGenerator", "QmlGenerator",
           "Interface", "generate_interfaces"]
