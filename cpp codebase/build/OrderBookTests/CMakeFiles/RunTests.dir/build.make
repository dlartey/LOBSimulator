# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.26

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
CMAKE_COMMAND = /opt/homebrew/Cellar/cmake/3.26.0/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/Cellar/cmake/3.26.0/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/Users/shreyashonnalli/Desktop/Group Project/GroupRepo/cpp codebase"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/Users/shreyashonnalli/Desktop/Group Project/GroupRepo/cpp codebase/build"

# Include any dependencies generated for this target.
include OrderBookTests/CMakeFiles/RunTests.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include OrderBookTests/CMakeFiles/RunTests.dir/compiler_depend.make

# Include the progress variables for this target.
include OrderBookTests/CMakeFiles/RunTests.dir/progress.make

# Include the compile flags for this target's objects.
include OrderBookTests/CMakeFiles/RunTests.dir/flags.make

OrderBookTests/CMakeFiles/RunTests.dir/RunTests_autogen/mocs_compilation.cpp.o: OrderBookTests/CMakeFiles/RunTests.dir/flags.make
OrderBookTests/CMakeFiles/RunTests.dir/RunTests_autogen/mocs_compilation.cpp.o: OrderBookTests/RunTests_autogen/mocs_compilation.cpp
OrderBookTests/CMakeFiles/RunTests.dir/RunTests_autogen/mocs_compilation.cpp.o: OrderBookTests/CMakeFiles/RunTests.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/shreyashonnalli/Desktop/Group Project/GroupRepo/cpp codebase/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object OrderBookTests/CMakeFiles/RunTests.dir/RunTests_autogen/mocs_compilation.cpp.o"
	cd "/Users/shreyashonnalli/Desktop/Group Project/GroupRepo/cpp codebase/build/OrderBookTests" && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT OrderBookTests/CMakeFiles/RunTests.dir/RunTests_autogen/mocs_compilation.cpp.o -MF CMakeFiles/RunTests.dir/RunTests_autogen/mocs_compilation.cpp.o.d -o CMakeFiles/RunTests.dir/RunTests_autogen/mocs_compilation.cpp.o -c "/Users/shreyashonnalli/Desktop/Group Project/GroupRepo/cpp codebase/build/OrderBookTests/RunTests_autogen/mocs_compilation.cpp"

OrderBookTests/CMakeFiles/RunTests.dir/RunTests_autogen/mocs_compilation.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/RunTests.dir/RunTests_autogen/mocs_compilation.cpp.i"
	cd "/Users/shreyashonnalli/Desktop/Group Project/GroupRepo/cpp codebase/build/OrderBookTests" && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/shreyashonnalli/Desktop/Group Project/GroupRepo/cpp codebase/build/OrderBookTests/RunTests_autogen/mocs_compilation.cpp" > CMakeFiles/RunTests.dir/RunTests_autogen/mocs_compilation.cpp.i

OrderBookTests/CMakeFiles/RunTests.dir/RunTests_autogen/mocs_compilation.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/RunTests.dir/RunTests_autogen/mocs_compilation.cpp.s"
	cd "/Users/shreyashonnalli/Desktop/Group Project/GroupRepo/cpp codebase/build/OrderBookTests" && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/shreyashonnalli/Desktop/Group Project/GroupRepo/cpp codebase/build/OrderBookTests/RunTests_autogen/mocs_compilation.cpp" -o CMakeFiles/RunTests.dir/RunTests_autogen/mocs_compilation.cpp.s

OrderBookTests/CMakeFiles/RunTests.dir/SomeTests.cpp.o: OrderBookTests/CMakeFiles/RunTests.dir/flags.make
OrderBookTests/CMakeFiles/RunTests.dir/SomeTests.cpp.o: /Users/shreyashonnalli/Desktop/Group\ Project/GroupRepo/cpp\ codebase/OrderBookTests/SomeTests.cpp
OrderBookTests/CMakeFiles/RunTests.dir/SomeTests.cpp.o: OrderBookTests/CMakeFiles/RunTests.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/shreyashonnalli/Desktop/Group Project/GroupRepo/cpp codebase/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object OrderBookTests/CMakeFiles/RunTests.dir/SomeTests.cpp.o"
	cd "/Users/shreyashonnalli/Desktop/Group Project/GroupRepo/cpp codebase/build/OrderBookTests" && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT OrderBookTests/CMakeFiles/RunTests.dir/SomeTests.cpp.o -MF CMakeFiles/RunTests.dir/SomeTests.cpp.o.d -o CMakeFiles/RunTests.dir/SomeTests.cpp.o -c "/Users/shreyashonnalli/Desktop/Group Project/GroupRepo/cpp codebase/OrderBookTests/SomeTests.cpp"

OrderBookTests/CMakeFiles/RunTests.dir/SomeTests.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/RunTests.dir/SomeTests.cpp.i"
	cd "/Users/shreyashonnalli/Desktop/Group Project/GroupRepo/cpp codebase/build/OrderBookTests" && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/shreyashonnalli/Desktop/Group Project/GroupRepo/cpp codebase/OrderBookTests/SomeTests.cpp" > CMakeFiles/RunTests.dir/SomeTests.cpp.i

OrderBookTests/CMakeFiles/RunTests.dir/SomeTests.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/RunTests.dir/SomeTests.cpp.s"
	cd "/Users/shreyashonnalli/Desktop/Group Project/GroupRepo/cpp codebase/build/OrderBookTests" && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/shreyashonnalli/Desktop/Group Project/GroupRepo/cpp codebase/OrderBookTests/SomeTests.cpp" -o CMakeFiles/RunTests.dir/SomeTests.cpp.s

# Object files for target RunTests
RunTests_OBJECTS = \
"CMakeFiles/RunTests.dir/RunTests_autogen/mocs_compilation.cpp.o" \
"CMakeFiles/RunTests.dir/SomeTests.cpp.o"

# External object files for target RunTests
RunTests_EXTERNAL_OBJECTS =

OrderBookTests/RunTests: OrderBookTests/CMakeFiles/RunTests.dir/RunTests_autogen/mocs_compilation.cpp.o
OrderBookTests/RunTests: OrderBookTests/CMakeFiles/RunTests.dir/SomeTests.cpp.o
OrderBookTests/RunTests: OrderBookTests/CMakeFiles/RunTests.dir/build.make
OrderBookTests/RunTests: lib/libgtest.a
OrderBookTests/RunTests: lib/libgtest_main.a
OrderBookTests/RunTests: OrderBook/libOrderBook.a
OrderBookTests/RunTests: DBHandler/libDBHandler.a
OrderBookTests/RunTests: /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX14.2.sdk/usr/lib/libsqlite3.tbd
OrderBookTests/RunTests: lib/libgtest.a
OrderBookTests/RunTests: /Users/shreyashonnalli/miniconda3/lib/libQt5Widgets.5.15.2.dylib
OrderBookTests/RunTests: /Users/shreyashonnalli/miniconda3/lib/libQt5Gui.5.15.2.dylib
OrderBookTests/RunTests: /Users/shreyashonnalli/miniconda3/lib/libQt5Core.5.15.2.dylib
OrderBookTests/RunTests: OrderBookTests/CMakeFiles/RunTests.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/Users/shreyashonnalli/Desktop/Group Project/GroupRepo/cpp codebase/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable RunTests"
	cd "/Users/shreyashonnalli/Desktop/Group Project/GroupRepo/cpp codebase/build/OrderBookTests" && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/RunTests.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
OrderBookTests/CMakeFiles/RunTests.dir/build: OrderBookTests/RunTests
.PHONY : OrderBookTests/CMakeFiles/RunTests.dir/build

OrderBookTests/CMakeFiles/RunTests.dir/clean:
	cd "/Users/shreyashonnalli/Desktop/Group Project/GroupRepo/cpp codebase/build/OrderBookTests" && $(CMAKE_COMMAND) -P CMakeFiles/RunTests.dir/cmake_clean.cmake
.PHONY : OrderBookTests/CMakeFiles/RunTests.dir/clean

OrderBookTests/CMakeFiles/RunTests.dir/depend:
	cd "/Users/shreyashonnalli/Desktop/Group Project/GroupRepo/cpp codebase/build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/Users/shreyashonnalli/Desktop/Group Project/GroupRepo/cpp codebase" "/Users/shreyashonnalli/Desktop/Group Project/GroupRepo/cpp codebase/OrderBookTests" "/Users/shreyashonnalli/Desktop/Group Project/GroupRepo/cpp codebase/build" "/Users/shreyashonnalli/Desktop/Group Project/GroupRepo/cpp codebase/build/OrderBookTests" "/Users/shreyashonnalli/Desktop/Group Project/GroupRepo/cpp codebase/build/OrderBookTests/CMakeFiles/RunTests.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : OrderBookTests/CMakeFiles/RunTests.dir/depend

