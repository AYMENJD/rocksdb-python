from .options import Options, ReadOptions, WriteOptions, FlushOptions
from typing import Dict, Union
from pathlib import Path
from logging import getLogger
from concurrent.futures import ThreadPoolExecutor
from .rocksdb_ext import RocksDBext, Response

import rocksdb, asyncio

logger = getLogger(__name__)


class NotSupported(Exception):
    pass


class RocksDB:
    """RocksDB client

    Args:
        db_path (``str``):
           RocksDB path.

        options (:class:`~rocksdb.Options`):
            RocksDB options.

        read_only (``bool``, optional):
            If `True` open database as [secondary instance](https://github.com/facebook/rocksdb/wiki/Read-only-and-Secondary-instances) of the specified `db_path`. This requires `secondary_path` to be specified. Defaults to False.

        secondary_path (``str``, optional):
            Secondary path for rocksdb `read_only` instance. `secondary_path` must be different from `db_path`. Implies `read_only`.

        loop (:py:class:`~asyncio.AbstractEventLoop`, optional):
            Event loop. Defaults to None (auto-detect).

        workers (``int``, optional):
            Number of workers for :py:class:`~concurrent.futures.ThreadPoolExecutor`. Defaults to 1.

    Raises:
        `TypeError`
        `ValueError`
        `RuntimeError`
    """

    def __init__(
        self,
        db_path: str,
        options: Options,
        read_only: bool = False,
        secondary_path: str = None,
        loop: asyncio.AbstractEventLoop = None,
        workers: int = 1,
    ) -> None:

        if not isinstance(db_path, str):
            raise TypeError("db_path must be str")
        elif not isinstance(options, Options):
            raise TypeError(f"Invalid class '{type(options).__name__}'")
        elif not isinstance(read_only, bool):
            raise TypeError("read_only must be boolean")
        elif not isinstance(workers, int):
            raise TypeError("workers must be int")
        elif workers < 1:
            raise ValueError("workers must be greater than 1")

        if isinstance(loop, asyncio.AbstractEventLoop):
            self.loop = loop
        else:
            self.loop = asyncio.get_event_loop()

        if read_only and not isinstance(secondary_path, str):
            raise TypeError("secondary_path must be str")
        elif isinstance(secondary_path, str):
            self.secondary_path = str(Path(secondary_path).absolute())
        else:
            self.secondary_path = ""

        self.db_path = str(Path(db_path).absolute())
        assert self.secondary_path != self.db_path
        self.options = options
        self.read_only = read_only
        self.workers = workers
        self.executer = ThreadPoolExecutor(workers)

        logger.info(rocksdb.getRocksBuildInfo(verbose=True))

        logger.info(f"Database path: {self.db_path}")
        if self.read_only:
            logger.info(
                f"Using read only instance with secondary path: {self.secondary_path}"
            )

        logger.debug(f"Options:  {str(self.options)}")

        self.__rocksdb = RocksDBext(
            self.db_path,
            self.options,
            self.read_only,
            self.secondary_path,
        )

        logger.info("Connected to rocksdb")

    async def __aenter__(self):
        return self

    async def __aexit__(self, exc_type, exc_val, exc_tb):
        try:
            await self.close()
        except Exception:
            pass

    @property
    def is_running(self) -> bool:
        return self.__rocksdb.is_running

    async def get(self, options: ReadOptions, key: str) -> Response:
        """Get the value of `key`

        Args:
            options (:class:`~rocksdb.ReadOptions`):
                RocksDB read options.

            key (``str``):
                The key.

        Raises:
            `TypeError`
           `RuntimeError`

        Returns:
            `Response`
        """

        if not isinstance(key, str):
            raise TypeError("key must be str")
        elif not isinstance(options, ReadOptions):
            raise TypeError(f"Invalid class '{type(options).__name__}'")

        future = self.loop.run_in_executor(
            self.executer, self.__rocksdb.Get, options, key
        )

        return await future

    async def put(self, options: WriteOptions, key: str, value: str) -> Response:
        """Set the database entry for `key` to `value`

        Args:
            options (:class:`~rocksdb.WriteOptions`):
                RocksDB read options.

            key (``str``):
                The key.

            value (``str``):
                The value of the `key`.

        Raises:
            `TypeError`
            `RuntimeError`

        Returns:
            `Response`
        """

        if not isinstance(key, str):
            raise TypeError("key must be str")
        elif not isinstance(value, str):
            raise TypeError("value must be str")
        elif not isinstance(options, WriteOptions):
            raise TypeError(f"Invalid class '{type(options).__name__}'")

        future = self.loop.run_in_executor(
            self.executer, self.__rocksdb.Put, options, key, value
        )

        return await future

    async def merge(self, options: WriteOptions, key: str, value: str) -> Response:
        """Merge the database entry for `key` with `value`

        Args:
            options (:class:`~rocksdb.WriteOptions`):
                RocksDB read options.

            key (``str``):
                The key.

            value (``str``):
                The value of the `key`.

        Raises:
            `TypeError`
            `RuntimeError`

        Returns:
            `Response`
        """

        if not isinstance(key, str):
            raise TypeError("key must be str")
        elif not isinstance(value, str):
            raise TypeError("value must be str")
        elif not isinstance(options, WriteOptions):
            raise TypeError(f"Invalid class '{type(options).__name__}'")

        future = self.loop.run_in_executor(
            self.executer, self.__rocksdb.Merge, options, key, value
        )

        return await future

    async def keyMayExist(self, options: ReadOptions, key: str) -> Response:
        """Check if `key` may exists

        Args:
            options (:class:`~rocksdb.ReadOptions`):
                RocksDB read options.

            key (``str``):
                The key.

        Raises:
            `TypeError`
            `RuntimeError`

        Returns:
            `Response`
        """

        if not isinstance(key, str):
            raise TypeError("key must be str")
        elif not isinstance(options, ReadOptions):
            raise TypeError(f"Invalid class '{type(options).__name__}'")

        future = self.loop.run_in_executor(
            self.executer, self.__rocksdb.KeyMayExist, options, key
        )

        return await future

    async def delete(self, options: WriteOptions, key: str) -> Response:
        """Remove the database entry (if any) for `key`

        Args:
            options (:class:`~rocksdb.WriteOptions`):
                RocksDB read options.

            key (``str``):
                The key.

        Raises:
            `TypeError`
            `RuntimeError`

        Returns:
            `Response`
        """

        if not isinstance(key, str):
            raise TypeError("key must be str")
        elif not isinstance(options, WriteOptions):
            raise TypeError(f"Invalid class '{type(options).__name__}'")

        future = self.loop.run_in_executor(
            self.executer, self.__rocksdb.Del, options, key
        )

        return await future

    async def getOptions(self) -> Response:
        """Get DB Options that we use

        Returns:
            `Response`
        """

        future = self.loop.run_in_executor(self.executer, self.__rocksdb.GetOptions)

        return await future

    async def setOptions(self, options: Dict[str, str]) -> Response:
        """Dynamically change options a running DB

        Args:
            options (Dict[`str`, `str`]):
                The Dictionary with option name and thier value.

        Raises:
            `TypeError`
            `RuntimeError`

        Returns:
            `Response`
        """

        if not isinstance(options, dict):
            raise TypeError("options must be dict")

        future = self.loop.run_in_executor(
            self.executer, self.__rocksdb.SetOptions, options
        )

        return await future

    async def setDBOptions(self, options: Dict[str, str]) -> Response:
        """Just like `setOptions` but for `DBoptions`

        Args:
            options (Dict[`str`, `str`]):
                The Dictionary with option name and thier value.

        Raises:
            `TypeError`
            `RuntimeError`

        Returns:
            `Response`
        """
        if not isinstance(options, dict):
            raise TypeError("options must be dict")

        future = self.loop.run_in_executor(
            self.executer, self.__rocksdb.SetDBOptions, options
        )

        return await future

    async def getProperty(self, property: str) -> Response:
        """Get property state

        Args:
            property (``str``):
                The property.

        Raises:
            `TypeError`
            `RuntimeError`

        Returns:
            `Response`
        """

        if not isinstance(property, str):
            raise TypeError("key must be str")

        future = self.loop.run_in_executor(
            self.executer, self.__rocksdb.GetProperty, property
        )

        return await future

    async def flush(self, options: FlushOptions) -> Response:
        """Flush all mem-table data

        Args:
            options (:class:`~rocksdb.FlushOptions`):
                RocksDB Flush options.

        Raises:
            `TypeError`
            `RuntimeError`

        Returns:
            `Response`
        """

        if not isinstance(options, FlushOptions):
            raise TypeError(f"Invalid class '{type(options).__name__}'")

        future = self.loop.run_in_executor(self.executer, self.__rocksdb.Flush, options)

        return await future

    async def tryCatchUpWithPrimary(self) -> Response:
        """Make the secondary instance catch up with the primary by tailing and replaying the MANIFEST and WAL of the primary

        Raises:
            :class:`~rocksdb.NotSupported`
            `RuntimeError`

        Returns:
            `Response`
        """

        if not self.read_only:
            raise NotSupported(
                "tryCatchUpWithPrimary is not supported for non-read_only instance"
            )

        future = self.loop.run_in_executor(
            self.executer, self.__rocksdb.TryCatchUpWithPrimary
        )

        return await future

    async def close(self) -> Response:
        """Close the DB by releasing resources, closing files etc

        Raises:
            `RuntimeError`

        Returns:
            `Response`
        """
        future = asyncio.Future()

        future = self.loop.run_in_executor(self.executer, self.__rocksdb.Close)

        return await future
