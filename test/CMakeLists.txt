include_directories(../3rdparty/gtest/include ../src/c ../src/cpp)
link_directories(${LIBRARY_OUTPUT_PATH})
add_executable(cilt_test test.cpp)
target_link_libraries(cilt_test gtest_main pthread cilt)

add_test(NAME cilt_test COMMAND cilt_test)
