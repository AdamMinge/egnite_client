"""QML Generator"""

from io import TextIOWrapper
from typing import Iterable
from pathlib import Path
from code_generator import generator

from ..schema import ClientSchema, ApiSchema, ModelSchema
from .generator import Generator


class HeaderClient(generator.CppFile):
    pass

class SrcClient(generator.CppFile):
    pass

class HeaderApi(generator.CppFile):
    pass

class SrcApi(generator.CppFile):
    pass

class HeaderModel(generator.CppFile):
    pass

class SrcModel(generator.CppFile):
    pass

class QmlGenerator(Generator):
    code_style = generator.CppCodeStyle(
        indent=" "*2, 
        lf="\n"
    )
    
    def _generate_client(self,
                         client_schema: ClientSchema,
                         header_stream: TextIOWrapper,
                         src_stream: TextIOWrapper,
                         include_dependencies: Iterable[Path]) -> None:
        header_client = HeaderClient()
        src_client = SrcClient()
        
        header_stream.write(header_client.code(code_style=QmlGenerator.code_style, indent_level=0))
        src_stream.write(src_client.code(code_style=QmlGenerator.code_style, indent_level=0))
        

    def _generate_api(self, api_schema: ApiSchema,
                      header_stream: TextIOWrapper,
                      src_stream: TextIOWrapper,
                      include_dependencies: Iterable[Path]) -> None:
        header_api = HeaderApi()
        src_api = SrcApi()
        
        header_stream.write(header_api.code(code_style=QmlGenerator.code_style, indent_level=0))
        src_stream.write(src_api.code(code_style=QmlGenerator.code_style, indent_level=0))

    def _generate_model(self,
                        model_schema: ModelSchema,
                        header_stream: TextIOWrapper,
                        src_stream: TextIOWrapper) -> None:
        header_model = HeaderModel()
        src_model = SrcModel()
        
        header_stream.write(header_model.code(code_style=QmlGenerator.code_style, indent_level=0))
        src_stream.write(src_model.code(code_style=QmlGenerator.code_style, indent_level=0))
