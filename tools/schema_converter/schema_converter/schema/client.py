"""Schema representing client"""

from __future__ import annotations

from dataclasses import dataclass, field


@dataclass
class Client:
    name: str = field(metadata=dict(type="Attribute", name="name"))
    base_url: str = field(metadata=dict(type="Element", name="BaseUrl"))
    version: str = field(metadata=dict(type="Element", name="Version"))
    apis: list[NameType] = field(default_factory=list, metadata=dict(type="Element", name="Api"))
    includes: list[str] = field(default_factory=list, metadata=dict(type="Element", name="Include"))
    global_parameters: list[KeyValue] = field(default_factory=list, metadata=dict(type="Element", name="Parameter"))
    global_headers: list[KeyValue] = field(default_factory=list, metadata=dict(type="Element", name="Header"))

@dataclass
class NameType:
    name: str = field(metadata=dict(type="Attribute", name="name"))
    type: str = field(metadata=dict(type="Attribute", name="type"))
        
@dataclass
class KeyValue:
    key: str = field(metadata=dict(type="Attribute", name="key"))
    value: str = field(metadata=dict(type="Attribute", name="value"))
