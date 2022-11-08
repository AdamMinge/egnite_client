"""Schema representing model"""

from __future__ import annotations

from dataclasses import dataclass, field


@dataclass
class Model:
    name: str = field(metadata=dict(type="Attribute", name="name"))
    properties: list[Property] = field(default_factory=list, metadata=dict(type="Element", name="Property"))
    includes: list[str] = field(default_factory=list, metadata=dict(type="Element", name="Include"))

@dataclass
class Property:
    name: str = field(metadata=dict(type="Attribute", name="name"))
    type: str = field(metadata=dict(type="Attribute", name="type"))
