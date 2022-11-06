"""Schema representing client"""

from __future__ import annotations

from dataclasses import dataclass, field


@dataclass
class ClientSchema:
    name: str = field(metadata=dict(type="Attribute", name="name"))
    base_url: str = field(metadata=dict(type="Element", name="BaseUrl"))
    version: str = field(metadata=dict(type="Element", name="Version"))
    apis: list[Api] = field(default_factory=list, metadata=dict(type="Element", name="Api"))
    includes: list[str] = field(default_factory=list, metadata=dict(type="Element", name="Include"))

    @dataclass
    class Api:
        name: str = field(metadata=dict(type="Attribute", name="name"))
        type: str = field(metadata=dict(type="Attribute", name="type"))
