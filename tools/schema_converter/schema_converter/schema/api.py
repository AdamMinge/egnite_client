"""Schema representing api"""

from __future__ import annotations

from dataclasses import dataclass, field

@dataclass
class Api:
    name: str = field(metadata=dict(type="Attribute", name="name"))
    path: str = field(metadata=dict(type="Element", name="Path"))
    methods: list[Method] = field(default_factory=list, metadata=dict(type="Element", name="Method"))
    includes: list[str] = field(default_factory=list, metadata=dict(type="Element", name="Include"))
    global_parameters: list[KeyValue] = field(default_factory=list, metadata=dict(type="Element", name="Parameter"))
    global_headers: list[KeyValue] = field(default_factory=list, metadata=dict(type="Element", name="Header"))
    
@dataclass
class Method:
    name: str = field(metadata=dict(type="Attribute", name="name"))
    verb: str = field(metadata=dict(type="Attribute", name="verb"))
    returns: str = field(metadata=dict(type="Attribute", name="returns"))
    excepts: str = field(default="void", metadata=dict(type="Attribute", name="excepts"))
    body: str = field(default="", metadata=dict(type="Attribute", name="body"))
    arguments: list[KeyType] = field(default_factory=list, metadata=dict(type="Element", name="Argument"))
    parameters: list[KeyValue] = field(default_factory=list, metadata=dict(type="Element", name="Parameter"))
    headers: list[KeyValue] = field(default_factory=list, metadata=dict(type="Element", name="Header"))
    
@dataclass
class KeyType:
    key: str = field(metadata=dict(type="Attribute", name="key"))
    type: str = field(metadata=dict(type="Attribute", name="type"))
    
@dataclass
class KeyValue:
    key: str = field(metadata=dict(type="Attribute", name="key"))
    value: str = field(metadata=dict(type="Attribute", name="value")) 
