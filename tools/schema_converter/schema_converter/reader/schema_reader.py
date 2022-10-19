"""Schema reader"""

import abc

from pathlib import Path

from ..schema import Schema


class SchemaReader(abc.ABC):
    @abc.abstractmethod
    def read(self,
             schema_file: Path) -> Schema:
        pass
