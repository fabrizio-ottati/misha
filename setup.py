# Inspired by https://github.com/himbeles/ctypes-example.
from pathlib import Path
from distutils.command.build_ext import build_ext as build_ext_orig

from setuptools import Extension, setup


class CTypesExtension(Extension):
    pass


class build_ext(build_ext_orig):
    def build_extension(self, ext):
        self._ctypes = isinstance(ext, CTypesExtension)
        return super().build_extension(ext)

    def get_export_symbols(self, ext):
        if self._ctypes:
            return ext.export_symbols
        return super().get_export_symbols(ext)

    def get_ext_filename(self, ext_name):
        return super().get_ext_filename(ext_name)


# with open("README.md", "r") as file:
#     long_description = file.read()

my_extension = CTypesExtension(
    name = "clib-misha", 
    sources = [
        str(Path("misha", "cc", "src", "dat.cc")), 
    ], 
    include_dirs = [str(Path("misha", "cc", "include")),]
    )

setup(
    name="misha",
    install_requires=["numpy"],
    author="Fabrizio Ottati",
    author_email="mail@fabrizio-ottati.dev",
    maintainer="Fabrizio Ottati",
    maintainer_email="mail@fabrizio-ottati.dev, mail@lenzgregor.com",
    url="https://github.com/fabrizio-ottati/misha",
    version="0.0.0",
    classifiers = [
        "Programming Language :: Python :: 3",
        "Programming Language :: C++",
        "License :: OSI Approved :: GNU General Public License v2 (GPLv2)",
        "Operating System :: OS Independent",
    ],
    # long_description=long_description,
    # long_description_content_type="text/markdown",
    packages=[
        "misha",
    ],
    ext_modules=[
        my_extension,
    ],
    cmdclass={"build_ext": build_ext},
)
