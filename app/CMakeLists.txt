add_executable(app app.cpp
                   ${PROJECT_SOURCE_DIR}/src/cppdescent.cpp
                   ${PROJECT_SOURCE_DIR}/src/ADTPQueue.cpp
                   ${PROJECT_SOURCE_DIR}/src/ADTGraph.cpp
                   ${PROJECT_SOURCE_DIR}/src/ADTMap.cpp
                   ${PROJECT_SOURCE_DIR}/src/ADTList.cpp
                   ${PROJECT_SOURCE_DIR}/src/ADTVector.cpp)

target_compile_features(app PRIVATE cxx_std_11)
# target_compile_options(app PRIVATE -Wall -Wextra -Wpedantic -Werror)
target_compile_options(app PRIVATE -Wall -Wextra -Wpedantic -pg)
target_link_options(app PRIVATE -pg)

# target_include_directories(app PUBLIC ../include)
target_link_libraries(app PRIVATE cppdescent)