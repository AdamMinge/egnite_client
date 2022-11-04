"""Schema representing api"""

from __future__ import annotations

from dataclasses import dataclass, field


@dataclass
class ApiSchema:
    name: str
    path: str
    methods: list[Method] = field(default_factory=list)
    includes: list[str] = field(default_factory=list)

    @dataclass
    class Method:
        name: str
        verb: str
        returns: tuple[str, str]
        parameters: list[Parameter] = field(
            default_factory=list)

        @dataclass
        class Parameter:
            key: str
            type: str
