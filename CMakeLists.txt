cmake_minimum_required(VERSION 3.1...3.27)

project(CPPDescent 
	VERSION 0.0.1
    DESCRIPTION "A KNN-Graph creation library"
	LANGUAGES CXX)

# Only do these if this is the main project, and not if it is included through add_subdirectory
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

  # Let's nicely support folders in IDEs
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)

endif()

# The compiled library code is here
add_subdirectory(src)
# The executable code is here
add_subdirectory(app)

# Testing only available if this is the main app
# Emergency override MODERN_CMAKE_BUILD_TESTING provided as well
if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR MODERN_CMAKE_BUILD_TESTING)
   AND BUILD_TESTING)
  add_subdirectory(test)
endif()