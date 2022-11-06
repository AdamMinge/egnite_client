"""Schema representing api"""

from __future__ import annotations

from dataclasses import dataclass, field


@dataclass
class ApiSchema:
    name: str = field(metadata=dict(type="Attribute", name="name"))
    path: str = field(metadata=dict(type="Element", name="Path"))
    methods: list[Method] = field(default_factory=list, metadata=dict(type="Element", name="Method"))
    includes: list[str] = field(default_factory=list, metadata=dict(type="Element", name="Include"))
    
@dataclass
class Method:
    name: str = field(metadata=dict(type="Attribute", name="name"))
    verb: str = field(metadata=dict(type="Attribute", name="verb"))
    returns: str = field(metadata=dict(type="Attribute", name="returns"))
    excepts: str = field(default="void", metadata=dict(type="Attribute", name="excepts"))
    parameters: list[Parameter] = field(default_factory=list, metadata=dict(type="Element", name="Parameter"))

@dataclass
class Parameter:
    key: str = field(metadata=dict(type="Attribute", name="key"))
    type: str = field(metadata=dict(type="Attribute", name="type"))

    
