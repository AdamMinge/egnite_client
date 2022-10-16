"""Tools for representing schema"""

import abc


class Schema(abc.ABC):
    def __init__(self) -> None:
        super().__init__()
