"""
--------------------------------------------------------------
schema_converter: command line tool for converting egnite
schema files such as JSON,XML to Qt/QML interface files.
--------------------------------------------------------------
Usage: schema_converter <options> --sources <paths> --destination <directory>
Options:
  -h, --help             Show help screen only (this message).
"""

from argparse import ArgumentParser


def main():
    parser = ArgumentParser(prog="RestGenerator")
    parser.add_argument("--sources", type=str, nargs="+",
                        help="")
    parser.add_argument("--destination", nargs=1,
                        help="")

    args = parser.parse_args()


if __name__ == "__main__":
    main()
