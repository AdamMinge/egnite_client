"""XML Schema reader"""

from pathlib import Path
from xml.etree import ElementTree

from ..schema import Schema, ClientSchema, ApiSchema, ModelSchema
from .schema_reader import SchemaReader


def get_element_attribute(element: ElementTree.Element,
                          key: str) -> str:
    if key not in element.attrib:
        raise IncorrectXmlSchema(
            f"parsing xml element: ({element.tag}) hasn't attribute: ({key})")
    return element.attrib[key]


def get_element_child_value(element: ElementTree.Element,
                            key: str) -> str:
    if (child := element.find(key)) == None:
        raise IncorrectXmlSchema(
            f"parsing xml element: ({element.tag}) hasn't child: ({key})")
    return getattr(child, "text", "")


class XmlClientSchemaReader:
    def read(self,
             client_tree: ElementTree.Element) -> ClientSchema:
        client = ClientSchema(
            name=get_element_attribute(client_tree, "name"),
            base_url=get_element_child_value(client_tree, "BaseUrl"),
            version=get_element_child_value(client_tree, "Version"),
        )

        for api_tree in client_tree.findall("Api"):
            client.apis.append(self._read_api(api_tree))

        return client

    def _read_api(self,
                  api_tree: ElementTree.Element) -> ClientSchema.Api:
        return ClientSchema.Api(
            name=get_element_attribute(api_tree, "name"),
            type=get_element_attribute(api_tree, "type"),
        )


class XmlApiSchemaReader:
    def read(self,
             api_tree: ElementTree.Element) -> ApiSchema:
        api = ApiSchema(
            name=get_element_attribute(api_tree, "name"),
            path=get_element_child_value(api_tree, "Path"),
        )

        for method_tree in api_tree.findall("Method"):
            api.methods.append(self._read_method(method_tree))

        return api

    def _read_method(self,
                     method_tree: ElementTree.Element) -> ApiSchema.Method:
        method = ApiSchema.Method(
            name=get_element_attribute(method_tree, "name"),
            verb=get_element_attribute(method_tree, "verb"),
            returns=get_element_attribute(method_tree, "returns"),
        )

        for parameter_tree in method_tree.findall("Parameter"):
            method.parameters.append(self._read_parameter(parameter_tree))

        return method

    def _read_parameter(self,
                        parameter_tree: ElementTree.Element) -> ApiSchema.Method.Parameter:
        return ApiSchema.Method.Parameter(
            key=get_element_attribute(parameter_tree, "key"),
            type=get_element_attribute(parameter_tree, "type"),
        )


class XmlModelSchemaReader:
    def read(self,
             model_tree: ElementTree.Element) -> ModelSchema:
        model = ModelSchema(
            name=get_element_attribute(model_tree, "name"),
        )

        for property_tree in model_tree.findall("Property"):
            model.properties.append(self._read_property(property_tree))

        return model

    def _read_property(self,
                       parameter_tree: ElementTree.Element) -> ModelSchema.Property:
        return ModelSchema.Property(
            name=get_element_attribute(parameter_tree, "name"),
            type=get_element_attribute(parameter_tree, "type"),
        )


class XmlSchemaReader(SchemaReader):
    def read(self,
             schema_file: Path) -> Schema:
        tree = ElementTree.parse(schema_file)
        root = tree.getroot()

        reader: XmlClientSchemaReader | XmlApiSchemaReader | XmlModelSchemaReader
        match root.tag:
            case "Client":
                reader = XmlClientSchemaReader()
            case "Api":
                reader = XmlApiSchemaReader()
            case "Model":
                reader = XmlModelSchemaReader()
            case _:
                raise IncorrectXmlSchema(
                    f"parsing xml client schema file has unsupported root element: ({root.tag})")

        return reader.read(root)


class IncorrectXmlSchema(Exception):
    pass
