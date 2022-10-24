"""QT Generator"""

from io import TextIOWrapper
from typing import Iterable
from pathlib import Path
from code_generator import generator

from ..schema import ClientSchema, ApiSchema, ModelSchema
from .generator import Generator


class HeaderClient(generator.CppFile):
    def __init__(self, client_schema: ClientSchema) -> None:
        super().__init__()
        
class SrcClient(generator.CppFile):
    def __init__(self, client_schema: ClientSchema) -> None:
        super().__init__()

class HeaderApi(generator.CppFile):
    def __init__(self, api_schema: ApiSchema) -> None:
        super().__init__()

class SrcApi(generator.CppFile):
    def __init__(self, api_schema: ApiSchema) -> None:
        super().__init__()

class HeaderModel(generator.CppFile):
    def __init__(self, model_schema: ModelSchema) -> None:
        super().__init__()
        
        self.create_line(f"#ifndef SCHEMA_GENERATED_MODEL_{model_schema.name.upper()}_H")
        self.create_line(f"#define SCHEMA_GENERATED_MODEL_{model_schema.name.upper()}_H")
        self.create_line()
        self.create_line("#include <QObject>")
        self.create_line()
        
        model_struct = self.create_struct(model_schema.name)
        model_struct.create_line("Q_GADGET")
        model_struct.add_element(self._create_property_scope(model_schema))
        model_struct.create_line()
        model_struct.create_line(" public:", offset_indent = -1)
        model_struct.add_element(self._create_member_scope_scope(model_schema))
        
        self.create_line()
        self.create_line(f"#endif //SCHEMA_GENERATED_MODEL_{model_schema.name.upper()}_H")
        
    def _create_property_scope(self, model_schema: ModelSchema) -> generator.CppScope:
        scope = generator.CppScope()
        for property in model_schema.properties:
            scope.add_element(generator.CppLine(f"Q_PROPERTY({property.type} {property.name} MEMBER {property.name})"))
            
        return scope
    
    def _create_member_scope_scope(self, model_schema: ModelSchema) -> generator.CppScope:
        scope = generator.CppScope()
        for property in model_schema.properties:
            scope.add_element(generator.CppLine(f"{property.type} {property.name};"))
        
        return scope

class SrcModel(generator.CppFile):
    def __init__(self, model_schema: ModelSchema) -> None:
        super().__init__()
        self.create_line("#include \"model.h\"")

class QtGenerator(Generator):
    code_style = generator.CppCodeStyle(
        indent=" "*2, 
        lf="\n"
    )
    
    def _generate_client(self,
                         client_schema: ClientSchema,
                         header_stream: TextIOWrapper,
                         src_stream: TextIOWrapper,
                         include_dependencies: Iterable[Path]) -> None:
        header_client = HeaderClient(client_schema)
        src_client = SrcClient(client_schema)
        
        header_stream.write(header_client.code(code_style=QtGenerator.code_style, indent_level=0))
        src_stream.write(src_client.code(code_style=QtGenerator.code_style, indent_level=0))
        

    def _generate_api(self, api_schema: ApiSchema,
                      header_stream: TextIOWrapper,
                      src_stream: TextIOWrapper,
                      include_dependencies: Iterable[Path]) -> None:
        header_api = HeaderApi(api_schema)
        src_api = SrcApi(api_schema)
        
        header_stream.write(header_api.code(code_style=QtGenerator.code_style, indent_level=0))
        src_stream.write(src_api.code(code_style=QtGenerator.code_style, indent_level=0))

    def _generate_model(self,
                        model_schema: ModelSchema,
                        header_stream: TextIOWrapper,
                        src_stream: TextIOWrapper) -> None:
        header_model = HeaderModel(model_schema)
        src_model = SrcModel(model_schema)
        
        header_stream.write(header_model.code(code_style=QtGenerator.code_style, indent_level=0))
        src_stream.write(src_model.code(code_style=QtGenerator.code_style, indent_level=0))
