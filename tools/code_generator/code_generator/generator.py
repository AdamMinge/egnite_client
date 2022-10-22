"""Tools for generating cpp code files"""

from __future__ import annotations

import abc
import dataclasses

from typing import Iterable


@dataclasses.dataclass
class CppCodeStyle:
    indent: str
    lf: str


class CppGenerator(abc.ABC):
    @abc.abstractmethod
    def code(self,
             code_style: CppCodeStyle,
             indent_level: int) -> str:
        pass


class CppLine(CppGenerator):
    def __init__(self,
                 txt: str = "",
                 ignore_indent: bool = False) -> None:
        self._txt = txt
        self._ignore_indent = ignore_indent

    def code(self,
             code_style: CppCodeStyle,
             indent_level: int) -> str:
        if self._ignore_indent:
            indent_level = 0
        return f"{code_style.indent * indent_level}{self._txt}"


class CppScope(CppGenerator):
    def __init__(self,
                 begin: str | None = None,
                 end: str | None = None) -> None:
        self._elements: list[CppGenerator] = []
        if begin and end:
            self._scope = (CppLine(txt=begin), CppLine(txt=end))

    def code(self,
             code_style: CppCodeStyle,
             indent_level: int) -> str:
        elements_code: list[str] = []

        if self._scope:
            elements_code = [generator.code(code_style=code_style, indent_level=indent_level + 1)
                             for generator in self._elements]
            elements_code.insert(0, self._scope[0].code(
                code_style=code_style, indent_level=indent_level))
            elements_code.insert(len(elements_code), self._scope[1].code(
                code_style=code_style, indent_level=indent_level))
        else:
            elements_code = [generator.code(code_style=code_style, indent_level=indent_level)
                             for generator in self._elements]

        return code_style.lf.join(elements_code)


class CppFile(CppScope):
    def __init__(self) -> None:
        super().__init__()

    def add_line(self,
                 cpp_line: CppLine) -> None:
        self._elements.append(cpp_line)

    def add_class(self,
                  cpp_class: CppClass) -> None:
        self._elements.append(cpp_class)

    def add_function(self,
                     cpp_function: CppFunction) -> None:
        self._elements.append(cpp_function)


class CppClass(CppScope):
    def __init__(self,
                 name: str,
                 parents: str | Iterable[str] | None = None) -> None:
        super().__init__(begin="{", end="}")

        if not isinstance(parents, str):
            parents = f"{', '.join(parents)}" if parents else ""
        parents = f" : {parents}" if parents else ""

        self._class_header = CppLine(
            txt=f"class {name}{parents}")

    def add_function(self,
                     cpp_function: CppFunction) -> None:
        self._elements.append(cpp_function)

    def add_line(self,
                 cpp_line: CppLine) -> None:
        self._elements.append(cpp_line)

    def code(self,
             code_style: CppCodeStyle,
             indent_level: int) -> str:
        return code_style.lf.join([self._class_header.code(
            code_style=code_style, indent_level=indent_level),
            super().code(code_style=code_style, indent_level=indent_level)])


class CppFunction(CppScope):
    def __init__(self,
                 name: str,
                 return_type: str = "void",
                 arguments: str | Iterable[str] | None = None,
                 qualifiers: str | Iterable[str] | None = None) -> None:
        super().__init__(begin="{", end="}")

        if not isinstance(arguments, str):
            arguments = f"{', '.join(arguments)}" if arguments else ""
        if not isinstance(qualifiers, str):
            qualifiers = f"{' '.join(qualifiers)}" if qualifiers else ""

        self._function_header = CppLine(
            txt=f"{return_type} {name}({arguments}) {qualifiers}")

    def code(self,
             code_style: CppCodeStyle,
             indent_level: int) -> str:
        return code_style.lf.join([self._function_header.code(
            code_style=code_style, indent_level=indent_level),
            super().code(code_style=code_style, indent_level=indent_level)])
