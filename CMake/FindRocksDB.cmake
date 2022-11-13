find_path(ROCKSDB_ROOT_DIR
    NAMES rocksdb/db.h
)

find_library(ROCKSDB_LIB_PATH
    NAMES rocksdb
    HINTS ${ROCKSDB_ROOT_DIR}
)

find_path(ROCKSDB_INCLUDE_DIR
    NAMES rocksdb/db.h
    HINTS ${ROCKSDB_ROOT_DIR}/include
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(RocksDB DEFAULT_MSG
    ROCKSDB_LIB_PATH
    ROCKSDB_INCLUDE_DIR
)
