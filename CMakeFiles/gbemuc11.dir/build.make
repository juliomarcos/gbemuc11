# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.3

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.3.2/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.3.2/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/Users/juliorodrigues/Dropbox/Projetos Malucos/gb/gbemuc11"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/Users/juliorodrigues/Dropbox/Projetos Malucos/gb/gbemuc11"

# Include any dependencies generated for this target.
include CMakeFiles/gbemuc11.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/gbemuc11.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/gbemuc11.dir/flags.make

CMakeFiles/gbemuc11.dir/src/CPU.cpp.o: CMakeFiles/gbemuc11.dir/flags.make
CMakeFiles/gbemuc11.dir/src/CPU.cpp.o: src/CPU.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/juliorodrigues/Dropbox/Projetos Malucos/gb/gbemuc11/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/gbemuc11.dir/src/CPU.cpp.o"
	clang++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/gbemuc11.dir/src/CPU.cpp.o -c "/Users/juliorodrigues/Dropbox/Projetos Malucos/gb/gbemuc11/src/CPU.cpp"

CMakeFiles/gbemuc11.dir/src/CPU.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gbemuc11.dir/src/CPU.cpp.i"
	clang++  $(CXX_DEFINES) $(CXX_FLAGS) -E "/Users/juliorodrigues/Dropbox/Projetos Malucos/gb/gbemuc11/src/CPU.cpp" > CMakeFiles/gbemuc11.dir/src/CPU.cpp.i

CMakeFiles/gbemuc11.dir/src/CPU.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gbemuc11.dir/src/CPU.cpp.s"
	clang++  $(CXX_DEFINES) $(CXX_FLAGS) -S "/Users/juliorodrigues/Dropbox/Projetos Malucos/gb/gbemuc11/src/CPU.cpp" -o CMakeFiles/gbemuc11.dir/src/CPU.cpp.s

CMakeFiles/gbemuc11.dir/src/CPU.cpp.o.requires:

.PHONY : CMakeFiles/gbemuc11.dir/src/CPU.cpp.o.requires

CMakeFiles/gbemuc11.dir/src/CPU.cpp.o.provides: CMakeFiles/gbemuc11.dir/src/CPU.cpp.o.requires
	$(MAKE) -f CMakeFiles/gbemuc11.dir/build.make CMakeFiles/gbemuc11.dir/src/CPU.cpp.o.provides.build
.PHONY : CMakeFiles/gbemuc11.dir/src/CPU.cpp.o.provides

CMakeFiles/gbemuc11.dir/src/CPU.cpp.o.provides.build: CMakeFiles/gbemuc11.dir/src/CPU.cpp.o


CMakeFiles/gbemuc11.dir/src/GPU.cpp.o: CMakeFiles/gbemuc11.dir/flags.make
CMakeFiles/gbemuc11.dir/src/GPU.cpp.o: src/GPU.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/juliorodrigues/Dropbox/Projetos Malucos/gb/gbemuc11/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/gbemuc11.dir/src/GPU.cpp.o"
	clang++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/gbemuc11.dir/src/GPU.cpp.o -c "/Users/juliorodrigues/Dropbox/Projetos Malucos/gb/gbemuc11/src/GPU.cpp"

CMakeFiles/gbemuc11.dir/src/GPU.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gbemuc11.dir/src/GPU.cpp.i"
	clang++  $(CXX_DEFINES) $(CXX_FLAGS) -E "/Users/juliorodrigues/Dropbox/Projetos Malucos/gb/gbemuc11/src/GPU.cpp" > CMakeFiles/gbemuc11.dir/src/GPU.cpp.i

CMakeFiles/gbemuc11.dir/src/GPU.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gbemuc11.dir/src/GPU.cpp.s"
	clang++  $(CXX_DEFINES) $(CXX_FLAGS) -S "/Users/juliorodrigues/Dropbox/Projetos Malucos/gb/gbemuc11/src/GPU.cpp" -o CMakeFiles/gbemuc11.dir/src/GPU.cpp.s

CMakeFiles/gbemuc11.dir/src/GPU.cpp.o.requires:

.PHONY : CMakeFiles/gbemuc11.dir/src/GPU.cpp.o.requires

CMakeFiles/gbemuc11.dir/src/GPU.cpp.o.provides: CMakeFiles/gbemuc11.dir/src/GPU.cpp.o.requires
	$(MAKE) -f CMakeFiles/gbemuc11.dir/build.make CMakeFiles/gbemuc11.dir/src/GPU.cpp.o.provides.build
.PHONY : CMakeFiles/gbemuc11.dir/src/GPU.cpp.o.provides

CMakeFiles/gbemuc11.dir/src/GPU.cpp.o.provides.build: CMakeFiles/gbemuc11.dir/src/GPU.cpp.o


CMakeFiles/gbemuc11.dir/src/Main.cpp.o: CMakeFiles/gbemuc11.dir/flags.make
CMakeFiles/gbemuc11.dir/src/Main.cpp.o: src/Main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/juliorodrigues/Dropbox/Projetos Malucos/gb/gbemuc11/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/gbemuc11.dir/src/Main.cpp.o"
	clang++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/gbemuc11.dir/src/Main.cpp.o -c "/Users/juliorodrigues/Dropbox/Projetos Malucos/gb/gbemuc11/src/Main.cpp"

CMakeFiles/gbemuc11.dir/src/Main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gbemuc11.dir/src/Main.cpp.i"
	clang++  $(CXX_DEFINES) $(CXX_FLAGS) -E "/Users/juliorodrigues/Dropbox/Projetos Malucos/gb/gbemuc11/src/Main.cpp" > CMakeFiles/gbemuc11.dir/src/Main.cpp.i

CMakeFiles/gbemuc11.dir/src/Main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gbemuc11.dir/src/Main.cpp.s"
	clang++  $(CXX_DEFINES) $(CXX_FLAGS) -S "/Users/juliorodrigues/Dropbox/Projetos Malucos/gb/gbemuc11/src/Main.cpp" -o CMakeFiles/gbemuc11.dir/src/Main.cpp.s

CMakeFiles/gbemuc11.dir/src/Main.cpp.o.requires:

.PHONY : CMakeFiles/gbemuc11.dir/src/Main.cpp.o.requires

CMakeFiles/gbemuc11.dir/src/Main.cpp.o.provides: CMakeFiles/gbemuc11.dir/src/Main.cpp.o.requires
	$(MAKE) -f CMakeFiles/gbemuc11.dir/build.make CMakeFiles/gbemuc11.dir/src/Main.cpp.o.provides.build
.PHONY : CMakeFiles/gbemuc11.dir/src/Main.cpp.o.provides

CMakeFiles/gbemuc11.dir/src/Main.cpp.o.provides.build: CMakeFiles/gbemuc11.dir/src/Main.cpp.o


CMakeFiles/gbemuc11.dir/src/RomPath.cpp.o: CMakeFiles/gbemuc11.dir/flags.make
CMakeFiles/gbemuc11.dir/src/RomPath.cpp.o: src/RomPath.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/juliorodrigues/Dropbox/Projetos Malucos/gb/gbemuc11/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/gbemuc11.dir/src/RomPath.cpp.o"
	clang++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/gbemuc11.dir/src/RomPath.cpp.o -c "/Users/juliorodrigues/Dropbox/Projetos Malucos/gb/gbemuc11/src/RomPath.cpp"

CMakeFiles/gbemuc11.dir/src/RomPath.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gbemuc11.dir/src/RomPath.cpp.i"
	clang++  $(CXX_DEFINES) $(CXX_FLAGS) -E "/Users/juliorodrigues/Dropbox/Projetos Malucos/gb/gbemuc11/src/RomPath.cpp" > CMakeFiles/gbemuc11.dir/src/RomPath.cpp.i

CMakeFiles/gbemuc11.dir/src/RomPath.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gbemuc11.dir/src/RomPath.cpp.s"
	clang++  $(CXX_DEFINES) $(CXX_FLAGS) -S "/Users/juliorodrigues/Dropbox/Projetos Malucos/gb/gbemuc11/src/RomPath.cpp" -o CMakeFiles/gbemuc11.dir/src/RomPath.cpp.s

CMakeFiles/gbemuc11.dir/src/RomPath.cpp.o.requires:

.PHONY : CMakeFiles/gbemuc11.dir/src/RomPath.cpp.o.requires

CMakeFiles/gbemuc11.dir/src/RomPath.cpp.o.provides: CMakeFiles/gbemuc11.dir/src/RomPath.cpp.o.requires
	$(MAKE) -f CMakeFiles/gbemuc11.dir/build.make CMakeFiles/gbemuc11.dir/src/RomPath.cpp.o.provides.build
.PHONY : CMakeFiles/gbemuc11.dir/src/RomPath.cpp.o.provides

CMakeFiles/gbemuc11.dir/src/RomPath.cpp.o.provides.build: CMakeFiles/gbemuc11.dir/src/RomPath.cpp.o


CMakeFiles/gbemuc11.dir/src/StringUtilscpp.cpp.o: CMakeFiles/gbemuc11.dir/flags.make
CMakeFiles/gbemuc11.dir/src/StringUtilscpp.cpp.o: src/StringUtilscpp.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/juliorodrigues/Dropbox/Projetos Malucos/gb/gbemuc11/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/gbemuc11.dir/src/StringUtilscpp.cpp.o"
	clang++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/gbemuc11.dir/src/StringUtilscpp.cpp.o -c "/Users/juliorodrigues/Dropbox/Projetos Malucos/gb/gbemuc11/src/StringUtilscpp.cpp"

CMakeFiles/gbemuc11.dir/src/StringUtilscpp.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gbemuc11.dir/src/StringUtilscpp.cpp.i"
	clang++  $(CXX_DEFINES) $(CXX_FLAGS) -E "/Users/juliorodrigues/Dropbox/Projetos Malucos/gb/gbemuc11/src/StringUtilscpp.cpp" > CMakeFiles/gbemuc11.dir/src/StringUtilscpp.cpp.i

CMakeFiles/gbemuc11.dir/src/StringUtilscpp.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gbemuc11.dir/src/StringUtilscpp.cpp.s"
	clang++  $(CXX_DEFINES) $(CXX_FLAGS) -S "/Users/juliorodrigues/Dropbox/Projetos Malucos/gb/gbemuc11/src/StringUtilscpp.cpp" -o CMakeFiles/gbemuc11.dir/src/StringUtilscpp.cpp.s

CMakeFiles/gbemuc11.dir/src/StringUtilscpp.cpp.o.requires:

.PHONY : CMakeFiles/gbemuc11.dir/src/StringUtilscpp.cpp.o.requires

CMakeFiles/gbemuc11.dir/src/StringUtilscpp.cpp.o.provides: CMakeFiles/gbemuc11.dir/src/StringUtilscpp.cpp.o.requires
	$(MAKE) -f CMakeFiles/gbemuc11.dir/build.make CMakeFiles/gbemuc11.dir/src/StringUtilscpp.cpp.o.provides.build
.PHONY : CMakeFiles/gbemuc11.dir/src/StringUtilscpp.cpp.o.provides

CMakeFiles/gbemuc11.dir/src/StringUtilscpp.cpp.o.provides.build: CMakeFiles/gbemuc11.dir/src/StringUtilscpp.cpp.o


# Object files for target gbemuc11
gbemuc11_OBJECTS = \
"CMakeFiles/gbemuc11.dir/src/CPU.cpp.o" \
"CMakeFiles/gbemuc11.dir/src/GPU.cpp.o" \
"CMakeFiles/gbemuc11.dir/src/Main.cpp.o" \
"CMakeFiles/gbemuc11.dir/src/RomPath.cpp.o" \
"CMakeFiles/gbemuc11.dir/src/StringUtilscpp.cpp.o"

# External object files for target gbemuc11
gbemuc11_EXTERNAL_OBJECTS =

build/gbemuc11: CMakeFiles/gbemuc11.dir/src/CPU.cpp.o
build/gbemuc11: CMakeFiles/gbemuc11.dir/src/GPU.cpp.o
build/gbemuc11: CMakeFiles/gbemuc11.dir/src/Main.cpp.o
build/gbemuc11: CMakeFiles/gbemuc11.dir/src/RomPath.cpp.o
build/gbemuc11: CMakeFiles/gbemuc11.dir/src/StringUtilscpp.cpp.o
build/gbemuc11: CMakeFiles/gbemuc11.dir/build.make
build/gbemuc11: CMakeFiles/gbemuc11.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/Users/juliorodrigues/Dropbox/Projetos Malucos/gb/gbemuc11/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX executable build/gbemuc11"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/gbemuc11.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/gbemuc11.dir/build: build/gbemuc11

.PHONY : CMakeFiles/gbemuc11.dir/build

CMakeFiles/gbemuc11.dir/requires: CMakeFiles/gbemuc11.dir/src/CPU.cpp.o.requires
CMakeFiles/gbemuc11.dir/requires: CMakeFiles/gbemuc11.dir/src/GPU.cpp.o.requires
CMakeFiles/gbemuc11.dir/requires: CMakeFiles/gbemuc11.dir/src/Main.cpp.o.requires
CMakeFiles/gbemuc11.dir/requires: CMakeFiles/gbemuc11.dir/src/RomPath.cpp.o.requires
CMakeFiles/gbemuc11.dir/requires: CMakeFiles/gbemuc11.dir/src/StringUtilscpp.cpp.o.requires

.PHONY : CMakeFiles/gbemuc11.dir/requires

CMakeFiles/gbemuc11.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/gbemuc11.dir/cmake_clean.cmake
.PHONY : CMakeFiles/gbemuc11.dir/clean

CMakeFiles/gbemuc11.dir/depend:
	cd "/Users/juliorodrigues/Dropbox/Projetos Malucos/gb/gbemuc11" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/Users/juliorodrigues/Dropbox/Projetos Malucos/gb/gbemuc11" "/Users/juliorodrigues/Dropbox/Projetos Malucos/gb/gbemuc11" "/Users/juliorodrigues/Dropbox/Projetos Malucos/gb/gbemuc11" "/Users/juliorodrigues/Dropbox/Projetos Malucos/gb/gbemuc11" "/Users/juliorodrigues/Dropbox/Projetos Malucos/gb/gbemuc11/CMakeFiles/gbemuc11.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/gbemuc11.dir/depend

