"""Tools for generating cpp code files"""

from __future__ import annotations

import re
import abc
import dataclasses

from typing import Iterable

from .utils import listify


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
        self.txt = txt
        self.ignore_indent = ignore_indent

    def code(self,
             code_style: CppCodeStyle,
             indent_level: int) -> str:
        if self.ignore_indent:
            indent_level = 0
        return f"{code_style.indent * indent_level}{self.txt}"
    

class CppMultiLine(CppGenerator):
    def __init__(self, 
                 txt: str = "", 
                 ignore_indent: bool = False):
        self.txt = txt
        self.ignore_indent = ignore_indent
        
    def code(self,
            code_style: CppCodeStyle,
            indent_level: int) -> str:   
        elements_code: list[str] = []
        for txt in self.txt.split("\n"):
            elements_code.append(CppLine(txt=txt, ignore_indent = self.ignore_indent).code(
                code_style=code_style, indent_level=indent_level))
        return code_style.lf.join(elements_code) 
        

class CppScope(CppGenerator):
    def __init__(self,
                 begin: CppGenerator | None = None,
                 end: CppGenerator | None = None,
                 indent_elements: bool = False) -> None:
        self.elements: list[CppGenerator] = []
        self.begin = begin
        self.end = end
        self.indent_elements = indent_elements

    def code(self,
             code_style: CppCodeStyle,
             indent_level: int) -> str:
        
        elements_indent_level = indent_level + 1 if self.indent_elements else indent_level
        elements_code = [generator.code(code_style=code_style, indent_level=elements_indent_level)
                            for generator in self.elements]
        if self.begin:
            elements_code.insert(0, self.begin.code(
                code_style=code_style, indent_level=indent_level))
        if self.end:
            elements_code.insert(len(elements_code), self.end.code(
                code_style=code_style, indent_level=indent_level))
        return code_style.lf.join(elements_code)
    
    def add_element(self, 
                    element: CppGenerator) -> None:
        self.elements.append(element)


class CppFile(CppScope):
    def __init__(self,
                 header_guard: str | None = None) -> None:
        begin = CppMultiLine(f"#ifndef {header_guard}\n"
                             f"#define {header_guard}") if header_guard else None
        end = CppLine(f"#endif //{header_guard}") if header_guard else None
        super().__init__(begin=begin, end=end)


class CppClass(CppScope):
    def __init__(self,
                 name: str,
                 parents: str | Iterable[str] | None = None,
                 is_struct: bool = False) -> None:
        super().__init__(begin=CppLine("{"), end=CppLine("};"), indent_elements=True)
        self.name = name
        self.parents = parents
        self.is_struct = is_struct

    def code(self,
             code_style: CppCodeStyle,
             indent_level: int) -> str: 
        return code_style.lf.join([
            CppLine(txt=self._class_header()).code(code_style=code_style, indent_level=indent_level),
            super().code(code_style=code_style, indent_level=indent_level)])
        
    def declaration(self) -> CppLine:
        return CppLine(txt=f"{self._class_header(ignore_parents=True)};")
        
    def _class_header(self, ignore_parents = False) -> str:
        structure_type = "struct" if self.is_struct else "class"
        parents = ""
        
        if not ignore_parents:
            parents = listify(self.parents)
            parents = f": {', '.join(parents)}" if parents else ""
        return re.sub(" +", " ", f"{structure_type} {self.name} {parents}".strip())
      
        
class CppStruct(CppClass):
    def __init__(self,
                 name: str,
                 parents: str | Iterable[str] | None = None) -> None:
        super().__init__(name, parents, is_struct = True) 


class CppFunction(CppScope):
    def __init__(self,
                 name: str,
                 namespace: str | None = None,
                 return_type: str | None = None,
                 arguments: str | Iterable[str] | None = None,
                 qualifiers: str | Iterable[str] | None = None) -> None:
        super().__init__(begin=CppLine("{"), end=CppLine("}"), indent_elements=True)
        self.name = name
        self.namespace = namespace
        self.return_type = return_type
        self.arguments = arguments
        self.qualifiers = qualifiers

    def code(self,
             code_style: CppCodeStyle,
             indent_level: int) -> str:  
        return code_style.lf.join([
            CppLine(txt=self._function_header()).code(
            code_style=code_style, indent_level=indent_level),
            super().code(code_style=code_style, indent_level=indent_level)])
        
    def declaration(self) -> CppLine:
        return CppLine(txt=f"{self._function_header()};")
        
    def _function_header(self) -> str:
        name = f"{self.namespace}::{self.name}" if self.namespace else self.name
        return_type = f"{self.return_type}" if self.return_type else ""
        pre_qualifiers, post_qualifiers = self._split_qualifiers(listify(self.qualifiers))
        arguments = listify(self.arguments)

        arguments = ', '.join(arguments)
        pre_qualifiers = ' '.join(pre_qualifiers)
        post_qualifiers = ' '.join(post_qualifiers)
        
        return re.sub(" +", " ", f"{pre_qualifiers} {return_type} {name}({arguments}) {post_qualifiers}".strip())
        
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
    

class CppConstructor(CppFunction):
    def __init__(self, 
                 name: str, 
                 namespace: str | None = None,
                 arguments: str | Iterable[str] | None = None, 
                 initializations: str | Iterable[str] | None = None,
                 qualifiers: str | Iterable[str] | None = None) -> None:
        super().__init__(name, namespace, None, arguments, qualifiers)
        self.initializations = initializations

    def code(self,
             code_style: CppCodeStyle,
             indent_level: int) -> str:  
        if self.initializations:
            return code_style.lf.join([
                CppLine(txt=f"{self._function_header()} :").code(code_style=code_style, indent_level=indent_level),
                self._constructor_initializations().code(code_style=code_style, indent_level=indent_level),                
                CppScope.code(self, code_style=code_style, indent_level=indent_level)])
        else:
            return super().code(code_style=code_style, indent_level=indent_level)
        
    def _constructor_initializations(self) -> CppScope:
        scope = CppScope(indent_elements=True)
        
        initializations = listify(self.initializations)
        for initialization in initializations[:-1]:
            scope.add_element(CppLine(txt=f"{initialization},"))
        scope.add_element(CppLine(txt=initializations[-1]))
        
        return scope
        

class CppDestructor(CppFunction):
    def __init__(self, 
                 name: str, 
                 namespace: str | None = None,
                 qualifiers: str | Iterable[str] | None = None) -> None:
        super().__init__(f"~{name}", namespace, None, None, qualifiers)
