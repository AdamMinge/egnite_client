"""QML Generator"""

from io import TextIOWrapper
from pathlib import Path
from code_generator import constructs, writer

from ..generator import Generator
from .schema import (
    ClientSchema,
    ApiSchema, 
    MethodSchema,
    NameTypeSchema,
    ValueSchema)


QmlSchema = ClientSchema | ApiSchema


class QmlSchemaGenerator:
    external_includes = []
    
    def __init__(self,
                 schema: QmlSchema) -> None:
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
    external_includes = ["client.h"]
    
    def __init__(self,
                 client_schema: ClientSchema) -> None:
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
        model_class = constructs.CppClass(name=self.client_schema.name, parents="public QmlClient")
        model_class.add_code("Q_OBJECT")
        model_class.add_code("QML_ELEMENT")
        model_class.add_code("")
        model_class.add_code("public:")
        model_class.add_code((method.declaration for method in self._methods()))
        return model_class
    
    def _methods(self) -> list[constructs.CppClassMethod]:
        methods: list[constructs.CppClassMethod] = []
        methods.append(self._constructor())
        methods.append(self._destructor())
        return methods
    
    def _constructor(self) -> constructs.CppClassConstructor:
        constructor = constructs.CppClassConstructor(
            class_owner=self.client_schema.name, 
            arguments="QObject* parent = nullptr",
            qualifiers="explicit")
        constructor.add_initialization("QmlClient(parent)")
        constructor.add_code(self._constructor_body().code)
        return constructor
    
    def _constructor_body(self) -> writer.CodeWriter:
        wr = writer.CodeWriter()
        wr.add_line(f'setBaseUrl(QUrl("{self.client_schema.base_url}"));')
        wr.add_line(f'setVersion(QVersionNumber::fromString("{self.client_schema.version}"));')
        
        set_global_headers_writer = self._set_global_headers("_headers")
        set_global_parameters_writer = self._set_global_parameters("_parameters")
        
        if set_global_headers_writer:
            wr.add_line()
            wr.add_lines(set_global_headers_writer.code)
            
        if set_global_parameters_writer:
            wr.add_line()
            wr.add_lines(set_global_parameters_writer.code)

        return wr
    
    def _set_global_headers(self, 
                            variable: str) -> writer.CodeWriter:
        wr = writer.CodeWriter()
        if self.client_schema.headers:
            wr.add_line(f"egnite::rest::Headers {variable};")
            for header in self.client_schema.headers:
                wr.add_line(f'{variable}["{header.key}"] = "{header.value}";')
            wr.add_line(f"setGlobalHeaders({variable});")
        return wr
    
    def _set_global_parameters(self, 
                               variable: str) -> writer.CodeWriter:
        wr = writer.CodeWriter()
        if self.client_schema.parameters:
            wr.add_line(f"QUrlQuery {variable};")
            for parameter in self.client_schema.parameters:
                wr.add_line(f'{variable}.addQueryItem("{parameter.key}", "{parameter.value}");')
            wr.add_line(f"setGlobalParameters({variable});")
        return wr
    
    def _destructor(self) -> constructs.CppClassDestructor:
        destructor = constructs.CppClassDestructor(
            class_owner=self.client_schema.name, 
            qualifiers="override")
        return destructor
    

class QmlApiGenerator(QmlSchemaGenerator):
    external_includes = ["api.h"]
    verb_use_body = ["POST", "PUT", "PATCH"]
    verbs = {
        "POST": "egnite::rest::IApi::PostVerb",
        "PUT": "egnite::rest::IApi::PutVerb",
        "PATCH": "egnite::rest::IApi::PatchVerb",
        "DELETE": "egnite::rest::IApi::DeleteVerb",
        "HEAD": "egnite::rest::IApi::HeadVerb",
        "GET": "egnite::rest::IApi::GetVerb"
    }
    
    def __init__(self,
                 api_schema: ApiSchema) -> None:
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
        model_class = constructs.CppClass(name=self.api_schema.name, parents="public QmlApi")
        model_class.add_code("Q_OBJECT")
        model_class.add_code("QML_ELEMENT")
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
            arguments=["QObject* parent = nullptr"],
            qualifiers="explicit")
        constructor.add_initialization('QmlApi(parent)')
        constructor.add_code(self._constructor_body().code)
        return constructor
    
    def _constructor_body(self) -> writer.CodeWriter:
        wr = writer.CodeWriter()
        
        set_global_headers_writer = self._set_global_headers("_headers")
        set_global_parameters_writer = self._set_global_parameters("_parameters")
        
        if set_global_headers_writer:
            wr.add_lines(set_global_headers_writer.code)
            
        if set_global_parameters_writer:
            wr.add_line()
            wr.add_lines(set_global_parameters_writer.code)
        
        return wr
    
    def _set_global_headers(self, 
                            variable: str) -> writer.CodeWriter:
        wr = writer.CodeWriter()
        if self.api_schema.headers:
            wr.add_line(f"egnite::rest::Headers {variable};")
            for header in self.api_schema.headers:
                wr.add_line(f'{variable}["{header.key}"] = "{header.value}";')
            wr.add_line(f"setGlobalHeaders({variable});")
        return wr
    
    def _set_global_parameters(self, 
                               variable: str) -> writer.CodeWriter:
        wr = writer.CodeWriter()
        if self.api_schema.parameters:
            wr.add_line(f"QUrlQuery {variable};")
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
                return_type=f"QmlReply*",
                arguments=self._reply_method_arguments(method),
                qualifiers=["Q_INVOKABLE", "[[nodiscard]]"])
            reply_methods.append(reply_method)    
            reply_method.add_code(self._reply_method_body(method).code)
        return reply_methods
    
    def _reply_method_arguments(self,
                                method: MethodSchema) -> list[str]:
        arguments: list[str] = []
        for api_segment in self.api_schema.path.param_segments:
            arguments.append(f"const QJSValue& {api_segment.name}")
        if self._reply_method_use_body(method):
            arguments.append(f"const QJSValue& data")
        for method_segment in method.path.param_segments:
            arguments.append(f"const QJSValue& {method_segment.name}")
        for argument in method.arguments:
            arguments.append(f"const QJSValue& {argument.name}") 
        return arguments
    
    def _reply_method_body(self, 
                           method: MethodSchema) -> writer.CodeWriter:
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
                                    method: MethodSchema, 
                                    variable: str) -> writer.CodeWriter:
        def segment_to_value(segment):
             if isinstance(segment, NameTypeSchema): 
                return f"QJSValueToPath<{segment.type}>({segment.name})"
             elif isinstance(segment, ValueSchema):
                return f'"{segment.value}"'

        wr = writer.CodeWriter()
        wr.add_line(f"QStringList {variable};")
        for api_segment in self.api_schema.path.segments:
            wr.add_line(f'{variable} << {segment_to_value(api_segment)};')
        for method_segment in method.path.segments:
            wr.add_line(f'{variable} << {segment_to_value(method_segment)};')
        return wr
 
    def _reply_method_parameters_variable(self, 
                                          method: MethodSchema, 
                                          variable: str) -> writer.CodeWriter:
        wr = writer.CodeWriter()
        wr.add_line(f"QUrlQuery {variable};")
        for parameter in method.parameters:
            wr.add_line(f'{variable}.addQueryItem("{parameter.key}", "{parameter.value}");') 
        for argument in method.arguments:
            wr.add_line(f'{variable}.addQueryItem("{argument.key}", {argument.name}.toString());')    
        return wr
    
    def _reply_method_headers_variable(self, 
                                       method: MethodSchema, 
                                       variable: str) -> writer.CodeWriter:
        wr = writer.CodeWriter()
        wr.add_line(f"egnite::rest::Headers {variable};")
        for header in method.headers:
            wr.add_line(f'{variable}["{header.key}"] = "{header.value}";')
        return wr
    
    def _reply_call(self, 
                    method: MethodSchema, 
                    path: str,
                    parameters: str, 
                    headers: str) -> writer.CodeWriter:
        wr = writer.CodeWriter()
        verb = self.__class__.verbs[method.verb]
        arguments_without_data = f'{verb}, {path}, {parameters}, {headers}' 
        arguments_with_data = f'{verb}, {path}, body, {parameters}, {headers}'
        if not self._reply_method_use_body(method):
            wr.add_line(f"return createQmlReply(m_api->callRaw({arguments_without_data}));")
            return wr
        
        wr.add_line(f"const auto _opt_body = getBody(data);")
        wr.add_line("\n".join([
        "return createQmlReply(",
        "  std::visit(egnite::core::utils::overloaded{",
        "    [&](std::nullopt_t) -> egnite::rest::IReply* {",
        "      return m_api->callRaw({args});".format(args=arguments_without_data),
        "    },",
        "    [&](const auto& body) -> egnite::rest::IReply* {",
        "      return m_api->callRaw({args});".format(args=arguments_with_data),
        "    }},",
        "  *_opt_body));"]))
        return wr
    
    def _reply_method_use_body(self, 
                               method: MethodSchema) -> bool:
        return method.verb in self.__class__.verb_use_body


class QmlGenerator(Generator):
    def _supported_schemas(self) -> tuple[type, ...]:
        return (ClientSchema, ApiSchema)
    
    def _generate(self,
                  schema: QmlSchema,
                  header_stream: TextIOWrapper,
                  src_stream: TextIOWrapper) -> None:
        match schema:
            case ClientSchema():
                self._generate_client(
                    schema, header_stream, src_stream)
            case ApiSchema():
                self._generate_api(
                    schema, header_stream, src_stream)
    
    def _generate_client(self,
                         client_schema: ClientSchema,
                         header_stream: TextIOWrapper,
                         src_stream: TextIOWrapper) -> None:
        client_generator = QmlClientGenerator(client_schema=client_schema)
        client_generator.write(header_stream=header_stream, 
                               src_stream=src_stream)
        
    def _generate_api(self, api_schema: ApiSchema,
                      header_stream: TextIOWrapper,
                      src_stream: TextIOWrapper) -> None:
        api_generator = QmlApiGenerator(api_schema=api_schema)
        api_generator.write(header_stream=header_stream, 
                            src_stream=src_stream)
