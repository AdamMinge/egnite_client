"""Tools for representing schema"""

from __future__ import annotations

from dataclasses import dataclass, field


@dataclass
class Client:
    name: str
    base_url: str
    version: int


@dataclass
class Api:
    name: str
    path: str
    methods: list[Method] = field(default_factory=list)


@dataclass
class Method:
    name: str
    verb: str
    returns: str
    parameters: list[Property] = field(
        default_factory=list)


@dataclass
class Model:
    name: str
    properties: list[Property] = field(default_factory=list)


@dataclass
class Property:
    key: str
    type: str


@dataclass
class Schema:
    clients: list[tuple[Client, str]] = field(default_factory=list)
    apis: list[tuple[Api, str]] = field(default_factory=list)
    models: list[tuple[Model, str]] = field(default_factory=list)

    @property
    def elements(self) -> list[tuple[Client, str] | tuple[Api, str] | tuple[Model, str]]:
        return self.clients + self.apis + self.models
