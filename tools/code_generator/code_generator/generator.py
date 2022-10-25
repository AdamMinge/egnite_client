"""Tools for generating cpp code files"""

from __future__ import annotations

import re
import abc
import dataclasses

from typing import Iterable

from .utils import convert_to_list


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
        self.begin = begin
        self.end = end

    def code(self,
             code_style: CppCodeStyle,
             indent_level: int) -> str:
        elements_code: list[str] = []

        if self.begin and self.end:
            elements_code = [generator.code(code_style=code_style, indent_level=indent_level + 1)
                             for generator in self.elements]
            elements_code.insert(0, CppLine(txt=self.begin).code(
                code_style=code_style, indent_level=indent_level))
            elements_code.insert(len(elements_code), CppLine(txt=self.end).code(
                code_style=code_style, indent_level=indent_level))
        else:
            elements_code = [generator.code(code_style=code_style, indent_level=indent_level)
                             for generator in self.elements]

        return code_style.lf.join(elements_code)
    
    def add_element(self, 
                    element: CppGenerator) -> None:
        self.elements.append(element)


class CppFile(CppScope):
    def __init__(self,
                 header_guard: str | None = None) -> None:
        super().__init__()
        self.header_guard = header_guard
        
    def code(self,
             code_style: CppCodeStyle,
             indent_level: int) -> str: 
        
        if self.header_guard:
            elements_code = [generator.code(code_style=code_style, indent_level=indent_level)
                             for generator in self.elements]
            elements_code.insert(0, CppLine(txt=f"#ifndef {self.header_guard}").code(
                code_style=code_style, indent_level=indent_level))
            elements_code.insert(1, CppLine(txt=f"#define {self.header_guard}").code(
                code_style=code_style, indent_level=indent_level))
            elements_code.insert(len(elements_code), CppLine(txt=f"#endif //{self.header_guard}").code(
                code_style=code_style, indent_level=indent_level))
        else:
            elements_code = [generator.code(code_style=code_style, indent_level=indent_level)
                             for generator in self.elements]

        return code_style.lf.join(elements_code)


class CppClass(CppScope):
    def __init__(self,
                 name: str,
                 parents: str | Iterable[str] | None = None) -> None:
        super().__init__(begin="{", end="};")
        self.name = name
        self.parents = parents

    def code(self,
             code_style: CppCodeStyle,
             indent_level: int) -> str: 
        return code_style.lf.join([
            self._class_header().code(code_style=code_style, indent_level=indent_level),
            super().code(code_style=code_style, indent_level=indent_level)])
        
    def _class_header(self) -> CppLine:
        parents = convert_to_list(self.parents)
        parents = f": {', '.join(parents)}" if parents else ""
        return CppLine(txt=re.sub(" +", " ", f"class {self.name} {parents}".strip()))
      
        
class CppStruct(CppClass):
    def _class_header(self) -> CppLine:
        parents = convert_to_list(self.parents)
        parents = f": {', '.join(parents)}" if parents else ""
        return CppLine(txt=re.sub(" +", " ", f"struct {self.name} {parents}").strip())


class CppFunction(CppScope):
    def __init__(self,
                 name: str,
                 return_type: str | None = None,
                 arguments: str | Iterable[str] | None = None,
                 qualifiers: str | Iterable[str] | None = None) -> None:
        super().__init__(begin="{", end="}")
        self.name = name
        self.return_type = return_type
        self.arguments = arguments
        self.qualifiers = qualifiers

    def code(self,
             code_style: CppCodeStyle,
             indent_level: int) -> str:  
        return code_style.lf.join([self._function_header().code(
            code_style=code_style, indent_level=indent_level),
            super().code(code_style=code_style, indent_level=indent_level)])
        
    def _function_header(self) -> CppLine:
        return_type = f"{self.return_type}" if self.return_type else ""
        pre_qualifiers, post_qualifiers = self._split_qualifiers(convert_to_list(self.qualifiers))
        arguments = convert_to_list(self.arguments)

        arguments = ', '.join(arguments)
        pre_qualifiers = ' '.join(pre_qualifiers)
        post_qualifiers = ' '.join(post_qualifiers)
        
        return CppLine(
            txt=re.sub(" +", " ", f"{pre_qualifiers} {return_type} {self.name}({arguments}) {post_qualifiers}".strip()))
        
    def _split_qualifiers(self, 
                          qualifiers: Iterable[str]) -> tuple[list[str], list[str]]:
        pre_qualifiers: list[str] = []
        post_qualifiers: list[str] = []
        
        for qualifier in qualifiers:
            if qualifier in ["[[nodiscard]]", "virtual", "explicit"]:
                pre_qualifiers.append(qualifier)
            else:
                post_qualifiers.append(qualifier)
                
        return pre_qualifiers, post_qualifiers
