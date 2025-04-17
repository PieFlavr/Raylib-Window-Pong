cmake_minimum_required(VERSION 3.18)
project(as4 C CXX)

set(CMAKE_CXX_STANDARD 20)


add_subdirectory(../raylib_multi_window_experimental ${CMAKE_BINARY_DIR}/raylib_multi_window_experimental_build) 
add_subdirectory(../miniaudio ${CMAKE_BINARY_DIR}/miniaudio_build)

add_executable(as4 src/as4.cpp)

# Link the libraries
target_link_libraries(as4 PUBLIC raylib) 
target_link_libraries(as4 PUBLIC miniaudio)
# target_link_libraries(as4 PUBLIC raytracer m)
