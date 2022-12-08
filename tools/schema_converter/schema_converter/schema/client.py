"""Schema representing client"""

from __future__ import annotations

from dataclasses import dataclass, field


@dataclass
class NameType:
    name: str = field(metadata=dict(type="Attribute", name="name"))
    type: str = field(metadata=dict(type="Attribute", name="type"))
        
@dataclass
class KeyValue:
    key: str = field(metadata=dict(type="Attribute", name="key"))
    value: str = field(metadata=dict(type="Attribute", name="value"))
    
Api = NameType
Parameter = KeyValue
Header = KeyValue

@dataclass
class Client:
    name: str = field(metadata=dict(type="Attribute", name="name"))
    base_url: str = field(metadata=dict(type="Element", name="BaseUrl"))
    version: str = field(metadata=dict(type="Element", name="Version"))
    apis: list[Api] = field(default_factory=list, metadata=dict(type="Element", name="Api", wrapper="Apis"))
    includes: list[str] = field(default_factory=list, metadata=dict(type="Element", name="Include", wrapper="Includes"))
    parameters: list[Parameter] = field(default_factory=list, metadata=dict(type="Element", name="Parameter", wrapper="Parameters"))
    headers: list[Header] = field(default_factory=list, metadata=dict(type="Element", name="Header", wrapper="Headers"))
