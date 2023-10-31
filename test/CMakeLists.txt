add_subdirectory("${PROJECT_SOURCE_DIR}/extern/googletest" "extern/googletest")

# The next line is optional, but keeps your CACHE cleaner:
mark_as_advanced(
    BUILD_GMOCK BUILD_GTEST BUILD_SHARED_LIBS
    gmock_build_tests gtest_build_samples gtest_build_tests
    gtest_disable_pthreads gtest_force_shared_crt gtest_hide_internal_symbols
)

# for IDEs
set_target_properties(gtest PROPERTIES FOLDER extern)
set_target_properties(gtest_main PROPERTIES FOLDER extern)
set_target_properties(gmock PROPERTIES FOLDER extern)
set_target_properties(gmock_main PROPERTIES FOLDER extern)

macro(package_add_test TESTNAME)
    # create an executable in which the tests will be stored
    add_executable(${TESTNAME} ${ARGN})
    # link the Google test infrastructure, mocking library, and a default main function to
    # the test executable.  Remove gtest_main if writing your own main function.
    target_link_libraries(${TESTNAME} gtest gmock gtest_main cppdescent)
    # gtest_discover_tests replaces gtest_add_tests,
    # see https://cmake.org/cmake/help/v3.10/module/GoogleTest.html for more options to pass to it
    gtest_discover_tests(${TESTNAME}
        # set a working directory so your project root so that you can find test data via paths relative to the project root
        WORKING_DIRECTORY ${PROJECT_DIR}
        PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_DIR}"
    )
    set_target_properties(${TESTNAME} PROPERTIES FOLDER test)
    target_compile_features(${TESTNAME} PRIVATE cxx_std_11)
    target_compile_options(${TESTNAME} PRIVATE -Wall -Wextra -Wpedantic -Werror
                                               -fprofile-arcs -ftest-coverage)
    target_link_options(${TESTNAME} PRIVATE -fprofile-arcs)
endmacro()

# Declare tests
package_add_test(ADTList.test 
                 unit/ADTList.test.cpp
                 ${PROJECT_SOURCE_DIR}/src/ADTList.cpp
                 ${PROJECT_SOURCE_DIR}/src/listNode.cpp)

package_add_test(ADTVector.test 
                 unit/ADTVector.test.cpp
                 ${PROJECT_SOURCE_DIR}/src/ADTVector.cpp)

package_add_test(ADTPQueue.test 
                 unit/ADTPQueue.test.cpp
                 ${PROJECT_SOURCE_DIR}/src/ADTPQueue.cpp
                 ${PROJECT_SOURCE_DIR}/src/ADTVector.cpp)

package_add_test(ADTMap.test 
                 unit/ADTMap.test.cpp
                 ${PROJECT_SOURCE_DIR}/src/ADTMap.cpp
                 ${PROJECT_SOURCE_DIR}/src/ADTMap.cpp)