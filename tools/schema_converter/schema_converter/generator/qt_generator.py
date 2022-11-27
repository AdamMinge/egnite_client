"""QT Generator"""

from io import TextIOWrapper
from pathlib import Path
from typing import Iterable
from code_generator import constructs, writer

from ..schema import client, api, model, Schema
from .generator import Generator


class QtSchemaGenerator:
    external_includes = []
    
    def __init__(self,
                 schema: Schema) -> None:
        self.schema = schema
    
    def write(self,
              header_stream: TextIOWrapper,
              src_stream: TextIOWrapper):
        header_stream.write(self._write_header())
        src_stream.write(self._write_src(Path(header_stream.name))) 
        
    def _write_header(self) -> str:
        header_guard = self._include_guard_name()
        header_guard.add_code("")
        header_guard.add_code(iter(self._include_headers()))
        header_guard.add_code("")
        header_guard.add_code(iter(self._header_body()))
        header_guard.add_code("")
        return header_guard.code
    
    def _write_src(self,
                   header_path: Path) -> str:
        wr = writer.CodeWriter()
        wr.add_lines(iter(self._source_headers(header_path)))
        wr.add_lines("")
        wr.add_lines(iter(self._src_body()))
        return wr.code
    
    def _include_guard_name(self) -> constructs.CppIncludeGuard:
        return constructs.CppIncludeGuard(f"SCHEMA_GENERATED_{self.schema.name.upper()}_H")
    
    def _header_body(self) -> writer.CodeWriter:
        wr = writer.CodeWriter()
        return wr
    
    def _src_body(self) -> writer.CodeWriter:
        wr = writer.CodeWriter()
        return wr
    
    def _include_headers(self) -> writer.CodeWriter:
        wr = writer.CodeWriter()
        for include in self.__class__.external_includes:
            wr.add_line(text=f'#include <{include}>')
        if self.schema.includes:  
            wr.add_line()
            for include in self.schema.includes:
                wr.add_line(text=f'#include "{include}"')
        return wr
    
    def _source_headers(self, 
                        header_path: Path) -> writer.CodeWriter:
        wr = writer.CodeWriter()
        wr.add_line(text=f'#include "{header_path.name}"')
        return wr
    
    
class QtClientGenerator(QtSchemaGenerator):
    external_includes = ["egnite/rest/client.h"]
    
    def __init__(self,
                 client_schema: client.Client) -> None:
        super().__init__(client_schema)
        self.client_schema = client_schema
    
    def _header_body(self) -> writer.CodeWriter:
        wr = writer.CodeWriter()
        wr.add_lines(self._client_class().definition)
        return wr
    
    def _src_body(self) -> writer.CodeWriter:
        wr = writer.CodeWriter()
        wr.add_lines((f"{method.definition}\n" for method in self._client_class_methods()))
        return wr
    
    def _client_class(self) -> constructs.CppClass:
        model_class = constructs.CppClass(name=self.client_schema.name, parents="public egnite::rest::Client")
        model_class.add_code("Q_OBJECT")
        model_class.add_code("")
        model_class.add_code("public:")
        model_class.add_code((method.declaration for method in self._client_class_methods()))
        model_class.add_code("")
        model_class.add_code("private:")
        model_class.add_code(self._client_class_variables().code)
        return model_class
    
    def _client_class_methods(self) -> list[constructs.CppClassMethod]:
        methods: list[constructs.CppClassMethod] = []
        methods.append(self._client_class_constructor())
        methods.append(self._client_class_destructor())
        methods.extend(self._client_class_api_getters())
        return methods
    
    def _client_class_constructor(self) -> constructs.CppClassConstructor:
        constructor = constructs.CppClassConstructor(
            class_owner=self.client_schema.name, 
            arguments="QObject* parent = nullptr",
            qualifiers="explicit")
        constructor.add_initialization("egnite::rest::Client(parent)")
        constructor.add_code(self._client_class_constructor_body().code)
        return constructor
    
    def _client_class_constructor_body(self) -> writer.CodeWriter:
        wr = writer.CodeWriter()
        wr.add_line(f'setBaseUrl(QUrl("{self.client_schema.base_url}"));')
        wr.add_line(f'setVersion(QVersionNumber::fromString("{self.client_schema.version}"));')
        if self.client_schema.global_headers:
            wr.add_line("")
            wr.add_line("auto headers = getGlobalHeaders();")
            for header in self.client_schema.global_headers:
                wr.add_line(f'headers["{header.key}"] = "{header.value}";')
            wr.add_line("setGlobalHeaders(headers);")
        if self.client_schema.global_parameters:
            wr.add_line("")
            wr.add_line("auto parameters = getGlobalParameters();")
            for parameter in self.client_schema.global_parameters:
                wr.add_line(f'parameters.addQueryItem("{parameter.key}", "{parameter.value}");')
            wr.add_line("setGlobalParameters(parameters);")
        return wr
    
    def _client_class_destructor(self) -> constructs.CppClassDestructor:
        destructor = constructs.CppClassDestructor(
            class_owner=self.client_schema.name, 
            qualifiers="override")
        return destructor
    
    def _client_class_api_getters(self) -> list[constructs.CppClassMethod]:
        getters: list[constructs.CppClassMethod] = []
        for api in self.client_schema.apis:
            getter = constructs.CppClassMethod(
                name=api.name,
                class_owner=self.client_schema.name, 
                return_type=f"{api.type}*",
                qualifiers=["const", "[[nodiscard]]"])
            getter.add_code(f"return m_{api.name};")
            getters.append(getter)
        return getters
        
    def _client_class_variables(self) -> writer.CodeWriter:
        wr = writer.CodeWriter()
        for api in self.client_schema.apis:
            wr.add_line(f"{api.type}* m_{api.name};")
        return wr
    

class QtApiGenerator(QtSchemaGenerator):
    external_includes = ["egnite/rest/api.h"]
    verb_use_body = ["POST", "PUT", "PATCH"]
    reply_methods = {
        "POST": "post",
        "PUT": "put",
        "PATCH": "patch",
        "DELETE": "deleteResource",
        "HEAD": "head",
        "GET": "get"
    }
    
    def __init__(self,
                 api_schema: api.Api) -> None:
        super().__init__(api_schema)
        self.api_schema = api_schema
        
    def _header_body(self) -> writer.CodeWriter:
        wr = writer.CodeWriter()
        wr.add_lines(self._api_class().definition)
        return wr
    
    def _src_body(self) -> writer.CodeWriter:
        wr = writer.CodeWriter()
        wr.add_lines((f"{method.definition}\n" for method in self._api_class_methods()))
        return wr
        
    def _api_class(self) -> constructs.CppClass:
        model_class = constructs.CppClass(name=self.api_schema.name, parents="public egnite::rest::Api")
        model_class.add_code("Q_OBJECT")
        model_class.add_code("")
        model_class.add_code("public:")
        model_class.add_code((method.declaration for method in self._api_class_methods()))
        return model_class
    
    def _api_class_methods(self) -> list[constructs.CppClassMethod]:
        methods: list[constructs.CppClassMethod] = []
        methods.append(self._api_class_constructor())
        methods.append(self._api_class_destructor())
        methods.extend(self._api_class_reply_methods())
        return methods
    
    def _api_class_constructor(self) -> constructs.CppClassConstructor:
        constructor = constructs.CppClassConstructor(
            class_owner=self.api_schema.name, 
            arguments=["egnite::rest::IClient* client", "QObject* parent = nullptr"],
            qualifiers="explicit")
        constructor.add_initialization(f'egnite::rest::Api(client, "{self.api_schema.path}", parent)')
        constructor.add_code(self._api_class_constructor_body().code)
        return constructor
        
    def _api_class_constructor_body(self) -> writer.CodeWriter:
        wr = writer.CodeWriter()
        if self.api_schema.global_headers:
            wr.add_line("auto headers = getGlobalHeaders();")
            for header in self.api_schema.global_headers:
                wr.add_line(f'headers["{header.key}"] = "{header.value}";')
            wr.add_line("setGlobalHeaders(headers);")
        if self.api_schema.global_parameters:
            wr.add_line("")
            wr.add_line("auto parameters = getGlobalParameters();")
            for parameter in self.api_schema.global_parameters:
                wr.add_line(f'parameters.addQueryItem("{parameter.key}", "{parameter.value}");')
            wr.add_line("setGlobalParameters(parameters);")
        return wr
    
    def _api_class_destructor(self) -> constructs.CppClassDestructor:
        destructor = constructs.CppClassDestructor(
            class_owner=self.api_schema.name, 
            qualifiers="override")
        return destructor
    
    def _api_class_reply_methods(self) -> list[constructs.CppClassMethod]:
        reply_methods: list[constructs.CppClassMethod] = []
        for method in self.api_schema.methods:
            reply_method = constructs.CppClassMethod(
                name=method.name,
                class_owner=self.api_schema.name, 
                return_type=f"egnite::rest::GenericReply<{method.returns}, {method.excepts}>*",
                arguments=self._api_class_reply_method_arguments(method),
                qualifiers="[[nodiscard]]")
            reply_methods.append(reply_method)    
            reply_method.add_code(self._api_class_reply_method_body(method).code)
        return reply_methods
    
    def _api_class_reply_method_arguments(self,
                                          method: api.Method) -> list[str]:
        arguments: list[str] = []
        if self._api_class_reply_method_use_body(method):
            arguments.append(f"const {method.body}& data")
        for argument in method.arguments:
            arguments.append(f"{argument.type} {argument.key}")
        arguments.append("QObject* parent = nullptr")
        return arguments
    
    def _api_class_reply_method_body(self, 
                                     method: api.Method) -> writer.CodeWriter:
        wr = writer.CodeWriter()
        if method.parameters:
            wr.add_line("QUrlQuery parameters;")
            for parameter in method.parameters:
                wr.add_line(f'parameters.addQueryItem("{parameter.key}", "{parameter.value}");')    
            wr.add_line()    
        if method.headers:
            wr.add_line("egnite::rest::Headers headers;")
            for header in method.headers:
                wr.add_line(f'headers["{header.key}"] = "{header.value}";')
            wr.add_line()        
        wr.add_line(self._api_class_reply_call(
            method=method, 
            parameters_variable="parameters" if method.parameters else "{}",
            headers_variable="headers" if method.headers else "{}"))
        return wr
    
    def _api_class_reply_call(self, 
                              method: api.Method, 
                              parameters_variable: str, 
                              headers_variable: str) -> str:
        function = self.__class__.reply_methods[method.verb]
        template = f"<{method.returns}, {method.excepts}>"
        arguments = f'("", {parameters_variable}, {headers_variable}, parent)' 
        if self._api_class_reply_method_use_body(method):
            template = f"<{method.returns}, {method.excepts}, {method.body}>"
            arguments = f'("", data, {parameters_variable}, {headers_variable}, parent)'
        return f"return {function}{template}{arguments};"
        
    def _api_class_reply_method_use_body(self, 
                                         method: api.Method) -> bool:
        return method.verb in self.__class__.verb_use_body
        
    
class QtModelGenerator(QtSchemaGenerator):
    external_includes = ["QObject"]
    
    def __init__(self,
                 model_schema: model.Model) -> None:
        super().__init__(model_schema)
        self.model_schema = model_schema
    
    def _header_body(self) -> writer.CodeWriter:
        wr = writer.CodeWriter()
        wr.add_lines(self._model_class().definition)
        return wr
    
    def _model_class(self) -> constructs.CppClass:
        model_class = constructs.CppClass(
            name=self.schema.name,
            is_struct=True
        )
        model_class.add_code("Q_GADGET")
        for property in self.model_schema.properties:
            model_class.add_code(f"Q_PROPERTY({property.type} {property.name} MEMBER {property.name})")
        model_class.add_code("")
        for property in self.model_schema.properties:
            model_class.add_code(f"{property.type} {property.name};")
        return model_class  


class QtGenerator(Generator):
    def _generate_client(self,
                         client_schema: client.Client,
                         header_stream: TextIOWrapper,
                         src_stream: TextIOWrapper) -> None:
        client_generator = QtClientGenerator(client_schema=client_schema)
        client_generator.write(header_stream=header_stream, 
                              src_stream=src_stream)
        
    def _generate_api(self, api_schema: api.Api,
                      header_stream: TextIOWrapper,
                      src_stream: TextIOWrapper) -> None:
        api_generator = QtApiGenerator(api_schema=api_schema)
        api_generator.write(header_stream=header_stream, 
                              src_stream=src_stream)

    def _generate_model(self,
                        model_schema: model.Model,
                        header_stream: TextIOWrapper,
                        src_stream: TextIOWrapper) -> None:
        model_generator = QtModelGenerator(model_schema=model_schema)
        model_generator.write(header_stream=header_stream, 
                              src_stream=src_stream)
