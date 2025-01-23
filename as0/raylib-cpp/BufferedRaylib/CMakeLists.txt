cmake_minimum_required(VERSION 3.12)
project(BufferedRaylib CXX C)

set(CXX_STANDARD 20 REQUIRED)

# raylib
if(TARGET raylib)
    message("Raylib Found! No need to fetch!")
else()
    find_package(raylib QUIET)
    if (NOT raylib_FOUND)
        include(FetchContent)
        FetchContent_Declare(
            raylib
            GIT_REPOSITORY https://github.com/raysan5/raylib.git
            GIT_TAG 4.5.0
        )
        FetchContent_MakeAvailable(raylib)
    endif()
endif()

set(BUILD_TESTING false)
add_subdirectory(FastSignals)

add_library(buffered-raylib src/BufferedRaylib.hpp src/BufferedRaylib.cpp)
target_include_directories(buffered-raylib PUBLIC src)
target_link_libraries(buffered-raylib PUBLIC raylib libfastsignals)

add_library(raylib::buffered ALIAS buffered-raylib)

add_executable(tst "examples/test.cpp")
target_link_libraries(tst raylib::buffered)