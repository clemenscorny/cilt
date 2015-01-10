project(cilt)

cmake_minimum_required(VERSION 2.6)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

option(cilt_build_tests "Build all of cilt's tests." ON)
option(BUILD_SHARED_LIBS "Build shared libraries (DLLs)." ON)

add_subdirectory(src)

if(cilt_build_tests)
    enable_testing()
    add_subdirectory(3rdparty/gtest)
    add_subdirectory(test)
endif()
