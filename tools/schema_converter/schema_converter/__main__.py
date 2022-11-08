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
from xsdata.exceptions import ParserError
from xsdata.formats.dataclass.parsers import XmlParser
from xsdata.formats.dataclass.parsers.config import ParserConfig

from .schema import Schema, Client, Api, Model
from .generator import generate_interfaces, Interface


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
                            default="qt",
                            choices=["qt", "qml"])
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


def read_schema(path: Path) -> Schema:
    config = ParserConfig(fail_on_unknown_properties=True, 
                          fail_on_unknown_attributes=True, 
                          fail_on_converter_warnings=True)
    supported_schemas = [Client, Api, Model]
    for supported_schema in supported_schemas:
        parser = XmlParser(config=config)
        try:
            schema = parser.from_path(path, supported_schema)
            return schema
        except (ParserError, TypeError):
            continue
    raise ArgumentTypeError(
        f"given source path: ({path}) isn't correct schema format")


def main() -> None:
    args = Parser.get_args()

    schemas: list[tuple[Schema, Path]] = []
    for source in args.sources:
        print(f"schema = {read_schema(source)}")
        schemas.append((read_schema(source), source))

    generate_interfaces(schemas, args.destination, args.interface)
