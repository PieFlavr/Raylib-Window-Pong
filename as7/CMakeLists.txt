cmake_minimum_required(VERSION 3.18)
project(as7 C CXX)

set(CMAKE_CXX_STANDARD 20)
# Modified so I don't have to execute my precious drive space
add_subdirectory(../raylib_multi_window_experimental ${CMAKE_BINARY_DIR}/raylib_multi_window_experimental_build) 
add_subdirectory(../raylib-cpp/BufferedRaylib ${CMAKE_BINARY_DIR}/raylib-buffered_build)
# add_subdirectory(../raylib-cpp/raygui ${CMAKE_BINARY_DIR}/raygui_build)
add_subdirectory(../miniaudio ${CMAKE_BINARY_DIR}/miniaudio_build)

include(../assets/includeable.cmake)

# Include directories for raylib and raylib-cpp
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/src/baseCOs)
include_directories(${CMAKE_SOURCE_DIR}/src/derivativeCOs)
include_directories(${CMAKE_SOURCE_DIR}/src/entitySkeletons)

# Collect all source files
file(GLOB BASECO_SOURCES "${CMAKE_SOURCE_DIR}/src/baseCOs/*.cpp")
file(GLOB DERIVATEDCO_SOURCES "${CMAKE_SOURCE_DIR}/src/derivateCOs/*.cpp")
file(GLOB ENTITYSKELETONS_SOURCES "${CMAKE_SOURCE_DIR}/src/entitySkeletons/*.cpp")

# Add executable and link all necessary source files
add_executable(as7 
    src/as7.cpp
    #src/skybox.cpp
    ${BASECO_SOURCES}
    ${DERIVATEDCO_SOURCES}
    ${ENTITYSKELETONS_SOURCES}
)
target_link_libraries(as7 PUBLIC raylib raylib::buffered)

make_includeable(../assets/shaders/cubemap.fs generated/cubemap.fs)
make_includeable(../assets/shaders/cubemap.vs generated/cubemap.vs)
make_includeable(../assets/shaders/skybox.fs generated/skybox.fs)
make_includeable(../assets/shaders/skybox.vs generated/skybox.vs)

# For the sake of my own convenience, I am just gonna use the assets folder 
# instead of baking texture/models/etc. into the executable 