"""Tools for generating Qt/QML interface files from schema"""

import abc
import enum

from pathlib import Path

from schema import Schema


class CodeGenerator(abc.ABC):
    def __init__(self, schema: Schema) -> None:
        super().__init__()
        self.schema = schema

    def generate(self, destination: Path) -> None:
        pass


class QtInterfaceGenerator(CodeGenerator):
    pass


class QmlInterfaceGenerator(CodeGenerator):
    pass


class QtQmlInterfaceGenerator(CodeGenerator):
    pass


class GenerationTarget(enum.Flag):
    GenerateQtInterface = enum.auto()
    GenerateQmlInterface = enum.auto()
    GenerateQtQmlInterface = GenerateQtInterface | GenerateQmlInterface


def generate_code(schema: Schema, destination: Path, *,
                  generation_type: GenerationTarget) -> None:
    generator: CodeGenerator

    match generation_type:
        case GenerationTarget.GenerateQtQmlInterface:
            generator = QtQmlInterfaceGenerator(schema)
        case GenerationTarget.GenerateQtInterface:
            generator = QtInterfaceGenerator(schema)
        case GenerationTarget.GenerateQmlInterface:
            generator = QmlInterfaceGenerator(schema)

    generator.generate(destination)
