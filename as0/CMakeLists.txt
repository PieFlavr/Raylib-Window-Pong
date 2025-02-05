cmake_minimum_required(VERSION 3.18)
project(as0 C CXX)

set(CMAKE_CXX_STANDARD 20)
# Modified so I don't have to execute my precious drive space
add_subdirectory(../raylib-cpp ${CMAKE_BINARY_DIR}/raylib-cpp-build) 

add_executable(as0 src/as0.cpp)
target_link_libraries(as0 PUBLIC raylib raylib_cpp)