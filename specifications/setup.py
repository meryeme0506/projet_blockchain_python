import pybind11
from setuptools import setup, Extension

module = Extension("bip39", sources=["bip39.cpp"])

setup(
    ext_modules=[bip39],
)
