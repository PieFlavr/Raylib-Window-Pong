cmake_minimum_required(VERSION 3.18)
project(as4 C CXX)

set(CMAKE_CXX_STANDARD 20)


add_subdirectory(../raylib_multi_window_experimental ${CMAKE_BINARY_DIR}/raylib_multi_window_experimental_build) 
add_subdirectory(../miniaudio ${CMAKE_BINARY_DIR}/miniaudio_build)

include(../assets/includeable.cmake)

add_executable(as4 src/as4.cpp)

# Link the libraries
target_link_libraries(as4 PUBLIC raylib) 
target_link_libraries(as4 PUBLIC miniaudio)
# target_link_libraries(as4 PUBLIC raytracer m)

make_includeable(../assets/shaders/cubemap.fs generated/cubemap.fs)
make_includeable(../assets/shaders/cubemap.vs generated/cubemap.vs)
make_includeable(../assets/shaders/skybox.fs generated/skybox.fs)
make_includeable(../assets/shaders/skybox.vs generated/skybox.vs)

configure_file("../assets/textures/skybox.png" textures/skybox.png COPYONLY)
# For the sake of my own convenience, I am just gonna use the assets folder 
# instead of baking texture/models/etc. into the executable 