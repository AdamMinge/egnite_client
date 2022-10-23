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
                 ignore_indent: bool = False,
                 offset_indent: int = 0) -> None:
        self.txt = txt
        self.ignore_indent = ignore_indent
        self.offset_indent = offset_indent

    def code(self,
             code_style: CppCodeStyle,
             indent_level: int) -> str:
        if self.ignore_indent:
            indent_level = 0
        indent_level = max(0, indent_level + self.offset_indent)
            
        return f"{code_style.indent * indent_level}{self.txt}"


class CppScope(CppGenerator):
    def __init__(self,
                 begin: str | None = None,
                 end: str | None = None) -> None:
        self.elements: list[CppGenerator] = []
        if begin and end:
            self.scope = (CppLine(txt=begin), CppLine(txt=end))

    def code(self,
             code_style: CppCodeStyle,
             indent_level: int) -> str:
        elements_code: list[str] = []

        if hasattr(self, 'scope'):
            elements_code = [generator.code(code_style=code_style, indent_level=indent_level + 1)
                             for generator in self.elements]
            elements_code.insert(0, self.scope[0].code(
                code_style=code_style, indent_level=indent_level))
            elements_code.insert(len(elements_code), self.scope[1].code(
                code_style=code_style, indent_level=indent_level))
        else:
            elements_code = [generator.code(code_style=code_style, indent_level=indent_level)
                             for generator in self.elements]

        return code_style.lf.join(elements_code)
    
    def add_element(self, 
                    element: CppGenerator) -> None:
        self.elements.append(element)


class CppFile(CppScope):
    def __init__(self) -> None:
        super().__init__()

    def create_line(self, *args, **kwargs) -> CppLine:
        cpp_line = CppLine(*args, **kwargs)
        self.add_element(cpp_line)
        return cpp_line

    def create_class(self, *args, **kwargs) -> CppClass:
        cpp_class = CppClass(*args, **kwargs)
        self.add_element(cpp_class)
        return cpp_class

    def create_struct(self, *args, **kwargs) -> CppStruct:
        cpp_struct = CppStruct(*args, **kwargs)
        self.add_element(cpp_struct)
        return cpp_struct

    def create_function(self, *args, **kwargs) -> CppFunction:
        cpp_function = CppFunction(*args, **kwargs)
        self.add_element(cpp_function)
        return cpp_function


class CppClass(CppScope):
    def __init__(self,
                 name: str,
                 parents: str | Iterable[str] | None = None) -> None:
        super().__init__(begin="{", end="};")

        if not isinstance(parents, str):
            parents = f"{', '.join(parents)}" if parents else ""
        parents = f" : {parents}" if parents else ""

        self.class_header = CppLine(txt=f"class {name}{parents}")

    def create_line(self, *args, **kwargs) -> CppLine:
        cpp_line = CppLine(*args, **kwargs)
        self.add_element(cpp_line)
        return cpp_line

    def create_function(self, *args, **kwargs) -> CppFunction:
        cpp_function = CppFunction(*args, **kwargs)
        self.add_element(cpp_function)
        return cpp_function

    def code(self,
             code_style: CppCodeStyle,
             indent_level: int) -> str:
        return code_style.lf.join([self.class_header.code(
            code_style=code_style, indent_level=indent_level),
            super().code(code_style=code_style, indent_level=indent_level)])
      
        
class CppStruct(CppClass):
    def __init__(self, name: str, parents: str | Iterable[str] | None = None) -> None:
        super().__init__(name, parents)
        self.class_header.txt = self.class_header.txt.replace("class", "struct")


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

        self.function_header = CppLine(
            txt=f"{return_type} {name}({arguments}) {qualifiers}")

    def code(self,
             code_style: CppCodeStyle,
             indent_level: int) -> str:
        return code_style.lf.join([self.function_header.code(
            code_style=code_style, indent_level=indent_level),
            super().code(code_style=code_style, indent_level=indent_level)])
