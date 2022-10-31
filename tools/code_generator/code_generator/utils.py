"""Common utils used by tool"""

from typing import Iterable, Any


def is_iterable(value: Any) -> bool:
    try:
        iter(value)
    except TypeError:
        return False
    else:
        return True


def listify(value: Iterable[Any] | Any | None) -> list[Any]:
    if value is None:
        return []
    elif not isinstance(value, str) and is_iterable(value):
        return list(value)
    return [value]
