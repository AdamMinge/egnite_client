"""Module for schema generating to files"""

import enum

from pathlib import Path
from typing import Type, Iterable

from ..schema import Schema

from .generator import Generator
from .qt_generator import QtGenerator
from .qml_generator import QmlGenerator


class Interface(enum.Enum):
    QtInterface = enum.auto()
    QmlInterface = enum.auto()


def generate_interfaces(schema: Iterable[tuple[Schema, Path]],
                        destination: Path,
                        interfaces: Interface) -> None:
    interface_to_generator: dict[Interface, Type[Generator]] = {
        Interface.QtInterface: QtGenerator,
        Interface.QmlInterface: QmlGenerator
    }


__all__ = [Generator, QtGenerator, QmlGenerator,
           Interface, generate_interfaces]
