# libastar

This project provides a templatized, header-only C++11 library for executing the [A\* search algorithm](http://en.wikipedia.org/wiki/A*_search_algorithm).

Although it is a header-only template library, the project comes with examples demonstrating how the library may be used.

# Requirements

This project utilizes the [CMake](http://cmake.org/) build tool. You can download and install CMake through [their download page](http://cmake.org/cmake/resources/software.html)
or through your package manager, if it is available. Debian-based distributions can install with

    sudo apt-get install cmake
    
If you wish to have a graphical interface to CMake you can install `ccmake`, which is available via `apt` as `cmake-curses-gui`

    sudo apt-get install cmake-curses-gui
    
The project also utilizes C++11 and as such requires a C++11 compliant compiler.
It has been tested with `g++-4.7` and `clang++-3.1` but should also work fine Visual Studio's `cl`.

# Building and Running Examples

CMake supports multiple platforms and build systems. It can generate full projects for several IDEs
as well as various kinds of Makefiles. These instructions are for using Unix Makefiles from a Linux command line
while in the project root directory. Consult the [CMake documentation](http://www.cmake.org/cmake/help/documentation.html)
for help with your particular setup, e.g. to create Visual Studio a project.

1. Configure the project

    If you have elected to use `ccmake`, run it first to set your desired options, else just run `cmake`.

        ccmake .
        cmake .

2. Build the executables

        make [target]

    Use `make help` to see a list of valid targets.

3. Run an example

        ./examples/<example>/bin/<target> [target options]

    See the wiki for documentation on each example.

