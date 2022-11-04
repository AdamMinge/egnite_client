"""QT Generator"""

from io import TextIOWrapper
from pathlib import Path
from code_generator import code_generator

from ..schema import ClientSchema, ApiSchema, ModelSchema
from .generator import Generator

class HeaderClient(code_generator.CppFile):
    def __init__(self, 
                 client_schema: ClientSchema) -> None:
        super().__init__(f"SCHEMA_GENERATED_CLIENT_{client_schema.name.upper()}_H")
        
        self.add_element(code_generator.CppLine())
        self.add_element(self._create_include_headers_scope(client_schema))
        self.add_element(code_generator.CppLine())
        self.add_element(self._create_class(client_schema))
        self.add_element(code_generator.CppLine())
        
    def _create_include_headers_scope(self,
                                      client_schema: ClientSchema) -> code_generator.CppScope:
        scope = code_generator.CppScope()
        external_includes = [
            "egnite/rest/client.h",
        ]
        for include in external_includes:
            scope.add_element(code_generator.CppLine(f'#include <{include}>'))   
        if client_schema.includes:
            scope.add_element(code_generator.CppLine())
            for include in client_schema.includes:
                scope.add_element(code_generator.CppLine(f'#include "{include}"'))
        return scope
    
    def _create_class(self, 
                      client_schema: ClientSchema) -> code_generator.CppClass:
        client_class = code_generator.CppClass(client_schema.name, parents = "public egnite::rest::Client")
        client_class.add_element(code_generator.CppLine("Q_OBJECT"))
        client_class.add_element(code_generator.CppLine())
        client_class.add_element(self._public_class_interface_scope(client_schema))
        client_class.add_element(code_generator.CppLine())
        client_class.add_element(self._private_class_interface_scope(client_schema))
        return client_class
    
    def _public_class_interface_scope(self, 
                                      client_schema: ClientSchema) -> code_generator.CppScope:
        scope = code_generator.CppScope(begin=code_generator.CppLine("public:"), indent_elements=True)
        scope.add_element(self._create_constructors_scope(client_schema))
        scope.add_element(code_generator.CppLine())
        scope.add_element(self._create_apis_getters_scope(client_schema))
        return scope
    
    def _private_class_interface_scope(self, 
                                       client_schema: ClientSchema) -> code_generator.CppScope:
        scope = code_generator.CppScope(begin=code_generator.CppLine("private:"), indent_elements=True)
        scope.add_element(self._create_apis_scope(client_schema))
        return scope
        
    def _create_constructors_scope(self, 
                                   client_schema: ClientSchema) -> code_generator.CppScope:
        scope = code_generator.CppScope()
        scope.add_element(code_generator.CppConstructor(
            name=client_schema.name, 
            arguments= "QObject* parent = nullptr", 
            qualifiers= "explicit").declaration())
        scope.add_element(code_generator.CppDestructor(
            name=client_schema.name, 
            qualifiers= "override").declaration())
        return scope
    
    def _create_apis_getters_scope(self, 
                           client_schema: ClientSchema) -> code_generator.CppScope:
        scope = code_generator.CppScope()
        for api in client_schema.apis:
            scope.add_element(code_generator.CppFunction(
                name=api.name, 
                return_type= f"{api.type}*",
                qualifiers= ["const", "[[nodiscard]]"]).declaration())
        return scope
    
    def _create_apis_scope(self, 
                           client_schema: ClientSchema) -> code_generator.CppScope:
        scope = code_generator.CppScope()
        for api in client_schema.apis:
            scope.add_element(code_generator.CppLine(f"{api.type}* m_{api.name};"))
        return scope
    
        
class SrcClient(code_generator.CppFile):
    def __init__(self, 
                 client_schema: ClientSchema,
                 header_include: str) -> None:
        super().__init__()
        
        self.add_element(self._create_include_headers_scope(header_include))
        self.add_element(code_generator.CppLine())
        self.add_element(self._create_class_implementation(client_schema))
        
    def _create_include_headers_scope(self, 
                                      header_include: str) -> code_generator.CppScope:
        scope = code_generator.CppScope()
        scope.add_element(code_generator.CppLine(f'#include "{header_include}"'))
        return scope
    
    def _create_class_implementation(self, 
                                     client_schema: ClientSchema) -> code_generator.CppScope:
        scope = code_generator.CppScope()
        scope.add_element(self._create_constructors_scope(client_schema))
        scope.add_element(code_generator.CppLine())
        scope.add_element(self._create_apis_getters_scope(client_schema))
        return scope
    
    def _create_constructors_scope(self, 
                                   client_schema: ClientSchema) -> code_generator.CppScope:
        scope = code_generator.CppScope()
        
        constructor_initializations = ["egnite::rest::Client(parent)"]
        for api in client_schema.apis:
            constructor_initializations.append(f"m_{api.name}(new {api.type}(this))")
            
        constructor = code_generator.CppConstructor(
            name=client_schema.name, 
            namespace=client_schema.name,
            arguments= "QObject* parent",
            initializations=constructor_initializations)
        constructor.add_element(code_generator.CppLine(
            txt=f'setBaseUrl(QUrl("{client_schema.base_url}"));'))
        constructor.add_element(code_generator.CppLine(
            txt=f'setVersion(QVersionNumber::fromString("{client_schema.version}"));'))
        
        scope.add_element(constructor)
        scope.add_element(code_generator.CppLine())
        scope.add_element(code_generator.CppDestructor(
            name=client_schema.name,
            namespace=client_schema.name))
        return scope
    
    def _create_apis_getters_scope(self, 
                           client_schema: ClientSchema) -> code_generator.CppScope:
        scope = code_generator.CppScope()
        for api in client_schema.apis:
            api_function = code_generator.CppFunction(
                name=api.name,
                namespace=client_schema.name, 
                return_type= f"{api.type}*",
                qualifiers="const")
            api_function.add_element(code_generator.CppLine(f"return m_{api.name};"))
            
            scope.add_element(api_function)
        return scope


class HeaderApi(code_generator.CppFile):
    def __init__(self, 
                 api_schema: ApiSchema) -> None:
        super().__init__(f"SCHEMA_GENERATED_API_{api_schema.name.upper()}_H")
        
        self.add_element(code_generator.CppLine())
        self.add_element(self._create_include_headers_scope(api_schema))
        self.add_element(code_generator.CppLine())
        self.add_element(self._create_class(api_schema))
        self.add_element(code_generator.CppLine())
        
    def _create_class(self, 
                      api_schema: ApiSchema) -> code_generator.CppClass:
        api_class = code_generator.CppClass(api_schema.name, parents = "public egnite::rest::Api")
        api_class.add_element(code_generator.CppLine("Q_OBJECT"))
        api_class.add_element(code_generator.CppLine())
        api_class.add_element(self._public_class_interface_scope(api_schema))
        return api_class
    
    def _public_class_interface_scope(self, 
                                      api_schema: ApiSchema) -> code_generator.CppScope:
        scope = code_generator.CppScope(begin=code_generator.CppLine("public:"), indent_elements=True)
        scope.add_element(self._create_constructors_scope(api_schema))
        scope.add_element(code_generator.CppLine())
        scope.add_element(self._create_methods_scope(api_schema))
        return scope
         
    def _create_include_headers_scope(self,
                                      api_schema: ApiSchema) -> code_generator.CppScope:
        scope = code_generator.CppScope()
        external_includes = [
            "egnite/rest/api.h",
        ]
        for include in external_includes:
            scope.add_element(code_generator.CppLine(f'#include <{include}>'))      
        if api_schema.includes:  
            scope.add_element(code_generator.CppLine())
            for include in api_schema.includes:
                scope.add_element(code_generator.CppLine(f'#include "{include}"'))
        return scope

    def _create_constructors_scope(self, 
                                   api_schema: ApiSchema) -> code_generator.CppScope:
        scope = code_generator.CppScope()
        scope.add_element(code_generator.CppConstructor(
            name=api_schema.name, 
            arguments= ["egnite::rest::IClient* client", "QObject* parent = nullptr"], 
            qualifiers= "explicit").declaration())
        scope.add_element(code_generator.CppDestructor(
            name=api_schema.name, 
            qualifiers= "override").declaration())
        return scope
    
    def _create_methods_scope(self, 
                              api_schema: ApiSchema) -> code_generator.CppScope:
        scope = code_generator.CppScope()
        for method in api_schema.methods:
            scope.add_element(code_generator.CppFunction(
                name=method.name,
                return_type=f"egnite::rest::GenericReply<{method.returns[0]}, {method.returns[1]}>",
                arguments=[]
            ).declaration())
        return scope


class SrcApi(code_generator.CppFile):
    def __init__(self, 
                 api_schema: ApiSchema,
                 header_include: str) -> None:
        super().__init__()
    
        self.add_element(self._create_include_headers_scope(header_include))
        self.add_element(code_generator.CppLine())
        self.add_element(self._create_class_implementation(api_schema))
        
    def _create_include_headers_scope(self, 
                                      header_include: str) -> code_generator.CppScope:
        scope = code_generator.CppScope()
        scope.add_element(code_generator.CppLine(f'#include "{header_include}"'))   
        return scope
    
    def _create_class_implementation(self, 
                                     api_schema: ApiSchema) -> code_generator.CppScope:
        scope = code_generator.CppScope()
        scope.add_element(self._create_constructors_scope(api_schema))
        scope.add_element(code_generator.CppLine())
        scope.add_element(self._create_methods_scope(api_schema))
        return scope
    
    def _create_constructors_scope(self, 
                                   api_schema: ApiSchema) -> code_generator.CppScope:
        scope = code_generator.CppScope()
        
        constructor_initializations = [f'egnite::rest::Api(client, "{api_schema.path}", parent)']
            
        scope.add_element(code_generator.CppConstructor(
            name=api_schema.name, 
            namespace=api_schema.name,
            arguments= ["egnite::rest::IClient* client", "QObject* parent"], 
            initializations=constructor_initializations))
        scope.add_element(code_generator.CppLine())
        scope.add_element(code_generator.CppDestructor(
            name=api_schema.name,
            namespace=api_schema.name))
        return scope
    
    def _create_methods_scope(self, 
                              api_schema: ApiSchema) -> code_generator.CppScope:
        scope = code_generator.CppScope()
        for method in api_schema.methods:
            scope.add_element(code_generator.CppFunction(
                name=method.name,
                return_type=f"egnite::rest::GenericReply<{method.returns[0]}, {method.returns[1]}>",
                arguments=[]
            ))
            scope.add_element(code_generator.CppLine())
        return scope


class HeaderModel(code_generator.CppFile):
    def __init__(self, 
                 model_schema: ModelSchema) -> None:
        super().__init__(f"SCHEMA_GENERATED_MODEL_{model_schema.name.upper()}_H")
        
        self.add_element(code_generator.CppLine())
        self.add_element(self._create_include_headers_scope(model_schema))
        self.add_element(code_generator.CppLine())
        self.add_element(self._create_struct(model_schema))
        self.add_element(code_generator.CppLine())
        
    def _create_include_headers_scope(self,
                                      model_schema: ModelSchema) -> code_generator.CppScope:
        scope = code_generator.CppScope()
        external_includes = [
            "QObject",
        ]
        for include in external_includes:
            scope.add_element(code_generator.CppLine(f'#include <{include}>'))    
        if model_schema.includes:  
            scope.add_element(code_generator.CppLine())
            for include in model_schema.includes:
                scope.add_element(code_generator.CppLine(f'#include "{include}"'))  
        return scope
    
    def _create_struct(self, 
                       model_schema: ModelSchema) -> code_generator.CppStruct:
        
        model_struct = code_generator.CppStruct(model_schema.name)
        model_struct.add_element(code_generator.CppLine("Q_GADGET"))
        model_struct.add_element(self._create_properties_scope(model_schema))
        model_struct.add_element(code_generator.CppLine())
        model_struct.add_element(self._public_class_interface_scope(model_schema))
        return model_struct
    
    def _create_properties_scope(self, 
                                 model_schema: ModelSchema) -> code_generator.CppScope:
        scope = code_generator.CppScope()
        for property in model_schema.properties:
            scope.add_element(code_generator.CppLine(f"Q_PROPERTY({property.type} {property.name} MEMBER {property.name})"))  
        return scope
    
    def _public_class_interface_scope(self, 
                                      model_schema: ModelSchema) -> code_generator.CppScope:
        scope = code_generator.CppScope(begin=code_generator.CppLine("public:"), indent_elements=True)
        scope.add_element(self._create_members_scope_scope(model_schema))
        return scope
    
    def _create_members_scope_scope(self,
                                    model_schema: ModelSchema) -> code_generator.CppScope:
        scope = code_generator.CppScope()
        for property in model_schema.properties:
            scope.add_element(code_generator.CppLine(f"{property.type} {property.name};"))
        return scope


class SrcModel(code_generator.CppFile):
    def __init__(self, 
                 model_schema: ModelSchema,
                 header_include: str) -> None:
        super().__init__()
        
        self.add_element(self._create_include_headers_scope(header_include))
        self.add_element(code_generator.CppLine())
        
    def _create_include_headers_scope(self, 
                                      header_include: str) -> code_generator.CppScope:
        scope = code_generator.CppScope()
        scope.add_element(code_generator.CppLine(f'#include "{header_include}"'))
        return scope
        

class QtGenerator(Generator):
    code_style = code_generator.CppCodeStyle(
        indent=" "*2, 
        lf="\n"
    )
    
    def _generate_client(self,
                         client_schema: ClientSchema,
                         header_stream: TextIOWrapper,
                         src_stream: TextIOWrapper) -> None:
        header_client = HeaderClient(client_schema)
        src_client = SrcClient(client_schema, Path(header_stream.name).name)
        
        header_stream.write(header_client.code(code_style=QtGenerator.code_style, indent_level=0))
        src_stream.write(src_client.code(code_style=QtGenerator.code_style, indent_level=0))
        
    def _generate_api(self, api_schema: ApiSchema,
                      header_stream: TextIOWrapper,
                      src_stream: TextIOWrapper) -> None:
        header_api = HeaderApi(api_schema)
        src_api = SrcApi(api_schema, Path(header_stream.name).name)
        
        header_stream.write(header_api.code(code_style=QtGenerator.code_style, indent_level=0))
        src_stream.write(src_api.code(code_style=QtGenerator.code_style, indent_level=0))

    def _generate_model(self,
                        model_schema: ModelSchema,
                        header_stream: TextIOWrapper,
                        src_stream: TextIOWrapper) -> None:
        header_model = HeaderModel(model_schema)
        src_model = SrcModel(model_schema, Path(header_stream.name).name)
        
        
        header_stream.write(header_model.code(code_style=QtGenerator.code_style, indent_level=0))
        src_stream.write(src_model.code(code_style=QtGenerator.code_style, indent_level=0))
