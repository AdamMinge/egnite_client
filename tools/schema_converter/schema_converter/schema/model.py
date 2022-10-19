"""Schema representing model"""

from __future__ import annotations

from typing import Iterable
from dataclasses import dataclass, field


@dataclass
class ModelSchema:
    name: str
    properties: Iterable[Property] = field(default_factory=list)

    @dataclass
    class Property:
        key: str
        type: str
