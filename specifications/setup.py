import pybind11
from setuptools import setup, Extension

module = Extension("bip", sources=["bip.cpp"])

setup(
    ext_modules=[bip],
)
