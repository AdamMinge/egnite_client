"""QML Schemas"""

from __future__ import annotations

from dataclasses import dataclass, field


@dataclass
class NameTypeSchema:
    name: str = field(metadata=dict(type="Attribute", name="name"))
    type: str = field(metadata=dict(type="Attribute", name="type"))
        
@dataclass
class KeyValueSchema:
    key: str = field(metadata=dict(type="Attribute", name="key"))
    value: str = field(metadata=dict(type="Attribute", name="value"))
    
@dataclass
class KeyNameSchema:
    key: str = field(metadata=dict(type="Attribute", name="key"))
    name: str = field(metadata=dict(type="Attribute", name="name"))
    
@dataclass
class ValueSchema:
    value: str = field(metadata=dict(type="Attribute", name="value"))
    
@dataclass
class PathSchema:
    segments: list[object] = field(default_factory=list, metadata=dict(type="Elements", wrapper="Arguments", choices=(
        dict(
            name="FixedSegment",
            type=ValueSchema
        ),
        dict(
            name="ParamSegment",
            type=NameTypeSchema
        )
    )))
    
    @property
    def fixed_segments(self) -> list[ValueSchema]:
        return [segment for segment in self.segments if isinstance(segment, ValueSchema)]
    
    @property
    def param_segments(self) -> list[NameTypeSchema]:
        return [segment for segment in self.segments if isinstance(segment, NameTypeSchema)]

@dataclass
class ClientSchema:
    name: str = field(metadata=dict(type="Attribute", name="name"))
    base_url: str = field(metadata=dict(type="Element", name="BaseUrl"))
    version: str = field(metadata=dict(type="Element", name="Version"))
    includes: list[str] = field(default_factory=list, metadata=dict(type="Element", name="Include", wrapper="Includes"))
    parameters: list[KeyValueSchema] = field(default_factory=list, metadata=dict(type="Element", name="Parameter", wrapper="Parameters"))
    headers: list[KeyValueSchema] = field(default_factory=list, metadata=dict(type="Element", name="Header", wrapper="Headers"))

@dataclass
class MethodSchema:
    name: str = field(metadata=dict(type="Attribute", name="name"))
    verb: str = field(metadata=dict(type="Attribute", name="verb"))
    path: PathSchema = field(default_factory=PathSchema, metadata=dict(type="Element", name="Path"))
    arguments: list[KeyNameSchema] = field(default_factory=list, metadata=dict(type="Element", name="Argument", wrapper="Arguments"))
    parameters: list[KeyValueSchema] = field(default_factory=list, metadata=dict(type="Element", name="Parameter", wrapper="Parameters"))
    headers: list[KeyValueSchema] = field(default_factory=list, metadata=dict(type="Element", name="Header", wrapper="Headers"))

@dataclass
class ApiSchema:
    name: str = field(metadata=dict(type="Attribute", name="name"))
    path: PathSchema = field(default_factory=PathSchema, metadata=dict(type="Element", name="Path"))
    methods: list[MethodSchema] = field(default_factory=list, metadata=dict(type="Element", name="Method", wrapper="Methods"))
    includes: list[str] = field(default_factory=list, metadata=dict(type="Element", name="Include", wrapper="Includes"))
    parameters: list[KeyValueSchema] = field(default_factory=list, metadata=dict(type="Element", name="Parameter", wrapper="Parameters"))
    headers: list[KeyValueSchema] = field(default_factory=list, metadata=dict(type="Element", name="Header", wrapper="Headers"))
