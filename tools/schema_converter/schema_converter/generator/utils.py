"""Utils"""

from code_generator import code_generator

class Utils:
    @staticmethod
    def create_global_headers_scope(headers) -> code_generator.CppScope:
        scope = code_generator.CppScope()
        scope.add_element(code_generator.CppLine("auto headers = getGlobalHeaders();"))
        for header in headers:
            scope.add_element(code_generator.CppLine(f'headers["{header.key}"] = "{header.value}";'))
        scope.add_element(code_generator.CppLine("setGlobalHeaders(headers);"))
        return scope

    @staticmethod
    def create_global_parameters_scope(parameters) -> code_generator.CppScope:
        scope = code_generator.CppScope()
        scope.add_element(code_generator.CppLine("auto parameters = getGlobalParameters();"))
        for parameter in parameters:
            scope.add_element(code_generator.CppLine(f'parameters.addQueryItem("{parameter.key}", "{parameter.value}");'))
        scope.add_element(code_generator.CppLine("setGlobalParameters(parameters);"))
        return scope
