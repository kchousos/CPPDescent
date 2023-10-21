cmake_minimum_required(VERSION 3.27)

project(CPPDescent VERSION 0.0.1
                   LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_DIR src)

set(TEST_DIR test)

set(SOURCES ${SOURCE_DIR}/main.cpp)

add_executable(cppdescent ${SOURCES})

## Google Test
enable_testing()
add_subdirectory(external/googletest)

add_executable(
  hello_test
  ${TEST_DIR}/unit/my_test.cpp
)

target_link_libraries(
  hello_test
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(hello_test)