# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ahmad/Desktop/Sarf

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ahmad/Desktop/Sarf/Build

# Include any dependencies generated for this target.
include src/sql/CMakeFiles/sql.dir/depend.make

# Include the progress variables for this target.
include src/sql/CMakeFiles/sql.dir/progress.make

# Include the compile flags for this target's objects.
include src/sql/CMakeFiles/sql.dir/flags.make

src/sql/CMakeFiles/sql.dir/Search_Compatibility.cpp.o: src/sql/CMakeFiles/sql.dir/flags.make
src/sql/CMakeFiles/sql.dir/Search_Compatibility.cpp.o: ../src/sql/Search_Compatibility.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/ahmad/Desktop/Sarf/Build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src/sql/CMakeFiles/sql.dir/Search_Compatibility.cpp.o"
	cd /home/ahmad/Desktop/Sarf/Build/src/sql && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/sql.dir/Search_Compatibility.cpp.o -c /home/ahmad/Desktop/Sarf/src/sql/Search_Compatibility.cpp

src/sql/CMakeFiles/sql.dir/Search_Compatibility.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sql.dir/Search_Compatibility.cpp.i"
	cd /home/ahmad/Desktop/Sarf/Build/src/sql && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/ahmad/Desktop/Sarf/src/sql/Search_Compatibility.cpp > CMakeFiles/sql.dir/Search_Compatibility.cpp.i

src/sql/CMakeFiles/sql.dir/Search_Compatibility.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sql.dir/Search_Compatibility.cpp.s"
	cd /home/ahmad/Desktop/Sarf/Build/src/sql && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/ahmad/Desktop/Sarf/src/sql/Search_Compatibility.cpp -o CMakeFiles/sql.dir/Search_Compatibility.cpp.s

src/sql/CMakeFiles/sql.dir/Search_Compatibility.cpp.o.requires:
.PHONY : src/sql/CMakeFiles/sql.dir/Search_Compatibility.cpp.o.requires

src/sql/CMakeFiles/sql.dir/Search_Compatibility.cpp.o.provides: src/sql/CMakeFiles/sql.dir/Search_Compatibility.cpp.o.requires
	$(MAKE) -f src/sql/CMakeFiles/sql.dir/build.make src/sql/CMakeFiles/sql.dir/Search_Compatibility.cpp.o.provides.build
.PHONY : src/sql/CMakeFiles/sql.dir/Search_Compatibility.cpp.o.provides

src/sql/CMakeFiles/sql.dir/Search_Compatibility.cpp.o.provides.build: src/sql/CMakeFiles/sql.dir/Search_Compatibility.cpp.o

src/sql/CMakeFiles/sql.dir/Search_by_category.cpp.o: src/sql/CMakeFiles/sql.dir/flags.make
src/sql/CMakeFiles/sql.dir/Search_by_category.cpp.o: ../src/sql/Search_by_category.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/ahmad/Desktop/Sarf/Build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src/sql/CMakeFiles/sql.dir/Search_by_category.cpp.o"
	cd /home/ahmad/Desktop/Sarf/Build/src/sql && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/sql.dir/Search_by_category.cpp.o -c /home/ahmad/Desktop/Sarf/src/sql/Search_by_category.cpp

src/sql/CMakeFiles/sql.dir/Search_by_category.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sql.dir/Search_by_category.cpp.i"
	cd /home/ahmad/Desktop/Sarf/Build/src/sql && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/ahmad/Desktop/Sarf/src/sql/Search_by_category.cpp > CMakeFiles/sql.dir/Search_by_category.cpp.i

src/sql/CMakeFiles/sql.dir/Search_by_category.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sql.dir/Search_by_category.cpp.s"
	cd /home/ahmad/Desktop/Sarf/Build/src/sql && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/ahmad/Desktop/Sarf/src/sql/Search_by_category.cpp -o CMakeFiles/sql.dir/Search_by_category.cpp.s

src/sql/CMakeFiles/sql.dir/Search_by_category.cpp.o.requires:
.PHONY : src/sql/CMakeFiles/sql.dir/Search_by_category.cpp.o.requires

src/sql/CMakeFiles/sql.dir/Search_by_category.cpp.o.provides: src/sql/CMakeFiles/sql.dir/Search_by_category.cpp.o.requires
	$(MAKE) -f src/sql/CMakeFiles/sql.dir/build.make src/sql/CMakeFiles/sql.dir/Search_by_category.cpp.o.provides.build
.PHONY : src/sql/CMakeFiles/sql.dir/Search_by_category.cpp.o.provides

src/sql/CMakeFiles/sql.dir/Search_by_category.cpp.o.provides.build: src/sql/CMakeFiles/sql.dir/Search_by_category.cpp.o

src/sql/CMakeFiles/sql.dir/sql_queries.cpp.o: src/sql/CMakeFiles/sql.dir/flags.make
src/sql/CMakeFiles/sql.dir/sql_queries.cpp.o: ../src/sql/sql_queries.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/ahmad/Desktop/Sarf/Build/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src/sql/CMakeFiles/sql.dir/sql_queries.cpp.o"
	cd /home/ahmad/Desktop/Sarf/Build/src/sql && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/sql.dir/sql_queries.cpp.o -c /home/ahmad/Desktop/Sarf/src/sql/sql_queries.cpp

src/sql/CMakeFiles/sql.dir/sql_queries.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sql.dir/sql_queries.cpp.i"
	cd /home/ahmad/Desktop/Sarf/Build/src/sql && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/ahmad/Desktop/Sarf/src/sql/sql_queries.cpp > CMakeFiles/sql.dir/sql_queries.cpp.i

src/sql/CMakeFiles/sql.dir/sql_queries.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sql.dir/sql_queries.cpp.s"
	cd /home/ahmad/Desktop/Sarf/Build/src/sql && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/ahmad/Desktop/Sarf/src/sql/sql_queries.cpp -o CMakeFiles/sql.dir/sql_queries.cpp.s

src/sql/CMakeFiles/sql.dir/sql_queries.cpp.o.requires:
.PHONY : src/sql/CMakeFiles/sql.dir/sql_queries.cpp.o.requires

src/sql/CMakeFiles/sql.dir/sql_queries.cpp.o.provides: src/sql/CMakeFiles/sql.dir/sql_queries.cpp.o.requires
	$(MAKE) -f src/sql/CMakeFiles/sql.dir/build.make src/sql/CMakeFiles/sql.dir/sql_queries.cpp.o.provides.build
.PHONY : src/sql/CMakeFiles/sql.dir/sql_queries.cpp.o.provides

src/sql/CMakeFiles/sql.dir/sql_queries.cpp.o.provides.build: src/sql/CMakeFiles/sql.dir/sql_queries.cpp.o

src/sql/CMakeFiles/sql.dir/Retrieve_Template.cpp.o: src/sql/CMakeFiles/sql.dir/flags.make
src/sql/CMakeFiles/sql.dir/Retrieve_Template.cpp.o: ../src/sql/Retrieve_Template.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/ahmad/Desktop/Sarf/Build/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src/sql/CMakeFiles/sql.dir/Retrieve_Template.cpp.o"
	cd /home/ahmad/Desktop/Sarf/Build/src/sql && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/sql.dir/Retrieve_Template.cpp.o -c /home/ahmad/Desktop/Sarf/src/sql/Retrieve_Template.cpp

src/sql/CMakeFiles/sql.dir/Retrieve_Template.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sql.dir/Retrieve_Template.cpp.i"
	cd /home/ahmad/Desktop/Sarf/Build/src/sql && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/ahmad/Desktop/Sarf/src/sql/Retrieve_Template.cpp > CMakeFiles/sql.dir/Retrieve_Template.cpp.i

src/sql/CMakeFiles/sql.dir/Retrieve_Template.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sql.dir/Retrieve_Template.cpp.s"
	cd /home/ahmad/Desktop/Sarf/Build/src/sql && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/ahmad/Desktop/Sarf/src/sql/Retrieve_Template.cpp -o CMakeFiles/sql.dir/Retrieve_Template.cpp.s

src/sql/CMakeFiles/sql.dir/Retrieve_Template.cpp.o.requires:
.PHONY : src/sql/CMakeFiles/sql.dir/Retrieve_Template.cpp.o.requires

src/sql/CMakeFiles/sql.dir/Retrieve_Template.cpp.o.provides: src/sql/CMakeFiles/sql.dir/Retrieve_Template.cpp.o.requires
	$(MAKE) -f src/sql/CMakeFiles/sql.dir/build.make src/sql/CMakeFiles/sql.dir/Retrieve_Template.cpp.o.provides.build
.PHONY : src/sql/CMakeFiles/sql.dir/Retrieve_Template.cpp.o.provides

src/sql/CMakeFiles/sql.dir/Retrieve_Template.cpp.o.provides.build: src/sql/CMakeFiles/sql.dir/Retrieve_Template.cpp.o

src/sql/CMakeFiles/sql.dir/Search_by_item.cpp.o: src/sql/CMakeFiles/sql.dir/flags.make
src/sql/CMakeFiles/sql.dir/Search_by_item.cpp.o: ../src/sql/Search_by_item.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/ahmad/Desktop/Sarf/Build/CMakeFiles $(CMAKE_PROGRESS_5)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src/sql/CMakeFiles/sql.dir/Search_by_item.cpp.o"
	cd /home/ahmad/Desktop/Sarf/Build/src/sql && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/sql.dir/Search_by_item.cpp.o -c /home/ahmad/Desktop/Sarf/src/sql/Search_by_item.cpp

src/sql/CMakeFiles/sql.dir/Search_by_item.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sql.dir/Search_by_item.cpp.i"
	cd /home/ahmad/Desktop/Sarf/Build/src/sql && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/ahmad/Desktop/Sarf/src/sql/Search_by_item.cpp > CMakeFiles/sql.dir/Search_by_item.cpp.i

src/sql/CMakeFiles/sql.dir/Search_by_item.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sql.dir/Search_by_item.cpp.s"
	cd /home/ahmad/Desktop/Sarf/Build/src/sql && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/ahmad/Desktop/Sarf/src/sql/Search_by_item.cpp -o CMakeFiles/sql.dir/Search_by_item.cpp.s

src/sql/CMakeFiles/sql.dir/Search_by_item.cpp.o.requires:
.PHONY : src/sql/CMakeFiles/sql.dir/Search_by_item.cpp.o.requires

src/sql/CMakeFiles/sql.dir/Search_by_item.cpp.o.provides: src/sql/CMakeFiles/sql.dir/Search_by_item.cpp.o.requires
	$(MAKE) -f src/sql/CMakeFiles/sql.dir/build.make src/sql/CMakeFiles/sql.dir/Search_by_item.cpp.o.provides.build
.PHONY : src/sql/CMakeFiles/sql.dir/Search_by_item.cpp.o.provides

src/sql/CMakeFiles/sql.dir/Search_by_item.cpp.o.provides.build: src/sql/CMakeFiles/sql.dir/Search_by_item.cpp.o

# Object files for target sql
sql_OBJECTS = \
"CMakeFiles/sql.dir/Search_Compatibility.cpp.o" \
"CMakeFiles/sql.dir/Search_by_category.cpp.o" \
"CMakeFiles/sql.dir/sql_queries.cpp.o" \
"CMakeFiles/sql.dir/Retrieve_Template.cpp.o" \
"CMakeFiles/sql.dir/Search_by_item.cpp.o"

# External object files for target sql
sql_EXTERNAL_OBJECTS =

src/sql/libsql.a: src/sql/CMakeFiles/sql.dir/Search_Compatibility.cpp.o
src/sql/libsql.a: src/sql/CMakeFiles/sql.dir/Search_by_category.cpp.o
src/sql/libsql.a: src/sql/CMakeFiles/sql.dir/sql_queries.cpp.o
src/sql/libsql.a: src/sql/CMakeFiles/sql.dir/Retrieve_Template.cpp.o
src/sql/libsql.a: src/sql/CMakeFiles/sql.dir/Search_by_item.cpp.o
src/sql/libsql.a: src/sql/CMakeFiles/sql.dir/build.make
src/sql/libsql.a: src/sql/CMakeFiles/sql.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX static library libsql.a"
	cd /home/ahmad/Desktop/Sarf/Build/src/sql && $(CMAKE_COMMAND) -P CMakeFiles/sql.dir/cmake_clean_target.cmake
	cd /home/ahmad/Desktop/Sarf/Build/src/sql && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/sql.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/sql/CMakeFiles/sql.dir/build: src/sql/libsql.a
.PHONY : src/sql/CMakeFiles/sql.dir/build

src/sql/CMakeFiles/sql.dir/requires: src/sql/CMakeFiles/sql.dir/Search_Compatibility.cpp.o.requires
src/sql/CMakeFiles/sql.dir/requires: src/sql/CMakeFiles/sql.dir/Search_by_category.cpp.o.requires
src/sql/CMakeFiles/sql.dir/requires: src/sql/CMakeFiles/sql.dir/sql_queries.cpp.o.requires
src/sql/CMakeFiles/sql.dir/requires: src/sql/CMakeFiles/sql.dir/Retrieve_Template.cpp.o.requires
src/sql/CMakeFiles/sql.dir/requires: src/sql/CMakeFiles/sql.dir/Search_by_item.cpp.o.requires
.PHONY : src/sql/CMakeFiles/sql.dir/requires

src/sql/CMakeFiles/sql.dir/clean:
	cd /home/ahmad/Desktop/Sarf/Build/src/sql && $(CMAKE_COMMAND) -P CMakeFiles/sql.dir/cmake_clean.cmake
.PHONY : src/sql/CMakeFiles/sql.dir/clean

src/sql/CMakeFiles/sql.dir/depend:
	cd /home/ahmad/Desktop/Sarf/Build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ahmad/Desktop/Sarf /home/ahmad/Desktop/Sarf/src/sql /home/ahmad/Desktop/Sarf/Build /home/ahmad/Desktop/Sarf/Build/src/sql /home/ahmad/Desktop/Sarf/Build/src/sql/CMakeFiles/sql.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/sql/CMakeFiles/sql.dir/depend

