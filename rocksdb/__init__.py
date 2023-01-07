__version__ = "0.1.2"
__copyright__ = "Copyright (c) 2022-present AYMEN Mohammed ~ https://github.com/AYMENJD"


VERSION = __version__

from typing import Union
from .options import Options, ReadOptions, WriteOptions, FlushOptions
from .client import RocksDB, NotSupported
from .rocksdb_ext import Response, RocksDBext as __RocksDBext


def getRocksVersion() -> str:
    """Returns the current version of RocksDB as a string

    Returns:
        `str`
    """
    return __RocksDBext.GetRocksVersionAsString()


def getRocksBuildInfo(
    name: str = "rocksdb-python",
    string: bool = True,
    verbose: bool = False,
) -> Union[str, dict]:
    """Returns a set of properties indicating how/when/where this version of RocksDB was created

    Args:
        name (``str``, optional):
            The name of the program. Defaults to "rocksdb-python".

        string (``bool``, optional):
            IF `True` returns build info as string. Otherwise returns `dict`. Defaults to True.

        verbose (``bool``, optional):
            If `True`, the full set of properties is printed. Implies `string`. Defaults to False.

    Returns:
        `str`
        `dict`
    """
    if string:
        return __RocksDBext.GetRocksBuildInfoAsString(name, verbose)
    else:
        return __RocksDBext.GetRocksBuildProperties()
