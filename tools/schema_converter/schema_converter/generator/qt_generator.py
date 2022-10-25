"""QT Generator"""

from io import TextIOWrapper
from code_generator import generator

from ..schema import ClientSchema, ApiSchema, ModelSchema
from .generator import Generator


class HeaderClient(generator.CppFile):
    def __init__(self, 
                 client_schema: ClientSchema) -> None:
        super().__init__(f"SCHEMA_GENERATED_CLIENT_{client_schema.name.upper()}_H")
        
        self.add_element(generator.CppLine())
        self.add_element(self._create_include_headers_scope())
        self.add_element(generator.CppLine())
        self.add_element(self._create_apis_declaration_scope(client_schema))
        self.add_element(generator.CppLine())
        self.add_element(self._create_class(client_schema))
        self.add_element(generator.CppLine())
        
    def _create_include_headers_scope(self) -> generator.CppScope:
        scope = generator.CppScope()
        scope.add_element(generator.CppLine("#include <egnite/rest/client.h>"))
        return scope
    
    def _create_apis_declaration_scope(self, 
                                       client_schema: ClientSchema) -> generator.CppScope:
        scope = generator.CppScope()
        for api in client_schema.apis:
            scope.add_element(generator.CppLine(f"class {api.type};"))
        return scope
    
    def _create_class(self, 
                      client_schema: ClientSchema) -> generator.CppScope:
        client_class = generator.CppClass(client_schema.name, parents = "public egnite::rest::Client")
        client_class.add_element(generator.CppLine("Q_OBJECT"))
        client_class.add_element(generator.CppLine())
        client_class.add_element(generator.CppLine(" public:", offset_indent = -1))
        client_class.add_element(self._create_constructors_scope(client_schema))
        client_class.add_element(generator.CppLine())
        client_class.add_element(self._create_apis_scope(client_schema))
        return client_class
        
    def _create_constructors_scope(self, 
                                   client_schema: ClientSchema) -> generator.CppScope:
        scope = generator.CppScope()
        scope.add_element(generator.CppFunction(
            name=client_schema.name, 
            arguments= "QObject* parent = nullptr", 
            qualifiers= "explicit"))
        scope.add_element(generator.CppFunction(
            name=f"~{client_schema.name}", 
            qualifiers= "override"))
        return scope
    
    def _create_apis_scope(self, 
                           client_schema: ClientSchema) -> generator.CppScope:
        scope = generator.CppScope()
        for api in client_schema.apis:
            scope.add_element(generator.CppFunction(
                name=api.name, 
                return_type= f"{api.type}*",
                qualifiers= ["const", "[[nodiscard]]"]))
        return scope
    
        
class SrcClient(generator.CppFile):
    def __init__(self, 
                 client_schema: ClientSchema) -> None:
        super().__init__()


class HeaderApi(generator.CppFile):
    def __init__(self, 
                 api_schema: ApiSchema) -> None:
        super().__init__(f"SCHEMA_GENERATED_API_{api_schema.name.upper()}_H")
        
        self.add_element(generator.CppLine())
        self.add_element(self._create_include_headers_scope())
        self.add_element(generator.CppLine())
        self.add_element(self._create_class(api_schema))
        self.add_element(generator.CppLine())
        
    def _create_class(self, 
                      api_schema: ApiSchema) -> generator.CppClass:
        api_class = generator.CppClass(api_schema.name, parents = "public egnite::rest::Api")
        api_class.add_element(generator.CppLine("Q_OBJECT"))
        api_class.add_element(generator.CppLine())
        api_class.add_element(generator.CppLine(" public:", offset_indent = -1))
        return api_class
         
    def _create_include_headers_scope(self) -> generator.CppScope:
        scope = generator.CppScope()
        scope.add_element(generator.CppLine("#include <egnite/rest/api.h>"))
        return scope


class SrcApi(generator.CppFile):
    def __init__(self, 
                 api_schema: ApiSchema) -> None:
        super().__init__()


class HeaderModel(generator.CppFile):
    def __init__(self, 
                 model_schema: ModelSchema) -> None:
        super().__init__(f"SCHEMA_GENERATED_MODEL_{model_schema.name.upper()}_H")
        
        self.add_element(generator.CppLine())
        self.add_element(self._create_include_headers_scope())
        self.add_element(generator.CppLine())
        self.add_element(self._create_struct(model_schema))
        self.add_element(generator.CppLine())
        
    def _create_include_headers_scope(self) -> generator.CppScope:
        scope = generator.CppScope()
        scope.add_element(generator.CppLine("#include <QObject>"))
        return scope
    
    def _create_struct(self, 
                       model_schema: ModelSchema) -> generator.CppStruct:
        
        model_struct = generator.CppStruct(model_schema.name)
        model_struct.add_element(generator.CppLine("Q_GADGET"))
        model_struct.add_element(self._create_properties_scope(model_schema))
        model_struct.add_element(generator.CppLine())
        model_struct.add_element(generator.CppLine(" public:", offset_indent = -1))
        model_struct.add_element(self._create_members_scope_scope(model_schema))
        return model_struct
        
    def _create_properties_scope(self, 
                                 model_schema: ModelSchema) -> generator.CppScope:
        scope = generator.CppScope()
        for property in model_schema.properties:
            scope.add_element(generator.CppLine(f"Q_PROPERTY({property.type} {property.name} MEMBER {property.name})"))  
        return scope
    
    def _create_members_scope_scope(self,
                                    model_schema: ModelSchema) -> generator.CppScope:
        scope = generator.CppScope()
        for property in model_schema.properties:
            scope.add_element(generator.CppLine(f"{property.type} {property.name};"))
        return scope


class SrcModel(generator.CppFile):
    def __init__(self, 
                 model_schema: ModelSchema) -> None:
        super().__init__()
        

class QtGenerator(Generator):
    code_style = generator.CppCodeStyle(
        indent=" "*2, 
        lf="\n"
    )
    
    def _generate_client(self,
                         client_schema: ClientSchema,
                         header_stream: TextIOWrapper,
                         src_stream: TextIOWrapper) -> None:
        header_client = HeaderClient(client_schema)
        src_client = SrcClient(client_schema)
        header_stream.write(header_client.code(code_style=QtGenerator.code_style, indent_level=0))
        src_stream.write(src_client.code(code_style=QtGenerator.code_style, indent_level=0))
        

    def _generate_api(self, api_schema: ApiSchema,
                      header_stream: TextIOWrapper,
                      src_stream: TextIOWrapper) -> None:
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
