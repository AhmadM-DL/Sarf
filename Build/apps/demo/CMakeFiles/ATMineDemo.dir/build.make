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
include apps/demo/CMakeFiles/ATMineDemo.dir/depend.make

# Include the progress variables for this target.
include apps/demo/CMakeFiles/ATMineDemo.dir/progress.make

# Include the compile flags for this target's objects.
include apps/demo/CMakeFiles/ATMineDemo.dir/flags.make

apps/demo/ui_mainwindow.h: ../apps/demo/mainwindow.ui
	$(CMAKE_COMMAND) -E cmake_progress_report /home/ahmad/Desktop/Sarf/Build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Generating ui_mainwindow.h"
	cd /home/ahmad/Desktop/Sarf/Build/apps/demo && /usr/lib/x86_64-linux-gnu/qt5/bin/uic -o /home/ahmad/Desktop/Sarf/Build/apps/demo/ui_mainwindow.h /home/ahmad/Desktop/Sarf/apps/demo/mainwindow.ui

apps/demo/CMakeFiles/ATMineDemo.dir/initializationworker.cpp.o: apps/demo/CMakeFiles/ATMineDemo.dir/flags.make
apps/demo/CMakeFiles/ATMineDemo.dir/initializationworker.cpp.o: ../apps/demo/initializationworker.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/ahmad/Desktop/Sarf/Build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object apps/demo/CMakeFiles/ATMineDemo.dir/initializationworker.cpp.o"
	cd /home/ahmad/Desktop/Sarf/Build/apps/demo && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/ATMineDemo.dir/initializationworker.cpp.o -c /home/ahmad/Desktop/Sarf/apps/demo/initializationworker.cpp

apps/demo/CMakeFiles/ATMineDemo.dir/initializationworker.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ATMineDemo.dir/initializationworker.cpp.i"
	cd /home/ahmad/Desktop/Sarf/Build/apps/demo && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/ahmad/Desktop/Sarf/apps/demo/initializationworker.cpp > CMakeFiles/ATMineDemo.dir/initializationworker.cpp.i

apps/demo/CMakeFiles/ATMineDemo.dir/initializationworker.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ATMineDemo.dir/initializationworker.cpp.s"
	cd /home/ahmad/Desktop/Sarf/Build/apps/demo && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/ahmad/Desktop/Sarf/apps/demo/initializationworker.cpp -o CMakeFiles/ATMineDemo.dir/initializationworker.cpp.s

apps/demo/CMakeFiles/ATMineDemo.dir/initializationworker.cpp.o.requires:
.PHONY : apps/demo/CMakeFiles/ATMineDemo.dir/initializationworker.cpp.o.requires

apps/demo/CMakeFiles/ATMineDemo.dir/initializationworker.cpp.o.provides: apps/demo/CMakeFiles/ATMineDemo.dir/initializationworker.cpp.o.requires
	$(MAKE) -f apps/demo/CMakeFiles/ATMineDemo.dir/build.make apps/demo/CMakeFiles/ATMineDemo.dir/initializationworker.cpp.o.provides.build
.PHONY : apps/demo/CMakeFiles/ATMineDemo.dir/initializationworker.cpp.o.provides

apps/demo/CMakeFiles/ATMineDemo.dir/initializationworker.cpp.o.provides.build: apps/demo/CMakeFiles/ATMineDemo.dir/initializationworker.cpp.o

apps/demo/CMakeFiles/ATMineDemo.dir/main.cpp.o: apps/demo/CMakeFiles/ATMineDemo.dir/flags.make
apps/demo/CMakeFiles/ATMineDemo.dir/main.cpp.o: ../apps/demo/main.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/ahmad/Desktop/Sarf/Build/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object apps/demo/CMakeFiles/ATMineDemo.dir/main.cpp.o"
	cd /home/ahmad/Desktop/Sarf/Build/apps/demo && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/ATMineDemo.dir/main.cpp.o -c /home/ahmad/Desktop/Sarf/apps/demo/main.cpp

apps/demo/CMakeFiles/ATMineDemo.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ATMineDemo.dir/main.cpp.i"
	cd /home/ahmad/Desktop/Sarf/Build/apps/demo && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/ahmad/Desktop/Sarf/apps/demo/main.cpp > CMakeFiles/ATMineDemo.dir/main.cpp.i

apps/demo/CMakeFiles/ATMineDemo.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ATMineDemo.dir/main.cpp.s"
	cd /home/ahmad/Desktop/Sarf/Build/apps/demo && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/ahmad/Desktop/Sarf/apps/demo/main.cpp -o CMakeFiles/ATMineDemo.dir/main.cpp.s

apps/demo/CMakeFiles/ATMineDemo.dir/main.cpp.o.requires:
.PHONY : apps/demo/CMakeFiles/ATMineDemo.dir/main.cpp.o.requires

apps/demo/CMakeFiles/ATMineDemo.dir/main.cpp.o.provides: apps/demo/CMakeFiles/ATMineDemo.dir/main.cpp.o.requires
	$(MAKE) -f apps/demo/CMakeFiles/ATMineDemo.dir/build.make apps/demo/CMakeFiles/ATMineDemo.dir/main.cpp.o.provides.build
.PHONY : apps/demo/CMakeFiles/ATMineDemo.dir/main.cpp.o.provides

apps/demo/CMakeFiles/ATMineDemo.dir/main.cpp.o.provides.build: apps/demo/CMakeFiles/ATMineDemo.dir/main.cpp.o

apps/demo/CMakeFiles/ATMineDemo.dir/mainwindow.cpp.o: apps/demo/CMakeFiles/ATMineDemo.dir/flags.make
apps/demo/CMakeFiles/ATMineDemo.dir/mainwindow.cpp.o: ../apps/demo/mainwindow.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/ahmad/Desktop/Sarf/Build/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object apps/demo/CMakeFiles/ATMineDemo.dir/mainwindow.cpp.o"
	cd /home/ahmad/Desktop/Sarf/Build/apps/demo && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/ATMineDemo.dir/mainwindow.cpp.o -c /home/ahmad/Desktop/Sarf/apps/demo/mainwindow.cpp

apps/demo/CMakeFiles/ATMineDemo.dir/mainwindow.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ATMineDemo.dir/mainwindow.cpp.i"
	cd /home/ahmad/Desktop/Sarf/Build/apps/demo && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/ahmad/Desktop/Sarf/apps/demo/mainwindow.cpp > CMakeFiles/ATMineDemo.dir/mainwindow.cpp.i

apps/demo/CMakeFiles/ATMineDemo.dir/mainwindow.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ATMineDemo.dir/mainwindow.cpp.s"
	cd /home/ahmad/Desktop/Sarf/Build/apps/demo && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/ahmad/Desktop/Sarf/apps/demo/mainwindow.cpp -o CMakeFiles/ATMineDemo.dir/mainwindow.cpp.s

apps/demo/CMakeFiles/ATMineDemo.dir/mainwindow.cpp.o.requires:
.PHONY : apps/demo/CMakeFiles/ATMineDemo.dir/mainwindow.cpp.o.requires

apps/demo/CMakeFiles/ATMineDemo.dir/mainwindow.cpp.o.provides: apps/demo/CMakeFiles/ATMineDemo.dir/mainwindow.cpp.o.requires
	$(MAKE) -f apps/demo/CMakeFiles/ATMineDemo.dir/build.make apps/demo/CMakeFiles/ATMineDemo.dir/mainwindow.cpp.o.provides.build
.PHONY : apps/demo/CMakeFiles/ATMineDemo.dir/mainwindow.cpp.o.provides

apps/demo/CMakeFiles/ATMineDemo.dir/mainwindow.cpp.o.provides.build: apps/demo/CMakeFiles/ATMineDemo.dir/mainwindow.cpp.o

apps/demo/CMakeFiles/ATMineDemo.dir/statusbar.cpp.o: apps/demo/CMakeFiles/ATMineDemo.dir/flags.make
apps/demo/CMakeFiles/ATMineDemo.dir/statusbar.cpp.o: ../apps/demo/statusbar.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/ahmad/Desktop/Sarf/Build/CMakeFiles $(CMAKE_PROGRESS_5)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object apps/demo/CMakeFiles/ATMineDemo.dir/statusbar.cpp.o"
	cd /home/ahmad/Desktop/Sarf/Build/apps/demo && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/ATMineDemo.dir/statusbar.cpp.o -c /home/ahmad/Desktop/Sarf/apps/demo/statusbar.cpp

apps/demo/CMakeFiles/ATMineDemo.dir/statusbar.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ATMineDemo.dir/statusbar.cpp.i"
	cd /home/ahmad/Desktop/Sarf/Build/apps/demo && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/ahmad/Desktop/Sarf/apps/demo/statusbar.cpp > CMakeFiles/ATMineDemo.dir/statusbar.cpp.i

apps/demo/CMakeFiles/ATMineDemo.dir/statusbar.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ATMineDemo.dir/statusbar.cpp.s"
	cd /home/ahmad/Desktop/Sarf/Build/apps/demo && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/ahmad/Desktop/Sarf/apps/demo/statusbar.cpp -o CMakeFiles/ATMineDemo.dir/statusbar.cpp.s

apps/demo/CMakeFiles/ATMineDemo.dir/statusbar.cpp.o.requires:
.PHONY : apps/demo/CMakeFiles/ATMineDemo.dir/statusbar.cpp.o.requires

apps/demo/CMakeFiles/ATMineDemo.dir/statusbar.cpp.o.provides: apps/demo/CMakeFiles/ATMineDemo.dir/statusbar.cpp.o.requires
	$(MAKE) -f apps/demo/CMakeFiles/ATMineDemo.dir/build.make apps/demo/CMakeFiles/ATMineDemo.dir/statusbar.cpp.o.provides.build
.PHONY : apps/demo/CMakeFiles/ATMineDemo.dir/statusbar.cpp.o.provides

apps/demo/CMakeFiles/ATMineDemo.dir/statusbar.cpp.o.provides.build: apps/demo/CMakeFiles/ATMineDemo.dir/statusbar.cpp.o

apps/demo/CMakeFiles/ATMineDemo.dir/ATMineDemo_automoc.cpp.o: apps/demo/CMakeFiles/ATMineDemo.dir/flags.make
apps/demo/CMakeFiles/ATMineDemo.dir/ATMineDemo_automoc.cpp.o: apps/demo/ATMineDemo_automoc.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/ahmad/Desktop/Sarf/Build/CMakeFiles $(CMAKE_PROGRESS_6)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object apps/demo/CMakeFiles/ATMineDemo.dir/ATMineDemo_automoc.cpp.o"
	cd /home/ahmad/Desktop/Sarf/Build/apps/demo && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/ATMineDemo.dir/ATMineDemo_automoc.cpp.o -c /home/ahmad/Desktop/Sarf/Build/apps/demo/ATMineDemo_automoc.cpp

apps/demo/CMakeFiles/ATMineDemo.dir/ATMineDemo_automoc.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ATMineDemo.dir/ATMineDemo_automoc.cpp.i"
	cd /home/ahmad/Desktop/Sarf/Build/apps/demo && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/ahmad/Desktop/Sarf/Build/apps/demo/ATMineDemo_automoc.cpp > CMakeFiles/ATMineDemo.dir/ATMineDemo_automoc.cpp.i

apps/demo/CMakeFiles/ATMineDemo.dir/ATMineDemo_automoc.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ATMineDemo.dir/ATMineDemo_automoc.cpp.s"
	cd /home/ahmad/Desktop/Sarf/Build/apps/demo && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/ahmad/Desktop/Sarf/Build/apps/demo/ATMineDemo_automoc.cpp -o CMakeFiles/ATMineDemo.dir/ATMineDemo_automoc.cpp.s

apps/demo/CMakeFiles/ATMineDemo.dir/ATMineDemo_automoc.cpp.o.requires:
.PHONY : apps/demo/CMakeFiles/ATMineDemo.dir/ATMineDemo_automoc.cpp.o.requires

apps/demo/CMakeFiles/ATMineDemo.dir/ATMineDemo_automoc.cpp.o.provides: apps/demo/CMakeFiles/ATMineDemo.dir/ATMineDemo_automoc.cpp.o.requires
	$(MAKE) -f apps/demo/CMakeFiles/ATMineDemo.dir/build.make apps/demo/CMakeFiles/ATMineDemo.dir/ATMineDemo_automoc.cpp.o.provides.build
.PHONY : apps/demo/CMakeFiles/ATMineDemo.dir/ATMineDemo_automoc.cpp.o.provides

apps/demo/CMakeFiles/ATMineDemo.dir/ATMineDemo_automoc.cpp.o.provides.build: apps/demo/CMakeFiles/ATMineDemo.dir/ATMineDemo_automoc.cpp.o

# Object files for target ATMineDemo
ATMineDemo_OBJECTS = \
"CMakeFiles/ATMineDemo.dir/initializationworker.cpp.o" \
"CMakeFiles/ATMineDemo.dir/main.cpp.o" \
"CMakeFiles/ATMineDemo.dir/mainwindow.cpp.o" \
"CMakeFiles/ATMineDemo.dir/statusbar.cpp.o" \
"CMakeFiles/ATMineDemo.dir/ATMineDemo_automoc.cpp.o"

# External object files for target ATMineDemo
ATMineDemo_EXTERNAL_OBJECTS =

apps/demo/ATMineDemo: apps/demo/CMakeFiles/ATMineDemo.dir/initializationworker.cpp.o
apps/demo/ATMineDemo: apps/demo/CMakeFiles/ATMineDemo.dir/main.cpp.o
apps/demo/ATMineDemo: apps/demo/CMakeFiles/ATMineDemo.dir/mainwindow.cpp.o
apps/demo/ATMineDemo: apps/demo/CMakeFiles/ATMineDemo.dir/statusbar.cpp.o
apps/demo/ATMineDemo: apps/demo/CMakeFiles/ATMineDemo.dir/ATMineDemo_automoc.cpp.o
apps/demo/ATMineDemo: apps/demo/CMakeFiles/ATMineDemo.dir/build.make
apps/demo/ATMineDemo: /usr/lib/x86_64-linux-gnu/libQt5Core.so.5.2.1
apps/demo/ATMineDemo: /usr/lib/x86_64-linux-gnu/libQt5Sql.so.5.2.1
apps/demo/ATMineDemo: /usr/lib/x86_64-linux-gnu/libQt5Gui.so.5.2.1
apps/demo/ATMineDemo: /usr/lib/x86_64-linux-gnu/libQt5Widgets.so.5.2.1
apps/demo/ATMineDemo: src/build/libbuild.a
apps/demo/ATMineDemo: src/cache/libcache.a
apps/demo/ATMineDemo: src/case/libcase.a
apps/demo/ATMineDemo: src/common/libcommon.a
apps/demo/ATMineDemo: src/log/liblog.a
apps/demo/ATMineDemo: src/sarf/libsarf.a
apps/demo/ATMineDemo: src/sql/libsql.a
apps/demo/ATMineDemo: src/util/libutil.a
apps/demo/ATMineDemo: /usr/lib/x86_64-linux-gnu/libQt5Widgets.so.5.2.1
apps/demo/ATMineDemo: /usr/lib/x86_64-linux-gnu/libQt5Gui.so.5.2.1
apps/demo/ATMineDemo: src/cache/libcache.a
apps/demo/ATMineDemo: src/common/libcommon.a
apps/demo/ATMineDemo: src/cache/libcache.a
apps/demo/ATMineDemo: src/common/libcommon.a
apps/demo/ATMineDemo: src/sql/libsql.a
apps/demo/ATMineDemo: /usr/lib/x86_64-linux-gnu/libQt5Sql.so.5.2.1
apps/demo/ATMineDemo: /usr/lib/x86_64-linux-gnu/libQt5Core.so.5.2.1
apps/demo/ATMineDemo: apps/demo/CMakeFiles/ATMineDemo.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable ATMineDemo"
	cd /home/ahmad/Desktop/Sarf/Build/apps/demo && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ATMineDemo.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
apps/demo/CMakeFiles/ATMineDemo.dir/build: apps/demo/ATMineDemo
.PHONY : apps/demo/CMakeFiles/ATMineDemo.dir/build

apps/demo/CMakeFiles/ATMineDemo.dir/requires: apps/demo/CMakeFiles/ATMineDemo.dir/initializationworker.cpp.o.requires
apps/demo/CMakeFiles/ATMineDemo.dir/requires: apps/demo/CMakeFiles/ATMineDemo.dir/main.cpp.o.requires
apps/demo/CMakeFiles/ATMineDemo.dir/requires: apps/demo/CMakeFiles/ATMineDemo.dir/mainwindow.cpp.o.requires
apps/demo/CMakeFiles/ATMineDemo.dir/requires: apps/demo/CMakeFiles/ATMineDemo.dir/statusbar.cpp.o.requires
apps/demo/CMakeFiles/ATMineDemo.dir/requires: apps/demo/CMakeFiles/ATMineDemo.dir/ATMineDemo_automoc.cpp.o.requires
.PHONY : apps/demo/CMakeFiles/ATMineDemo.dir/requires

apps/demo/CMakeFiles/ATMineDemo.dir/clean:
	cd /home/ahmad/Desktop/Sarf/Build/apps/demo && $(CMAKE_COMMAND) -P CMakeFiles/ATMineDemo.dir/cmake_clean.cmake
.PHONY : apps/demo/CMakeFiles/ATMineDemo.dir/clean

apps/demo/CMakeFiles/ATMineDemo.dir/depend: apps/demo/ui_mainwindow.h
	cd /home/ahmad/Desktop/Sarf/Build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ahmad/Desktop/Sarf /home/ahmad/Desktop/Sarf/apps/demo /home/ahmad/Desktop/Sarf/Build /home/ahmad/Desktop/Sarf/Build/apps/demo /home/ahmad/Desktop/Sarf/Build/apps/demo/CMakeFiles/ATMineDemo.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : apps/demo/CMakeFiles/ATMineDemo.dir/depend

