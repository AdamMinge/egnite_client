"""Module for schema data representation"""

from .api import Api
from .client import Client
from .model import Model

Schema = Api | Client | Model


__all__ = ["Api", "Client", "Model", "Schema"]
