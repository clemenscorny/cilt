include_directories(../3rdparty/gtest/include ../src/c ../src/cpp)
link_directories(${LIBRARY_OUTPUT_PATH})
add_executable(cilt_test test.cpp)

if(CMAKE_COMPILER_IS_GNUCXX AND ENABLE_TESTING_COVERAGE)
    include(CodeCoverage)
    setup_target_for_coverage(coverage cilt_test coverage)
    target_link_libraries(cilt_test gtest_main pthread cilt ${COVERAGE_LIBRARIES})
else()
    target_link_libraries(cilt_test gtest_main pthread cilt)
endif()

add_test(NAME cilt_test COMMAND cilt_test)
