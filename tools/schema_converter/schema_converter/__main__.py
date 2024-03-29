"""
--------------------------------------------------------------
schema_converter: command line tool for converting egnite
schema files such as xml to Qt/QML interface files.
--------------------------------------------------------------
usage: schema_converter [-h] [--interface {qt,qml}] --sources SOURCES [SOURCES ...] --destination DESTINATION
Options:
  -h, --help            show this help message and exit
  --interface INTERFACE {qt,qml}
  --sources SOURCES [SOURCES ...]
                        schema files such as xml to convert
  --destination DESTINATION
                        directory in which the generated files will be to add
"""

from argparse import ArgumentParser, ArgumentTypeError, Action, Namespace
from pathlib import Path
from typing import Sequence, Any

from .generator import Interface
from .qml_generator import QmlGenerator
from .qt_generator import QtGenerator


def valid_generation_interface(arg: str) -> Interface:
    """Custom argparse type for generation target values given from the command line"""
    match arg:
        case "qt":
            return Interface.QtInterface
        case "qml":
            return Interface.QmlInterface
        case _:
            raise ArgumentTypeError(
                f"given interface: ({arg}) isn't correct, choose one of [qt, qml]")


def valid_source_paths(arg: str) -> Path:
    """Custom argparse type for file path values given from the command line"""
    path = Path(arg)

    if not path.exists():
        raise ArgumentTypeError(
            f"given source path: ({arg}) doesn't exist")
    if not path.is_file():
        raise ArgumentTypeError(
            f"given source path: ({arg}) isn't correct file path")

    return path


def valid_destination_path(arg: str) -> Path:
    """Custom argparse type for dir path values given from the command line"""
    path = Path(arg)

    if not path.exists():
        raise ArgumentTypeError(
            f"given destination path: ({arg}) doesn't exist")
    if not path.is_dir():
        raise ArgumentTypeError(
            f"given destination path: ({arg}) isn't correct dir path")

    return path


class UniqueAppendAction(Action):
    def __call__(
        self,
        parser: ArgumentParser,
        namespace: Namespace,
        values: str | Sequence[Any] | None,
        option_string: str | None = None
    ) -> None:
        
        unique_values = values
        if not isinstance(values, str | None):
            unique_values = set(values)

            if len(unique_values) != len(values):
                raise ArgumentTypeError("given arguments have to be unique")

        setattr(namespace, self.dest, unique_values)
            


class Parser():
    @staticmethod
    def get_args() -> Namespace:
        parser = ArgumentParser(prog="schema_converter")
        parser.add_argument("--interface",
                            type=valid_generation_interface,
                            nargs='?',
                            default="qt")
        parser.add_argument("--sources",
                            type=valid_source_paths, nargs="+",
                            required=True,
                            action=UniqueAppendAction,
                            help="schema files such as xml to convert")
        parser.add_argument("--destination",
                            type=valid_destination_path,
                            nargs='?',
                            required=True,
                            help="directory in which the generated files will be to add")

        return parser.parse_args()


def main() -> None:
    args = Parser.get_args()
    generator = QmlGenerator() if args.interface == Interface.QmlInterface else QtGenerator()
    generator.generate(args.sources, args.destination)
