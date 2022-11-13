find_path(PYBIND11_ROOT_DIR
    NAMES include/pybind11/pybind11.h
    PATHS ${PYBIND_PATH}
)

find_path(PYBIND11_INCLUDE_DIR
    NAMES pybind11/pybind11.h
    PATHS ${PYBIND11_ROOT_DIR}/include
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(pybind11 DEFAULT_MSG PYBIND11_INCLUDE_DIR)
