"""Setup script for schema_converter project"""

import schema_converter
import os.path

from setuptools import setup, find_packages

THIS_DIR = os.path.abspath(os.path.dirname(__file__))
CODE_GENERATOR_DIR = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', 'code_generator'))

with open(os.path.join(THIS_DIR, "README.md")) as handle:
    README_TEXT = handle.read()

setup(
    name="schema_converter",
    version=schema_converter.__version__,
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
    packages=find_packages(include=["schema_converter", "schema_converter.*"]),
    include_package_data=True,
    install_requires=[
        "importlib_resources", 
        "wheel",
        f"code_generator @ file://localhost/{CODE_GENERATOR_DIR}"],
    entry_points={"console_scripts": [
        "schema_converter=schema_converter.__main__:main"]},
)
