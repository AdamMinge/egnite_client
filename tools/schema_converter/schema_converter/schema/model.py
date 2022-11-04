"""Schema representing model"""

from __future__ import annotations

from dataclasses import dataclass, field


@dataclass
class ModelSchema:
    name: str
    properties: list[Property] = field(default_factory=list)
    includes: list[str] = field(default_factory=list)

    @dataclass
    class Property:
        name: str
        type: str
