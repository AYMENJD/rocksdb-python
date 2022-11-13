from .rocksdb_ext import _Options, _ReadOptions, _WriteOptions, _FlushOptions
from json import dumps


def set_kwargs(self, kwargs):
    for k, v in kwargs.items():
        try:
            setattr(self, k, v)
        except TypeError as e:
            raise TypeError(
                f"{k} must be of type '{type(getattr(self, k)).__name__}'"
            ) from e


class Options(_Options):
    def __init__(self, **kwargs) -> None:
        super().__init__()
        if kwargs:
            set_kwargs(self, kwargs)

    def __str__(self):
        return dumps(self.to_dict(), indent=4)


class ReadOptions(_ReadOptions):
    def __init__(self, **kwargs) -> None:
        super().__init__()
        if kwargs:
            set_kwargs(self, kwargs)


class WriteOptions(_WriteOptions):
    def __init__(self, **kwargs) -> None:
        super().__init__()
        if kwargs:
            set_kwargs(self, kwargs)


class FlushOptions(_FlushOptions):
    def __init__(self, **kwargs) -> None:
        super().__init__()
        if kwargs:
            set_kwargs(self, kwargs)
