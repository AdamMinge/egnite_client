"""Tools for reading data from xml schema file"""

import abc

from pathlib import Path
from xml.etree import ElementTree

from schema import Schema, Client, Api, Model, Method, Property


class SchemaReader(abc.ABC):
    def __init__(self) -> None:
        super().__init__()

    @abc.abstractmethod
    def read(self,
             schema_file: Path,
             schema: Schema) -> None:
        pass


class XmlSchemaReader(SchemaReader):
    def read(self,
             schema_file: Path,
             schema: Schema) -> None:
        tree = ElementTree.parse(schema_file)
        root = tree.getroot()

        match root.tag:
            case "Client":
                schema_client = self._read_client(root)
                schema.clients.append((schema_client, schema_file.stem))
            case "Api":
                schema_api = self._read_api(root)
                schema.apis.append((schema_api, schema_file.stem))
            case "Model":
                schema_model = self._read_model(root)
                schema.models.append((schema_model, schema_file.stem))
            case _:
                raise IncorrectXmlSchema(
                    f"parsing xml schema file has unsupported root element: ({root.tag})")

    @staticmethod
    def _get_element_attribute(element: ElementTree.Element,
                               key: str) -> str:
        if key not in element.attrib:
            raise IncorrectXmlSchema(
                f"parsing xml element: ({element.tag}) hasn't attribute: ({key})")
        return element.attrib[key]

    @staticmethod
    def _get_element_child_value(element: ElementTree.Element,
                                 key: str) -> str:
        if (child := element.find(key)) == None:
            raise IncorrectXmlSchema(
                f"parsing xml element: ({element.tag}) hasn't child: ({key})")
        return child.text

    def _read_client(self,
                     client_tree: ElementTree.Element) -> Client:
        return Client(
            name=self._get_element_attribute(client_tree, "name"),
            base_url=self._get_element_child_value(client_tree, "BaseUrl"),
            version=int(self._get_element_child_value(client_tree, "Version")),
        )

    def _read_property(self,
                       parameter_tree: ElementTree.Element) -> Property:
        return Property(
            key=self._get_element_attribute(parameter_tree, "key"),
            type=self._get_element_attribute(parameter_tree, "type"),
        )

    def _read_method(self,
                     method_tree: ElementTree.Element) -> Method:
        method = Method(
            name=self._get_element_attribute(method_tree, "name"),
            verb=self._get_element_attribute(method_tree, "verb"),
            returns=self._get_element_attribute(method_tree, "returns"),
        )

        for property_tree in method_tree.findall("Parameter"):
            method.parameters.append(self._read_property(property_tree))

        return method

    def _read_api(self,
                  api_tree: ElementTree.Element) -> Api:
        api = Api(
            name=self._get_element_attribute(api_tree, "name"),
            path=self._get_element_child_value(api_tree, "Path"),
        )

        for method_tree in api_tree.findall("Method"):
            api.methods.append(self._read_method(method_tree))

        return api

    def _read_model(self,
                    model_tree: ElementTree.Element) -> Model:
        model = Model(
            name=self._get_element_attribute(model_tree, "name"),
        )

        for property_tree in model_tree.findall("Property"):
            model.properties.append(self._read_property(property_tree))

        return model


class UnsupportedSchemaFile(Exception):
    pass


class IncorrectXmlSchema(Exception):
    pass


def read_schema(schema_files: list[Path]) -> Schema:
    file_to_reader = {
        ".xml": XmlSchemaReader()
    }

    schema = Schema()
    for schema_file in schema_files:
        reader = file_to_reader[schema_file.suffix]
        if not reader:
            raise UnsupportedSchemaFile(
                f"given schema file: ({schema_file}) has unsupported extension")
        reader.read(schema_file, schema)

    return schema
