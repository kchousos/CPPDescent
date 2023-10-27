add_executable(app app.cpp)
target_compile_features(app PRIVATE cxx_std_11)
target_compile_options(app PRIVATE -Wall -Wextra -Wpedantic -Werror)

# target_include_directories(app PUBLIC ../include)
target_link_libraries(app PRIVATE cppdescent)
