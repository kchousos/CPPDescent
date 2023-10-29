#!/bin/bash

cmake -S . -B build
cmake --build build
(cd docs; PROJECT_NUMBER="$(git rev-parse --short HEAD ; git diff-index --quiet HEAD || echo '(with uncommitted changes)')" doxygen;)
(cd docs/latex; make refman.pdf)
clang-format -i */*.cpp include/cppdescent/*.hpp
./build/app/app