"""QML Generator"""

from io import TextIOWrapper
from pathlib import Path
from code_generator import constructs, writer

from ..schema import client, api, model, Schema
from .generator import Generator


class QmlSchemaGenerator:
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
    
    
class QmlClientGenerator(QmlSchemaGenerator):
    external_includes = ["egnite/rest/client.h"]
    
    def __init__(self,
                 client_schema: client.Client) -> None:
        super().__init__(client_schema)
        self.client_schema = client_schema
    
    def _header_body(self) -> writer.CodeWriter:
        wr = writer.CodeWriter()
        wr.add_lines(self._class().definition)
        return wr
    
    def _src_body(self) -> writer.CodeWriter:
        wr = writer.CodeWriter()
        wr.add_lines((f"{method.definition}\n" for method in self._methods()))
        return wr
    
    def _class(self) -> constructs.CppClass:
        model_class = constructs.CppClass(name=self.client_schema.name, parents="public egnite::rest::Client")
        model_class.add_code("Q_OBJECT")
        model_class.add_code("")
        model_class.add_code("public:")
        model_class.add_code((method.declaration for method in self._methods()))
        model_class.add_code("")
        model_class.add_code("private:")
        model_class.add_code(self._variables().code)
        return model_class
    
    def _methods(self) -> list[constructs.CppClassMethod]:
        methods: list[constructs.CppClassMethod] = []
        methods.append(self._constructor())
        methods.append(self._destructor())
        methods.extend(self._api_getters())
        return methods
    
    def _constructor(self) -> constructs.CppClassConstructor:
        constructor = constructs.CppClassConstructor(
            class_owner=self.client_schema.name, 
            arguments="QObject* parent = nullptr",
            qualifiers="explicit")
        constructor.add_initialization("egnite::rest::Client(parent)")
        constructor.add_code(self._constructor_body().code)
        return constructor
    
    def _constructor_body(self) -> writer.CodeWriter:
        wr = writer.CodeWriter()
        wr.add_line(f'setBaseUrl(QUrl("{self.client_schema.base_url}"));')
        wr.add_line(f'setVersion(QVersionNumber::fromString("{self.client_schema.version}"));')
        wr.add_lines(self._set_global_headers("_headers").code)
        wr.add_line()
        wr.add_lines(self._set_global_parameters("_parameters").code)
        return wr
    
    def _set_global_headers(self, 
                            variable: str) -> writer.CodeWriter:
        wr = writer.CodeWriter()
        if self.client_schema.headers:
            wr.add_line(f"auto {variable} = getGlobalHeaders();")
            for header in self.client_schema.headers:
                wr.add_line(f'{variable}["{header.key}"] = "{header.value}";')
            wr.add_line(f"setGlobalHeaders({variable});")
        return wr
    
    def _set_global_parameters(self, 
                               variable: str) -> writer.CodeWriter:
        wr = writer.CodeWriter()
        if self.client_schema.parameters:
            wr.add_line(f"auto {variable} = getGlobalParameters();")
            for parameter in self.client_schema.parameters:
                wr.add_line(f'{variable}.addQueryItem("{parameter.key}", "{parameter.value}");')
            wr.add_line(f"setGlobalParameters({variable});")
        return wr
    
    def _destructor(self) -> constructs.CppClassDestructor:
        destructor = constructs.CppClassDestructor(
            class_owner=self.client_schema.name, 
            qualifiers="override")
        return destructor
    
    def _api_getters(self) -> list[constructs.CppClassMethod]:
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
        
    def _variables(self) -> writer.CodeWriter:
        wr = writer.CodeWriter()
        for api in self.client_schema.apis:
            wr.add_line(f"{api.type}* m_{api.name};")
        return wr
    

class QmlApiGenerator(QmlSchemaGenerator):
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
        wr.add_lines(self._class().definition)
        return wr
    
    def _src_body(self) -> writer.CodeWriter:
        wr = writer.CodeWriter()
        wr.add_lines((f"{method.definition}\n" for method in self._methods()))
        return wr
        
    def _class(self) -> constructs.CppClass:
        model_class = constructs.CppClass(name=self.api_schema.name, parents="public egnite::rest::Api")
        model_class.add_code("Q_OBJECT")
        model_class.add_code("")
        model_class.add_code("public:")
        model_class.add_code((method.declaration for method in self._methods()))
        return model_class
    
    def _methods(self) -> list[constructs.CppClassMethod]:
        methods: list[constructs.CppClassMethod] = []
        methods.append(self._constructor())
        methods.append(self._destructor())
        methods.extend(self._reply_methods())
        return methods
    
    def _constructor(self) -> constructs.CppClassConstructor:
        constructor = constructs.CppClassConstructor(
            class_owner=self.api_schema.name, 
            arguments=["egnite::rest::IClient* client", "QObject* parent = nullptr"],
            qualifiers="explicit")
        constructor.add_initialization('egnite::rest::Api(client, "", parent)')
        constructor.add_code(self._constructor_body().code)
        return constructor
        
    def _constructor_body(self) -> writer.CodeWriter:
        wr = writer.CodeWriter()
        wr.add_lines(self._set_global_headers("_headers").code)
        wr.add_line()
        wr.add_lines(self._set_global_parameters("_parameters").code)
        return wr
    
    def _set_global_headers(self, 
                            variable: str) -> writer.CodeWriter:
        wr = writer.CodeWriter()
        if self.api_schema.headers:
            wr.add_line(f"auto {variable} = getGlobalHeaders();")
            for header in self.api_schema.headers:
                wr.add_line(f'{variable}["{header.key}"] = "{header.value}";')
            wr.add_line(f"setGlobalHeaders({variable});")
        return wr
    
    def _set_global_parameters(self, 
                               variable: str) -> writer.CodeWriter:
        wr = writer.CodeWriter()
        if self.api_schema.parameters:
            wr.add_line(f"auto {variable} = getGlobalParameters();")
            for parameter in self.api_schema.parameters:
                wr.add_line(f'{variable}.addQueryItem("{parameter.key}", "{parameter.value}");')
            wr.add_line(f"setGlobalParameters({variable});")
        return wr
    
    def _destructor(self) -> constructs.CppClassDestructor:
        destructor = constructs.CppClassDestructor(
            class_owner=self.api_schema.name, 
            qualifiers="override")
        return destructor
    
    def _reply_methods(self) -> list[constructs.CppClassMethod]:
        reply_methods: list[constructs.CppClassMethod] = []
        for method in self.api_schema.methods:
            reply_method = constructs.CppClassMethod(
                name=method.name,
                class_owner=self.api_schema.name, 
                return_type=f"egnite::rest::GenericReply<{method.returns}, {method.excepts}>*",
                arguments=self._reply_method_arguments(method),
                qualifiers="[[nodiscard]]")
            reply_methods.append(reply_method)    
            reply_method.add_code(self._reply_method_body(method).code)
        return reply_methods
    
    def _reply_method_arguments(self,
                                method: api.Method) -> list[str]:
        arguments: list[str] = []
        for api_segment in self.api_schema.path.param_segments:
            arguments.append(f"{api_segment.type} {api_segment.name}")
        if self._reply_method_use_body(method):
            arguments.append(f"const {method.body}& data")
        for method_segment in method.path.param_segments:
            arguments.append(f"{method_segment.type} {method_segment.name}")
        for argument in method.arguments:
            arguments.append(f"{argument.type} {argument.name}") 
        arguments.append("QObject* parent = nullptr")
        return arguments
    
    def _reply_method_body(self, 
                           method: api.Method) -> writer.CodeWriter:
        wr = writer.CodeWriter()
        wr.add_lines(self._reply_method_parameters_variable(
            method=method, 
            variable="_parameters").code)
        wr.add_line()
        wr.add_lines(self._reply_method_headers_variable(
            method=method, 
            variable="_headers").code)
        wr.add_line()
        wr.add_lines(self._reply_method_path_variable(
            method=method, 
            variable="_path").code)
        wr.add_line()
        wr.add_line(self._reply_call(
            method=method, 
            path="_path.join('/')",
            parameters="_parameters",
            headers="_headers").code)
        return wr
    
    def _reply_method_path_variable(self, 
                                    method: api.Method, 
                                    variable: str) -> writer.CodeWriter:
        def segment_to_value(segment):
             if isinstance(segment, api.ParamSegment): 
                return segment.name
             elif isinstance(segment, api.FixedSegment):
                return f'"{segment.value}"'

        wr = writer.CodeWriter()
        wr.add_line(f"QStringList {variable};")
        for api_segment in self.api_schema.path.segments:
            wr.add_line(f'{variable} << QString("%1").arg({segment_to_value(api_segment)});')
        for method_segment in method.path.segments:
            wr.add_line(f'{variable} << QString("%1").arg({segment_to_value(method_segment)});')
        return wr
 
    def _reply_method_parameters_variable(self, 
                                          method: api.Method, 
                                          variable: str) -> writer.CodeWriter:
        wr = writer.CodeWriter()
        wr.add_line(f"QUrlQuery {variable};")
        for parameter in method.parameters:
            wr.add_line(f'{variable}.addQueryItem("{parameter.key}", "{parameter.value}");') 
        for argument in method.arguments:
            wr.add_line(f'{variable}.addQueryItem("{argument.key}", {argument.name});')    
        return wr
    
    def _reply_method_headers_variable(self, 
                                       method: api.Method, 
                                       variable: str) -> writer.CodeWriter:
        wr = writer.CodeWriter()
        wr.add_line(f"egnite::rest::Headers {variable};")
        for header in method.headers:
            wr.add_line(f'{variable}["{header.key}"] = "{header.value}";')
        return wr
    
    def _reply_call(self, 
                    method: api.Method, 
                    path: str,
                    parameters: str, 
                    headers: str) -> writer.CodeWriter:
        wr = writer.CodeWriter()
        function = self.__class__.reply_methods[method.verb]
        template = f"<{method.returns}, {method.excepts}>"
        arguments = f'({path}, {parameters}, {headers}, parent)' 
        if self._reply_method_use_body(method):
            template = f"<{method.returns}, {method.excepts}, {method.body}>"
            arguments = f'({path}, data, {parameters}, {headers}, parent)'
        wr.add_line(f"return {function}{template}{arguments};")
        return wr
        
    def _reply_method_use_body(self, 
                               method: api.Method) -> bool:
        return method.verb in self.__class__.verb_use_body
        
    
class QmlModelGenerator(QmlSchemaGenerator):
    external_includes = ["QObject"]
    
    def __init__(self,
                 model_schema: model.Model) -> None:
        super().__init__(model_schema)
        self.model_schema = model_schema
    
    def _header_body(self) -> writer.CodeWriter:
        wr = writer.CodeWriter()
        wr.add_lines(self._class().definition)
        return wr
    
    def _class(self) -> constructs.CppClass:
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


class QmlGenerator(Generator):
    def _generate_client(self,
                         client_schema: client.Client,
                         header_stream: TextIOWrapper,
                         src_stream: TextIOWrapper) -> None:
        client_generator = QmlClientGenerator(client_schema=client_schema)
        client_generator.write(header_stream=header_stream, 
                              src_stream=src_stream)
        
    def _generate_api(self, api_schema: api.Api,
                      header_stream: TextIOWrapper,
                      src_stream: TextIOWrapper) -> None:
        api_generator = QmlApiGenerator(api_schema=api_schema)
        api_generator.write(header_stream=header_stream, 
                              src_stream=src_stream)

    def _generate_model(self,
                        model_schema: model.Model,
                        header_stream: TextIOWrapper,
                        src_stream: TextIOWrapper) -> None:
        model_generator = QmlModelGenerator(model_schema=model_schema)
        model_generator.write(header_stream=header_stream, 
                              src_stream=src_stream)
