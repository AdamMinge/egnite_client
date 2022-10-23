"""Schema representing client"""

from __future__ import annotations

from dataclasses import dataclass, field


@dataclass
class ClientSchema:
    name: str
    base_url: str
    version: int
    apis: list[Api] = field(default_factory=list)

    @dataclass
    class Api:
        name: str
        type: str
