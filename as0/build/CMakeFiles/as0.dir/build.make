# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/mnt/c/Users/lucas/Desktop/Repo Clones/CS381-AS0/as0"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/mnt/c/Users/lucas/Desktop/Repo Clones/CS381-AS0/as0/build"

# Include any dependencies generated for this target.
include CMakeFiles/as0.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/as0.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/as0.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/as0.dir/flags.make

CMakeFiles/as0.dir/src/helloworld.cpp.o: CMakeFiles/as0.dir/flags.make
CMakeFiles/as0.dir/src/helloworld.cpp.o: /mnt/c/Users/lucas/Desktop/Repo\ Clones/CS381-AS0/as0/src/helloworld.cpp
CMakeFiles/as0.dir/src/helloworld.cpp.o: CMakeFiles/as0.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/mnt/c/Users/lucas/Desktop/Repo Clones/CS381-AS0/as0/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/as0.dir/src/helloworld.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/as0.dir/src/helloworld.cpp.o -MF CMakeFiles/as0.dir/src/helloworld.cpp.o.d -o CMakeFiles/as0.dir/src/helloworld.cpp.o -c "/mnt/c/Users/lucas/Desktop/Repo Clones/CS381-AS0/as0/src/helloworld.cpp"

CMakeFiles/as0.dir/src/helloworld.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/as0.dir/src/helloworld.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/mnt/c/Users/lucas/Desktop/Repo Clones/CS381-AS0/as0/src/helloworld.cpp" > CMakeFiles/as0.dir/src/helloworld.cpp.i

CMakeFiles/as0.dir/src/helloworld.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/as0.dir/src/helloworld.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/mnt/c/Users/lucas/Desktop/Repo Clones/CS381-AS0/as0/src/helloworld.cpp" -o CMakeFiles/as0.dir/src/helloworld.cpp.s

# Object files for target as0
as0_OBJECTS = \
"CMakeFiles/as0.dir/src/helloworld.cpp.o"

# External object files for target as0
as0_EXTERNAL_OBJECTS =

as0: CMakeFiles/as0.dir/src/helloworld.cpp.o
as0: CMakeFiles/as0.dir/build.make
as0: raylib-cpp/raylib/raylib/libraylib.a
as0: /usr/lib/x86_64-linux-gnu/libGL.so
as0: /usr/lib/x86_64-linux-gnu/librt.a
as0: /usr/lib/x86_64-linux-gnu/libm.so
as0: CMakeFiles/as0.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir="/mnt/c/Users/lucas/Desktop/Repo Clones/CS381-AS0/as0/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable as0"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/as0.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/as0.dir/build: as0
.PHONY : CMakeFiles/as0.dir/build

CMakeFiles/as0.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/as0.dir/cmake_clean.cmake
.PHONY : CMakeFiles/as0.dir/clean

CMakeFiles/as0.dir/depend:
	cd "/mnt/c/Users/lucas/Desktop/Repo Clones/CS381-AS0/as0/build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/mnt/c/Users/lucas/Desktop/Repo Clones/CS381-AS0/as0" "/mnt/c/Users/lucas/Desktop/Repo Clones/CS381-AS0/as0" "/mnt/c/Users/lucas/Desktop/Repo Clones/CS381-AS0/as0/build" "/mnt/c/Users/lucas/Desktop/Repo Clones/CS381-AS0/as0/build" "/mnt/c/Users/lucas/Desktop/Repo Clones/CS381-AS0/as0/build/CMakeFiles/as0.dir/DependInfo.cmake" "--color=$(COLOR)"
.PHONY : CMakeFiles/as0.dir/depend

