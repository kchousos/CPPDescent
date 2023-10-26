#!/bin/bash

cmake -S . -B build
cmake --build build
(cd docs; doxygen;)
# (cd docs/latex; make refman.pdf)
clang-format -i */*.cpp include/cppdescent/*.hpp
./build/app/app