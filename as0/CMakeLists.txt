cmake_minimum_required(VERSION 3.18)
project(as0 C CXX)

set(CMAKE_CXX_STANDARD 20)
add_subdirectory(raylib-cpp)

add_executable(as0 src/as0.cpp)
target_link_libraries(as0 PUBLIC raylib raylib_cpp)