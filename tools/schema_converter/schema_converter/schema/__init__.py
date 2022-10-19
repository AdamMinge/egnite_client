"""Module for schema data representation"""

from .api import ApiSchema
from .client import ClientSchema
from .model import ModelSchema

Schema = ApiSchema | ClientSchema | ModelSchema


__all__ = [ApiSchema, ClientSchema, ModelSchema, Schema]
