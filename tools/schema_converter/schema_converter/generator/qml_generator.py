"""QML Generator"""

from io import TextIOWrapper
from code_generator import code_generator

from ..schema import ClientSchema, ApiSchema, ModelSchema
from .generator import Generator


class HeaderClient(code_generator.CppFile):
    pass

class SrcClient(code_generator.CppFile):
    pass

class HeaderApi(code_generator.CppFile):
    pass

class SrcApi(code_generator.CppFile):
    pass

class HeaderModel(code_generator.CppFile):
    pass

class SrcModel(code_generator.CppFile):
    pass

class QmlGenerator(Generator):
    code_style = code_generator.CppCodeStyle(
        indent=" "*2, 
        lf="\n"
    )
    
    def _generate_client(self,
                         client_schema: ClientSchema,
                         header_stream: TextIOWrapper,
                         src_stream: TextIOWrapper) -> None:
        header_client = HeaderClient()
        src_client = SrcClient()
        
        header_stream.write(header_client.code(code_style=QmlGenerator.code_style, indent_level=0))
        src_stream.write(src_client.code(code_style=QmlGenerator.code_style, indent_level=0))
        

    def _generate_api(self, api_schema: ApiSchema,
                      header_stream: TextIOWrapper,
                      src_stream: TextIOWrapper) -> None:
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
