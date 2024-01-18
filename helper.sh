#!/bin/bash

function cleanBuild {
    rm -rf build/
    cmake -S . -B build
    cmake --build build
    mkdir -p ./build/cache
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
        if [ $? -ne 0 ] 
        then 
            exit 1
        fi
    done
}

function coverage {
    runTests
    rm -f ./test/coverage.info ./test/filtered_coverage.info
    rm -rf docs/lcov
    lcov -c -d ./build/test/CMakeFiles/ -o ./test/coverage.info --ignore-errors mismatch --ignore-errors gcov
    lcov --remove ./test/coverage.info "/usr/*" "$(pwd)/extern/googletest/*" "$(pwd)/test/unit/*" "$(pwd)/include/**" -o ./test/filtered_coverage.info
    genhtml test/filtered_coverage.info --output-directory ./docs/lcov/
}

function profile {
    gprof ./build/app/app | gprof2dot -s -w | dot -Tsvg -o ./docs/profile.svg
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
        --coverage)
            coverage
            ;;
        --profile)
            profile
            ;;
        --info)
            createDocs
            coverage
            profile
            ;;
        *)
            echo "Unknown option: $1"
            ;;
    esac
    shift
done
