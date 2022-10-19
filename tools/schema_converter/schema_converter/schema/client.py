"""Schema representing client"""

from __future__ import annotations

from typing import Iterable
from dataclasses import dataclass, field


@dataclass
class ClientSchema:
    name: str
    base_url: str
    version: int
    apis: Iterable[Api] = field(default_factory=list)

    @dataclass
    class Api:
        name: str
        type: str
