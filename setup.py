from setuptools import Extension, setup, find_packages
from setuptools.command.build_ext import build_ext
from pathlib import Path
from re import findall
from sys import executable
import subprocess


class CMakeExtension(Extension):
    def __init__(self, name, sourcedir=""):
        Extension.__init__(
            self,
            name,
            sources=[],
        )
        self.sourcedir = str(Path(sourcedir).absolute())


class CMakeBuild(build_ext):
    def build_extension(self, ext):
        cwd = str(Path().absolute())

        build_temp = Path(self.build_temp)
        build_temp.mkdir(parents=True, exist_ok=True)
        extdir = Path(self.get_ext_fullpath(ext.name))
        extdir.mkdir(parents=True, exist_ok=True)

        cmake_args = [
            # "-DCMAKE_BUILD_TYPE=Debug",
            f"-DCMAKE_LIBRARY_OUTPUT_DIRECTORY={str(extdir.parent.absolute())}",
            f"-DPYTHON_EXECUTABLE={executable}",
        ]
        subprocess.check_call(["cmake", cwd] + cmake_args, cwd=str(build_temp))
        subprocess.check_call(["cmake", "--build", "."], cwd=str(build_temp))


with open("rocksdb/__init__.py", "r") as f:
    version = findall(r"__version__ = \"(.+)\"", f.read())[0]

with open("README.md", "r") as f:
    readme = f.read()


setup(
    name="Rocksdb-python",
    version=version,
    description="Simple and easy rocksdb client for python",
    long_description=readme,
    long_description_content_type="text/markdown",
    author="AYMEN Mohammed",
    author_email="let.me.code.safe@gmail.com",
    url="https://github.com/AYMENJD/rocksdb-python",
    license="MIT",
    python_requires=">=3.8",
    project_urls={
        "Source": "https://github.com/AYMENJD/rocksdb-python",
        "Tracker": "https://github.com/AYMENJD/rocksdb-python/issues",
        "Documantation": "https://github.com/AYMENJD/rocksdb-python/wiki",
    },
    packages=find_packages(),
    ext_modules=[CMakeExtension("rocksdb/")],
    cmdclass={"build_ext": CMakeBuild},
    keywords=["rocksdb", "database", "async", "python", "pybind11"],
)
