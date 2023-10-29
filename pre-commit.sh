#!/bin/bash

# clean build
# rm -rf build/
cmake -S . -B build
cmake --build build

# generate documentation
rm -rf docs/html
rm -rf docs/latex
(cd docs; PROJECT_NUMBER="$(git rev-parse --short HEAD ; git diff-index --quiet HEAD || echo '(with uncommitted changes)')" doxygen;)
(cd docs/latex; make refman.pdf)

# format source code
clang-format -i */*.cpp include/cppdescent/*.hpp

# run tests
for i in ./build/test/*.test 
do 
    $i
done

# # generate code coverage
rm -f ./test/coverage.info ./test/filtered_coverage.info
rm -rf docs/lcov
lcov -c -d ./build/test/CMakeFiles/ -o ./test/coverage.info
lcov --remove ./test/coverage.info "/usr/*" "$(pwd)/extern/googletest/*" -o ./test/filtered_coverage.info
genhtml test/filtered_coverage.info --output-directory ./docs/lcov/