cmake_minimum_required(VERSION 3.0.2 FATAL_ERROR)

if(POLICY CMP0065)
  # do not export symbols from executables
  # affects compiler checks in project(), so must be set before it
  cmake_policy(SET CMP0065 NEW)
endif()

project(rocksdb-python VERSION 0.1.0 LANGUAGES CXX C)

if(NOT DEFINED CMAKE_MODULE_PATH)
  set(CMAKE_MODULE_PATH "")
endif()

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake" "${CMAKE_MODULE_PATH}")

if(POLICY CMP0054)
  # do not expand quoted arguments
  cmake_policy(SET CMP0054 NEW)
endif()

if(POLICY CMP0060)
  # link libraries by full path
  cmake_policy(SET CMP0060 NEW)
endif()

if(POLICY CMP0074)
  # use environment variables to find libraries
  cmake_policy(SET CMP0074 NEW)
endif()

include(PreventInSourceBuild)
prevent_in_source_build()

find_package(RocksDB REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

add_library(RocksDB SHARED IMPORTED)
set_target_properties(RocksDB PROPERTIES IMPORTED_LOCATION ${ROCKSDB_LIB_PATH})
set_target_properties(RocksDB PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${ROCKSDB_INCLUDE_DIR})

add_subdirectory(rocksdb/ext/pybind11)


pybind11_add_module(rocksdb_ext rocksdb/ext/extension.cpp)

target_link_libraries(rocksdb_ext PUBLIC RocksDB)


