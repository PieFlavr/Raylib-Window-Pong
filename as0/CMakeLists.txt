cmake_minimum_required(VERSION 3.18)
project(as1 C CXX)

set(CMAKE_CXX_STANDARD 20)
# Modified so I don't have to execute my precious drive space
add_subdirectory(../raylib-cpp ${CMAKE_BINARY_DIR}/raylib-cpp-build) 

add_executable(as1 src/as1.cpp)
target_link_libraries(as1 PUBLIC raylib raylib_cpp)