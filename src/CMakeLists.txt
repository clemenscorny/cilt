include_directories(c cpp)
add_library(cilt cpp/cilt.hpp c/cilt.cpp c/cilt.h cpp/Filter.cpp cpp/Filter.hpp cpp/ShiftReg.hpp)
