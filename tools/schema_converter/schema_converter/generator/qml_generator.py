"""QML Generator"""

from io import TextIOWrapper
from pathlib import Path
from typing import Iterable
from code_generator import constructs, writer

from ..schema import client, api, model
from .generator import Generator


class QmlClientGenerator:
    def __init__(self,
                 client_schema: client.Client) -> None:
        self.client_schema = client_schema
    
    def write(self,
              header_stream: TextIOWrapper,
              src_stream: TextIOWrapper):
        pass


class QmlApiGenerator:
    def __init__(self,
                 api_schema: api.Api) -> None:
        self.api_schema = api_schema
    
    def write(self,
              header_stream: TextIOWrapper,
              src_stream: TextIOWrapper):
        pass


class QmlModelGenerator:
    def __init__(self,
                 model_schema: model.Model) -> None:
        self.model_schema = model_schema
    
    def write(self,
              header_stream: TextIOWrapper,
              src_stream: TextIOWrapper):
        pass

class QmlGenerator(Generator):
    def _generate_client(self,
                         client_schema: client.Client,
                         header_stream: TextIOWrapper,
                         src_stream: TextIOWrapper) -> None:
        model_generator = QmlClientGenerator(client_schema=client_schema)
        model_generator.write(header_stream=header_stream, 
                              src_stream=src_stream)
        

    def _generate_api(self, api_schema: api.Api,
                      header_stream: TextIOWrapper,
                      src_stream: TextIOWrapper) -> None:
        model_generator = QmlApiGenerator(api_schema=api_schema)
        model_generator.write(header_stream=header_stream, 
                              src_stream=src_stream)

    def _generate_model(self,
                        model_schema: model.Model,
                        header_stream: TextIOWrapper,
                        src_stream: TextIOWrapper) -> None:
        model_generator = QmlModelGenerator(model_schema=model_schema)
        model_generator.write(header_stream=header_stream, 
                              src_stream=src_stream)
