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
CMAKE_SOURCE_DIR = /home/lukas/git/benchmark_harness/ablation_studies/LULESH/LULESH_8

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/lukas/git/benchmark_harness/ablation_studies/LULESH/LULESH_8/build

# Include any dependencies generated for this target.
include CMakeFiles/lulesh2.0.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/lulesh2.0.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/lulesh2.0.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/lulesh2.0.dir/flags.make

CMakeFiles/lulesh2.0.dir/lulesh-comm.cc.o: CMakeFiles/lulesh2.0.dir/flags.make
CMakeFiles/lulesh2.0.dir/lulesh-comm.cc.o: /home/lukas/git/benchmark_harness/ablation_studies/LULESH/LULESH_8/lulesh-comm.cc
CMakeFiles/lulesh2.0.dir/lulesh-comm.cc.o: CMakeFiles/lulesh2.0.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/lukas/git/benchmark_harness/ablation_studies/LULESH/LULESH_8/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/lulesh2.0.dir/lulesh-comm.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/lulesh2.0.dir/lulesh-comm.cc.o -MF CMakeFiles/lulesh2.0.dir/lulesh-comm.cc.o.d -o CMakeFiles/lulesh2.0.dir/lulesh-comm.cc.o -c /home/lukas/git/benchmark_harness/ablation_studies/LULESH/LULESH_8/lulesh-comm.cc

CMakeFiles/lulesh2.0.dir/lulesh-comm.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/lulesh2.0.dir/lulesh-comm.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lukas/git/benchmark_harness/ablation_studies/LULESH/LULESH_8/lulesh-comm.cc > CMakeFiles/lulesh2.0.dir/lulesh-comm.cc.i

CMakeFiles/lulesh2.0.dir/lulesh-comm.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/lulesh2.0.dir/lulesh-comm.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lukas/git/benchmark_harness/ablation_studies/LULESH/LULESH_8/lulesh-comm.cc -o CMakeFiles/lulesh2.0.dir/lulesh-comm.cc.s

CMakeFiles/lulesh2.0.dir/lulesh-init.cc.o: CMakeFiles/lulesh2.0.dir/flags.make
CMakeFiles/lulesh2.0.dir/lulesh-init.cc.o: /home/lukas/git/benchmark_harness/ablation_studies/LULESH/LULESH_8/lulesh-init.cc
CMakeFiles/lulesh2.0.dir/lulesh-init.cc.o: CMakeFiles/lulesh2.0.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/lukas/git/benchmark_harness/ablation_studies/LULESH/LULESH_8/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/lulesh2.0.dir/lulesh-init.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/lulesh2.0.dir/lulesh-init.cc.o -MF CMakeFiles/lulesh2.0.dir/lulesh-init.cc.o.d -o CMakeFiles/lulesh2.0.dir/lulesh-init.cc.o -c /home/lukas/git/benchmark_harness/ablation_studies/LULESH/LULESH_8/lulesh-init.cc

CMakeFiles/lulesh2.0.dir/lulesh-init.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/lulesh2.0.dir/lulesh-init.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lukas/git/benchmark_harness/ablation_studies/LULESH/LULESH_8/lulesh-init.cc > CMakeFiles/lulesh2.0.dir/lulesh-init.cc.i

CMakeFiles/lulesh2.0.dir/lulesh-init.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/lulesh2.0.dir/lulesh-init.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lukas/git/benchmark_harness/ablation_studies/LULESH/LULESH_8/lulesh-init.cc -o CMakeFiles/lulesh2.0.dir/lulesh-init.cc.s

CMakeFiles/lulesh2.0.dir/lulesh-util.cc.o: CMakeFiles/lulesh2.0.dir/flags.make
CMakeFiles/lulesh2.0.dir/lulesh-util.cc.o: /home/lukas/git/benchmark_harness/ablation_studies/LULESH/LULESH_8/lulesh-util.cc
CMakeFiles/lulesh2.0.dir/lulesh-util.cc.o: CMakeFiles/lulesh2.0.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/lukas/git/benchmark_harness/ablation_studies/LULESH/LULESH_8/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/lulesh2.0.dir/lulesh-util.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/lulesh2.0.dir/lulesh-util.cc.o -MF CMakeFiles/lulesh2.0.dir/lulesh-util.cc.o.d -o CMakeFiles/lulesh2.0.dir/lulesh-util.cc.o -c /home/lukas/git/benchmark_harness/ablation_studies/LULESH/LULESH_8/lulesh-util.cc

CMakeFiles/lulesh2.0.dir/lulesh-util.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/lulesh2.0.dir/lulesh-util.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lukas/git/benchmark_harness/ablation_studies/LULESH/LULESH_8/lulesh-util.cc > CMakeFiles/lulesh2.0.dir/lulesh-util.cc.i

CMakeFiles/lulesh2.0.dir/lulesh-util.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/lulesh2.0.dir/lulesh-util.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lukas/git/benchmark_harness/ablation_studies/LULESH/LULESH_8/lulesh-util.cc -o CMakeFiles/lulesh2.0.dir/lulesh-util.cc.s

CMakeFiles/lulesh2.0.dir/lulesh-viz.cc.o: CMakeFiles/lulesh2.0.dir/flags.make
CMakeFiles/lulesh2.0.dir/lulesh-viz.cc.o: /home/lukas/git/benchmark_harness/ablation_studies/LULESH/LULESH_8/lulesh-viz.cc
CMakeFiles/lulesh2.0.dir/lulesh-viz.cc.o: CMakeFiles/lulesh2.0.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/lukas/git/benchmark_harness/ablation_studies/LULESH/LULESH_8/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/lulesh2.0.dir/lulesh-viz.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/lulesh2.0.dir/lulesh-viz.cc.o -MF CMakeFiles/lulesh2.0.dir/lulesh-viz.cc.o.d -o CMakeFiles/lulesh2.0.dir/lulesh-viz.cc.o -c /home/lukas/git/benchmark_harness/ablation_studies/LULESH/LULESH_8/lulesh-viz.cc

CMakeFiles/lulesh2.0.dir/lulesh-viz.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/lulesh2.0.dir/lulesh-viz.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lukas/git/benchmark_harness/ablation_studies/LULESH/LULESH_8/lulesh-viz.cc > CMakeFiles/lulesh2.0.dir/lulesh-viz.cc.i

CMakeFiles/lulesh2.0.dir/lulesh-viz.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/lulesh2.0.dir/lulesh-viz.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lukas/git/benchmark_harness/ablation_studies/LULESH/LULESH_8/lulesh-viz.cc -o CMakeFiles/lulesh2.0.dir/lulesh-viz.cc.s

CMakeFiles/lulesh2.0.dir/lulesh.cc.o: CMakeFiles/lulesh2.0.dir/flags.make
CMakeFiles/lulesh2.0.dir/lulesh.cc.o: /home/lukas/git/benchmark_harness/ablation_studies/LULESH/LULESH_8/lulesh.cc
CMakeFiles/lulesh2.0.dir/lulesh.cc.o: CMakeFiles/lulesh2.0.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/lukas/git/benchmark_harness/ablation_studies/LULESH/LULESH_8/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/lulesh2.0.dir/lulesh.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/lulesh2.0.dir/lulesh.cc.o -MF CMakeFiles/lulesh2.0.dir/lulesh.cc.o.d -o CMakeFiles/lulesh2.0.dir/lulesh.cc.o -c /home/lukas/git/benchmark_harness/ablation_studies/LULESH/LULESH_8/lulesh.cc

CMakeFiles/lulesh2.0.dir/lulesh.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/lulesh2.0.dir/lulesh.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lukas/git/benchmark_harness/ablation_studies/LULESH/LULESH_8/lulesh.cc > CMakeFiles/lulesh2.0.dir/lulesh.cc.i

CMakeFiles/lulesh2.0.dir/lulesh.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/lulesh2.0.dir/lulesh.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lukas/git/benchmark_harness/ablation_studies/LULESH/LULESH_8/lulesh.cc -o CMakeFiles/lulesh2.0.dir/lulesh.cc.s

# Object files for target lulesh2.0
lulesh2_0_OBJECTS = \
"CMakeFiles/lulesh2.0.dir/lulesh-comm.cc.o" \
"CMakeFiles/lulesh2.0.dir/lulesh-init.cc.o" \
"CMakeFiles/lulesh2.0.dir/lulesh-util.cc.o" \
"CMakeFiles/lulesh2.0.dir/lulesh-viz.cc.o" \
"CMakeFiles/lulesh2.0.dir/lulesh.cc.o"

# External object files for target lulesh2.0
lulesh2_0_EXTERNAL_OBJECTS =

lulesh2.0: CMakeFiles/lulesh2.0.dir/lulesh-comm.cc.o
lulesh2.0: CMakeFiles/lulesh2.0.dir/lulesh-init.cc.o
lulesh2.0: CMakeFiles/lulesh2.0.dir/lulesh-util.cc.o
lulesh2.0: CMakeFiles/lulesh2.0.dir/lulesh-viz.cc.o
lulesh2.0: CMakeFiles/lulesh2.0.dir/lulesh.cc.o
lulesh2.0: CMakeFiles/lulesh2.0.dir/build.make
lulesh2.0: CMakeFiles/lulesh2.0.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/lukas/git/benchmark_harness/ablation_studies/LULESH/LULESH_8/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX executable lulesh2.0"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/lulesh2.0.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/lulesh2.0.dir/build: lulesh2.0
.PHONY : CMakeFiles/lulesh2.0.dir/build

CMakeFiles/lulesh2.0.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/lulesh2.0.dir/cmake_clean.cmake
.PHONY : CMakeFiles/lulesh2.0.dir/clean

CMakeFiles/lulesh2.0.dir/depend:
	cd /home/lukas/git/benchmark_harness/ablation_studies/LULESH/LULESH_8/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/lukas/git/benchmark_harness/ablation_studies/LULESH/LULESH_8 /home/lukas/git/benchmark_harness/ablation_studies/LULESH/LULESH_8 /home/lukas/git/benchmark_harness/ablation_studies/LULESH/LULESH_8/build /home/lukas/git/benchmark_harness/ablation_studies/LULESH/LULESH_8/build /home/lukas/git/benchmark_harness/ablation_studies/LULESH/LULESH_8/build/CMakeFiles/lulesh2.0.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/lulesh2.0.dir/depend

