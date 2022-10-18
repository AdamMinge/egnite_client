"""Tools for generating interfaces from schema"""

import abc
import enum

from pathlib import Path
from typing import Type, Iterable

from schema import Schema, Client, Api, Model


class InterfaceGenerator(abc.ABC):
    def __init__(self) -> None:
        super().__init__()


class QtInterfaceGenerator(InterfaceGenerator):
    pass


class QmlInterfaceGenerator(InterfaceGenerator):
    pass


class InterfacesGenerator:
    def __init__(self,
                 schema: Schema,
                 interface_generators: Iterable[Type[InterfaceGenerator]]) -> None:
        super().__init__()
        self.schema = schema
        self.interface_generators = interface_generators

    def generate(self,
                 destination: Path) -> None:
        pass


class Interface(enum.Flag):
    QtInterface = enum.auto()
    QmlInterface = enum.auto()


def generate_interfaces(schema: Schema,
                        destination: Path,
                        interfaces: Interface) -> None:
    interface_to_generator = {
        Interface.QtInterface: QtInterfaceGenerator,
        Interface.QmlInterface: QmlInterfaceGenerator
    }

    interface_generators = [generator()
                            for target, generator in interface_to_generator.items()
                            if interfaces & target]

    code_generator = InterfacesGenerator(schema, interface_generators)
    code_generator.generate(destination)
