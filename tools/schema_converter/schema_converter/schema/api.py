"""Schema representing api"""

from __future__ import annotations

from dataclasses import dataclass, field


@dataclass
class Value:
    value: str = field(metadata=dict(type="Attribute", name="value"))

@dataclass
class NameType:
    name: str = field(metadata=dict(type="Attribute", name="name"))
    type: str = field(metadata=dict(type="Attribute", name="type"))
    
@dataclass
class KeyTypeName:
    key: str = field(metadata=dict(type="Attribute", name="key"))
    type: str = field(metadata=dict(type="Attribute", name="type"))
    name: str = field(metadata=dict(type="Attribute", name="name"))
    
@dataclass
class KeyValue:
    key: str = field(metadata=dict(type="Attribute", name="key"))
    value: str = field(metadata=dict(type="Attribute", name="value")) 
    
FixedSegment = Value
ParamSegment = NameType
Argument = KeyTypeName
Parameter = KeyValue
Header = KeyValue
    
@dataclass
class Path:
    segments: list[object] = field(default_factory=list, metadata=dict(type="Elements", wrapper="Arguments", choices=(
        dict(
            name="FixedSegment",
            type=FixedSegment
        ),
        dict(
            name="ParamSegment",
            type=ParamSegment
        )
    )))
    
    @property
    def fixed_segments(self) -> list[FixedSegment]:
        return [segment for segment in self.segments if isinstance(segment, FixedSegment)]
    
    @property
    def param_segments(self) -> list[ParamSegment]:
        return [segment for segment in self.segments if isinstance(segment, ParamSegment)]
        

@dataclass
class Method:
    name: str = field(metadata=dict(type="Attribute", name="name"))
    verb: str = field(metadata=dict(type="Attribute", name="verb"))
    returns: str = field(metadata=dict(type="Attribute", name="returns"))
    excepts: str = field(default="void", metadata=dict(type="Attribute", name="excepts"))
    body: str = field(default="", metadata=dict(type="Attribute", name="body"))
    path: Path = field(default_factory=Path, metadata=dict(type="Element", name="Path"))
    arguments: list[Argument] = field(default_factory=list, metadata=dict(type="Element", name="Argument", wrapper="Arguments"))
    parameters: list[Parameter] = field(default_factory=list, metadata=dict(type="Element", name="Parameter", wrapper="Parameters"))
    headers: list[Header] = field(default_factory=list, metadata=dict(type="Element", name="Header", wrapper="Headers"))

@dataclass
class Api:
    name: str = field(metadata=dict(type="Attribute", name="name"))
    path: Path = field(default_factory=Path, metadata=dict(type="Element", name="Path"))
    methods: list[Method] = field(default_factory=list, metadata=dict(type="Element", name="Method", wrapper="Methods"))
    includes: list[str] = field(default_factory=list, metadata=dict(type="Element", name="Include", wrapper="Includes"))
    parameters: list[Parameter] = field(default_factory=list, metadata=dict(type="Element", name="Parameter", wrapper="Parameters"))
    headers: list[Header] = field(default_factory=list, metadata=dict(type="Element", name="Header", wrapper="Headers"))
