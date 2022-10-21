"""Setup script for code_generator project"""

import code_generator
import os.path

from setuptools import setup, find_packages

THIS_FOLDER = os.path.abspath(os.path.dirname(__file__))

with open(os.path.join(THIS_FOLDER, "README.md")) as handle:
    README_TEXT = handle.read()

setup(
    name="code_generator",
    version=code_generator.__version__,
    description="tool for cpp code generation",
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
    packages=find_packages(include=["code_generator", "code_generator.*"]),
    include_package_data=True,
    install_requires=["importlib_resources", "wheel"],
)
