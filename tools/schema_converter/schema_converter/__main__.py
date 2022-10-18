"""
--------------------------------------------------------------
schema_converter: command line tool for converting egnite
schema files such as xml to Qt/QML interface files.
--------------------------------------------------------------
usage: schema_converter [-h] [--sources SOURCES [SOURCES ...]] [--destination DESTINATION]
Options:
  -h, --help            show this help message and exit
  --sources SOURCES [SOURCES ...]
                        schema files such as xml to convert
  --destination DESTINATION
                        directory in which the generated files will be to add
"""

from argparse import ArgumentParser, ArgumentTypeError
from pathlib import Path

from reader import read_schema
from interface_generator import generate_interfaces, Interface


def valid_generation_interface(arg: str) -> Interface:
    """Custom argparse type for generation target values given from the command line"""
    match arg:
        case "qt":
            return Interface.QtInterface
        case "qml":
            return Interface.QmlInterface
        case "all":
            return Interface.QtInterface | Interface.QmlInterface
        case _:
            raise ArgumentTypeError(
                f"given interface: ({arg}) isn't correct, choose one of [qt, qml, all]")


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


class Parser():
    @staticmethod
    def get_args():
        parser = ArgumentParser(prog="schema_converter")
        parser.add_argument("--interface",
                            type=valid_generation_interface, nargs=1, default="all")
        parser.add_argument("--sources", type=valid_source_paths, nargs="+", required=True,
                            help="schema files such as xml to convert")
        parser.add_argument("--destination", type=valid_destination_path, nargs=1, required=True,
                            help="directory in which the generated files will be to add")
        return parser.parse_args()


def main():
    args = Parser.get_args()
    schema = read_schema(args.sources)
    generate_interfaces(schema, args.destination, args.interface)


if __name__ == "__main__":
    main()
