"""
--------------------------------------------------------------
schema_converter: command line tool for converting egnite
schema files such as json,xml to Qt/QML interface files.
--------------------------------------------------------------
usage: schema_converter [-h] [--sources SOURCES [SOURCES ...]] [--destination DESTINATION]
Options:
  -h, --help            show this help message and exit
  --sources SOURCES [SOURCES ...]
                        schema files such as json,xml to convert
  --destination DESTINATION
                        directory in which the generated files will be to add
"""

from argparse import ArgumentParser, ArgumentTypeError
from ast import parse
from pathlib import Path


def valid_source_paths(arg: str) -> Path:
    """Custom argparse type for file path values given from the command line"""
    path = Path(arg)

    if not path.exists():
        raise ArgumentTypeError(
            f"given source path: {arg} doesn't exist")
    if not path.is_file():
        raise ArgumentTypeError(
            f"given source path: {arg} isn't correct file path")

    return path


def valid_destination_path(arg: str) -> Path:
    """Custom argparse type for dir path values given from the command line"""
    path = Path(arg)

    if not path.exists():
        raise ArgumentTypeError(
            f"given destination path: {arg} doesn't exist")
    if not path.is_dir():
        raise ArgumentTypeError(
            f"given destination path: {arg} isn't correct dir path")

    return path


class Parser():
    @staticmethod
    def get_args():
        parser = ArgumentParser(prog="schema_converter")
        parser.add_argument("--sources", type=valid_source_paths, nargs="+", required=True,
                            help="schema files such as json,xml to convert")
        parser.add_argument("--destination", type=valid_destination_path, nargs=1, required=True,
                            help="directory in which the generated files will be to add")
        return parser.parse_args()


def main():
    args = Parser.get_args()
    print(f"args = ${args}")


if __name__ == "__main__":
    main()
