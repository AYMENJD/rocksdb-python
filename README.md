# rocksdb-python [![version](https://img.shields.io/pypi/v/rocksdb-python?style=flat&logo=pypi)](https://pypi.org/project/rocksdb-python) [![downloads](https://img.shields.io/pypi/dm/rocksdb-python?style=flat)](https://pypistats.org/packages/rocksdb-python)

Simple and easy rocksdb client for python.

### Requirements

- python3.8+
- [rocksdb](https://github.com/facebook/rocksdb)

Installation
------------

We need to [install](https://github.com/facebook/rocksdb/blob/master/INSTALL.md) and build rocksdb alongside rocksdb-python.

Build rocksdb
-------------

From source:
***********
```bash
apt-get install build-essential libgflags-dev libsnappy-dev zlib1g-dev libbz2-dev liblz4-dev libzstd-dev
git clone https://github.com/facebook/rocksdb.git
cd rocksdb
mkdir build
cd build
make shared_lib
```
Choose where to install rocksdb after building
*******************************
- You can install rocksdb shared library in `/usr/lib/` (systemwide) using:
  ```bash
  make install-shared INSTALL_PATH=/usr
  ```
  - If you wish to uninstall it use:
    ```bash
    make uninstall INSTALL_PATH=/usr
    ```

- If you don't like to install it systemwide you can set the following environment variables which will help the compiler to find rocksdb:
  ```bash
    export CPLUS_INCLUDE_PATH=${CPLUS_INCLUDE_PATH}:`pwd`/../include
    export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:`pwd`
    export LIBRARY_PATH=${LIBRARY_PATH}:`pwd`
  ```

Install rocksdb-python
----------------------
Using pip:
```bash
pip install rocksdb-python
```

From source (development version):
```bash
pip install git+https://github.com/AYMENJD/rocksdb-python
```

Usage
-----
```python
import rocksdb, asyncio

async def main():
    db = rocksdb.RocksDB(db_path="myrocksdb/", options=rocksdb.Options(create_if_missing=True))

    key = "123-456-789"
    response = await db.put(rocksdb.WriteOptions(), key, "Hello world. Bye!")

    if response.status.ok: # You always need to check if the request success.
        get_value = await db.get(rocksdb.ReadOptions(), key)

        print(get_value.value) # Hello world. Bye!

        await db.delete(rocksdb.WriteOptions(), key)
    else:
        print(get_value.status.to_dict())


    await db.close()

asyncio.run(main())
```
Check [Documentation](https://github.com/AYMENJD/rocksdb-python/wiki) for more.

Contributing
------------
Pull requests are always welcome!!

License
-------

MIT [License](https://github.com/AYMENJD/rocksdb-python/blob/main/LICENSE)
