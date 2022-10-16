"""Setup script for schema_converter project"""

from importlib.metadata import entry_points
import os.path
from setuptools import setup

THIS_FOLDER = os.path.abspath(os.path.dirname(__file__))

with open(os.path.join(THIS_FOLDER, "README.md")) as handle:
    README_TEXT = handle.read()

setup(
    name="schema_converter",
    version="1.0.0",
    description="tool for converting egnite schema files to Qt/QML interface files",
    long_description=README_TEXT,
    long_description_content_type="text/markdown",
    author="Adam Minge",
    author_email="minge.adam@gmail.com",
    license="MIT",
    classifiers=[
        "License :: OSI Approved :: MIT License",
        "Programming Language :: Python",
        "Programming Language :: Python :: 3",
    ],
    packages=["schema_converter"],
    include_package_data=["importlib_resources"],
    entry_points={"console_scripts": [
        "schema_converter=schema_converter.__main__:main"]},
)
