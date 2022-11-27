from __future__ import annotations

from re import sub
from typing import Iterable, Any

from .writer import CodeWriter


class ConstructsHelper:
    @staticmethod
    def is_iterable(value: Any) -> bool:
        try:
            iter(value)
        except TypeError:
            return False
        else:
            return True

    @staticmethod
    def listify(value: Iterable[Any] | Any | None) -> list[Any]:
        if value is None:
            return []
        elif not isinstance(value, str) and ConstructsHelper.is_iterable(value):
            return list(value)
        return [value]
    

class CppScope:
    def __init__(self,
                 prev_lines: str | Iterable[str],
                 post_lines: str | Iterable[str],
                 indent_scope: bool = True) -> None:
        self.prev_lines = ConstructsHelper.listify(prev_lines)
        self.post_lines = ConstructsHelper.listify(post_lines)
        self.indent_scope = indent_scope
        self.code_writer = CodeWriter()
        
    @property
    def code(self) -> str:
        writer = CodeWriter()
        writer.add_lines(self.prev_lines)
        if self.indent_scope:
            writer.indent()
        writer.add_lines(iter(self.code_writer))
        if self.indent_scope:
            writer.dedent()
        writer.add_lines(self.post_lines)
        return writer.code
    
    def add_code(self,
                 lines: Iterable[str] | str) -> None:
        self.code_writer.add_lines(lines)
    

class CppBraces(CppScope):
    def __init__(self, 
                 end_semicolon: bool = False) -> None:
        super().__init__(prev_lines='{', 
                         post_lines='};' if end_semicolon else '}', 
                         indent_scope=True)
        
    
class CppIncludeGuard(CppScope):
    def __init__(self, 
                 guard_name: str) -> None:
        super().__init__(prev_lines=[f"#ifndef {guard_name}", 
                                     f"#define {guard_name}"],
                         post_lines=f"#endif //{guard_name}",
                         indent_scope=False)


class CppFunction:
    post_qualifiers = []
    pre_qualifiers = ["[[nodiscard]]"]
    only_declaration_qualifiers = ["[[nodiscard]]"]
    
    def __init__(self,
                 name: str,
                 return_type: str | None = None,
                 arguments: str | Iterable[str] | None = None,
                 qualifiers: str | Iterable[str] | None = None) -> None:
        self.name = name
        self.return_type = return_type
        self.arguments = ConstructsHelper.listify(arguments)
        self.qualifiers = ConstructsHelper.listify(qualifiers)
        self.code_braces = CppBraces()
        
    @property
    def declaration(self) -> str:
        writer = CodeWriter()
        writer.add_line(f"{self._header(declaration=True)};")
        return writer.code
    
    @property
    def definition(self) -> str:
        writer = CodeWriter()
        writer.add_line(self._header(declaration=False)) 
        writer.add_lines(self.code_braces.code)
        return writer.code
    
    def add_code(self,
                 lines: Iterable[str] | str) -> None:
        self.code_braces.add_code(lines)
        
    def _header(self,
                declaration: bool) -> str:
        pre_qualifiers = " ".join(self._pre_qualifiers(declaration))
        post_qualifiers = " ".join(self._post_qualifiers(declaration))
        return_type = self._return_type()
        name = self._name(declaration)
        arguments = ", ".join(self._arguments(declaration)) 
        return sub(" +", " ", f"{pre_qualifiers} {return_type} {name}({arguments}) {post_qualifiers}".strip())
    
    def _pre_qualifiers(self,
                        declaration: bool) -> Iterable[str]:
        qualifiers_filter = lambda x: x in self.__class__.pre_qualifiers
        if not declaration:
             qualifiers_filter = (lambda x: x in x not in self.__class__.only_declaration_qualifiers 
                                  and x in self.__class__.pre_qualifiers)
        return filter(qualifiers_filter, self.qualifiers)
    
    def _post_qualifiers(self,
                         declaration: bool) -> Iterable[str]:
        qualifiers_filter = lambda x: x in self.__class__.post_qualifiers
        if not declaration:
             qualifiers_filter = (lambda x: x in x not in self.__class__.only_declaration_qualifiers 
                                  and x in self.__class__.post_qualifiers)
        return filter(qualifiers_filter, self.qualifiers)
    
    def _arguments(self,
                   declaration: bool) -> Iterable[str]:
        return (self._process_argument(argument, declaration) for argument in self.arguments)
    
    def _process_argument(self, argument: str, declaration: bool) -> str:
        if declaration:
            return argument
        return argument.split('=')[0].strip()
        
    def _return_type(self) -> str:
        return f"{self.return_type}" if self.return_type else ""
        
    def _name(self, 
              declaration: bool) -> str:
        return self.name


class CppClass:
    def __init__(self,
                 name: str,
                 parents: str | Iterable[str] | None = None,
                 is_struct: bool = False) -> None:
        self.name = name
        self.parents = ConstructsHelper.listify(parents)
        self.is_struct = is_struct
        self.code_braces = CppBraces(end_semicolon=True)
        
    @property
    def declaration(self) -> str:
        writer = CodeWriter()
        writer.add_line(f"{self._header(declaration=True)};")
        return writer.code
    
    @property
    def definition(self) -> str:
        writer = CodeWriter()
        writer.add_line(self._header(declaration=False))
        writer.add_lines(self.code_braces.code)
        return writer.code
    
    def add_code(self,
                 lines: Iterable[str] | str) -> None:
        self.code_braces.add_code(lines)
        
    def _header(self,
                declaration: bool) -> str:
        header = f"struct {self.name}" if self.is_struct else f"class {self.name}"
        if not declaration:
            parents = f" : {', '.join(self.parents)}" if self.parents else ""
            header = f"{header}{parents}"
        return header
    

class CppClassMethod(CppFunction):
    post_qualifiers = CppFunction.post_qualifiers + ["const", "override"]
    pre_qualifiers = CppFunction.pre_qualifiers + ["virtual", "explicit"]
    only_declaration_qualifiers = CppFunction.pre_qualifiers + ["override", "virtual", "explicit"]
    
    def __init__(self, 
                 name: str, 
                 class_owner: str,
                 return_type: str | None = None,
                 arguments: str | Iterable[str] | None = None,
                 qualifiers: str | Iterable[str] | None = None) -> None:
        super().__init__(name, return_type, arguments, qualifiers)
        self.class_owner = class_owner
        
    @property
    def default(self) -> str:
        writer = CodeWriter()
        writer.add_line(f"{self._header(declaration=False)} = default")
        return writer.code
        
    def _name(self, 
              declaration: bool) -> str:
        return self.name if declaration else f"{self.class_owner}::{self.name}"


class CppClassConstructor(CppClassMethod):
    def __init__(self, 
                 class_owner: str, 
                 arguments: str | Iterable[str] | None = None,
                 qualifiers: str | Iterable[str] | None = None) -> None:
        super().__init__(class_owner, class_owner, None, arguments, qualifiers)
        self.initializations = CppScope(prev_lines=[], post_lines=[])
        
    @property
    def definition(self) -> str:
        header = self._header(declaration=False)
        if self.initializations:
            header = f"{header} :"     
        writer = CodeWriter()
        writer.add_line(header)
        writer.add_lines(self.initializations.code)
        writer.add_lines(self.code_braces.code)  
        return writer.code
    
    def add_initialization(self,
                           lines: Iterable[str] | str) -> None:
        self.initializations.add_code(lines)


class CppClassDestructor(CppClassMethod):
    def __init__(self, 
                 class_owner: str, 
                 arguments: str | Iterable[str] | None = None,
                 qualifiers: str | Iterable[str] | None = None) -> None:
        super().__init__(f"~{class_owner}", class_owner, None, arguments, qualifiers)

