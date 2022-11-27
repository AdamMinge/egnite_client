from __future__ import annotations


from itertools import chain
from typing import Iterable


class CodeWriter:
    def __init__(self, 
                 lf: str = "\n", 
                 indent: int | str = 2) -> None:
        if isinstance(indent, int):
            indent = " " * indent 
        self._indent = indent
        self._lf = lf
        self._lines: list[str] = []
        self._indent_level = 0
        
    @property
    def code(self) -> str:
        return self._lf.join(self._lines)
    
    def add_line(self, 
                 text: str = "", 
                 ignore_indent: bool = False) -> None:
        indent_level = 0 if ignore_indent else self._indent_level
        current_line = f"{self._indent * indent_level}{text}"
        self._lines.append(current_line)
        
    def add_lines(self,
                  lines: Iterable[str] | str,
                  ignore_indent: bool = False) -> None:
        if isinstance(lines, str):
            lines = lines.split(self._lf)
        else:
            lines = chain.from_iterable(
                supposed_line.split(self._lf) for supposed_line in lines)
        for line in lines:
            self.add_line(line, ignore_indent)

    def indent(self):
        self._indent_level += 1
        
    def dedent(self):
        if self._indent_level > 0:
            self._indent_level -= 1
          
    def __str__(self) -> str:
        return self.code
    
    def __iter__(self) -> Iterable[str]:
        return iter(self._lines)
    
    def __getitem__(self, 
                    key) -> str:
        return self._lines.__getitem__(key)
    
    def __contains__(self,
                     key) -> bool:
        return self._lines.__contains__(key)
    
    def __len__(self) -> int:
        return self._lines.__len__()
