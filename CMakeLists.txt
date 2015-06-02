project(cilt)

cmake_minimum_required(VERSION 2.6)

find_package(Doxygen)
find_package(OpenMP)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

option(cilt_build_tests "Build all of cilt's tests." ON)
option(BUILD_SHARED_LIBS "Build shared libraries (DLLs)." ON)
option(BUILD_DOCUMENTATION "Create Doxygen documentation" ${DOXYGEN_FOUND})
option(WITH_OPENMP "Build with OpenMP support for parallelization" ${OPENMP_FOUND})

if(WITH_OPENMP)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    add_definitions(-DWITH_OPENMP)
endif()

add_subdirectory(src)

if(cilt_build_tests)
    enable_testing()
    add_subdirectory(3rdparty/gtest)
    add_subdirectory(test)
endif()

if(BUILD_DOCUMENTATION)
    if(NOT DOXYGEN_FOUND)
        message(FATAL_ERROR "Doxygen is needed to build the documentation.")
    endif()

    set(doxyfile_in ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
    set(doxyfile ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    configure_file(${doxyfile_in} ${doxyfile} @ONLY)

    add_custom_target(doc
        COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM)

    #install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html DESTINATION share/doc)
endif()
