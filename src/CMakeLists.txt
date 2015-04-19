include_directories(c cpp)
add_library(cilt cpp/cilt.hpp c/cilt.cpp c/cilt.h cpp/Filter.cpp cpp/Filter.hpp cpp/ShiftReg.hpp)

if(CMAKE_COMPILER_IS_GNUCXX AND ENABLE_TESTING_COVERAGE)
    set_target_properties(cilt PROPERTIES COMPILE_FLAGS "-fprofile-arcs -ftest-coverage"
    LINK_FLAGS "--coverage")
endif()
