#!/bin/bash

function cleanBuild {
    rm -rf build/
    cmake -S . -B build
    cmake --build build
}

function createDocs {
    rm -rf docs/html
    rm -rf docs/latex
    (cd docs; PROJECT_NUMBER="$(git rev-parse --short HEAD ; git diff-index --quiet HEAD || echo '(with uncommitted changes)')" doxygen;)
    (cd docs/latex; make refman.pdf)
    git add docs
    git commit -m "Regenerate docs"
}

function format {
    clang-format -i */*.cpp include/cppdescent/*.hpp
}

function runTests {
    for i in ./build/test/*.test
    do
        printf "\n\n"
        echo "=============================================="
        echo "$i"
        echo "=============================================="
        printf "\n\n"
        $i
    done
}

function valgrind {
    for i in ./build/test/*.test
    do
        printf "\n\n"
        echo "=============================================="
        echo "$i"
        echo "=============================================="
        printf "\n\n"
        valgrind $i
    done
}

function coverage {
    runTests
    rm -f ./test/coverage.info ./test/filtered_coverage.info
    rm -rf docs/lcov
    lcov -c -d ./build/test/CMakeFiles/ -o ./test/coverage.info
    lcov --remove ./test/coverage.info "/usr/*" "$(pwd)/extern/googletest/*" -o ./test/filtered_coverage.info
    genhtml test/filtered_coverage.info --output-directory ./docs/lcov/
}

if [ "$#" -eq 0 ]; then
    # cleanBuild
    format
    # runTests isn't needed because coverage runs them anyway
    coverage
    createDocs
fi

while [[ "$#" -gt 0 ]]; do
    case $1 in
        --clean-build)
            cleanBuild
            ;;
        --docs)
            createDocs
            ;;
        --format)
            format
            ;;
        --tests)
            runTests
            ;;
        --valgrind)
            valgrind
            ;;
        --coverage)
            coverage
            ;;
        *)
            echo "Unknown option: $1"
            ;;
    esac
    shift
done