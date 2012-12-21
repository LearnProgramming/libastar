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

# Installing

The command

    sudo make install

will install the header files to `${CMAKE_INSTALL_PREFIX}/include/astar`.
By default, `CMAKE_INSTALL_PREFIX = /usr/local` on Linux systems.

# Using the library

A\* requires 6 things, all of which must be provided by the user.

1. A way to represent state
2. A starting state
3. An ending state
4. A way to get from one state to others, a `Generator` function
5. A way to measure distance/cost between one state and the next, a `Distance` function
6. A way to estimate distance/cost from one state to the goal, an `Estimator` function

A state can be represented by any type `T` so long as the functions can operate on the state as described below.
States must be comparable through `operator<`. That is, `bool operator<(const T& left, const T& right)` must be defined.

A `Generator` function is any callable (function, lambda, functor) that takes one state and returns
a `std::vector` of "next", "nearest", or "neighbor" states.
That is, a `Generator` function `g` must support the expression `std::vector<T> v = g(t)` for any `t` of type `const T&`.

A `Distance` is any callable that takes two states and returns the distance between or cost to get from the first state to the second.
The representation of that distance can be anything. Typical types are `int` or `double` but user defined types are also permitted.
Therefore a `Distance` function `d` must support the expression `G g = d(from, to)`,
where `from` and `to` are both of type `const T&`.

An `Estimator` is any callable that takes two states and returns the distance between or cost to get from the first state to the second.
The representation of that distance can be anything. Typical types are `int` or `double` but user defined types are also permitted.
Therefore an `Estimator` function `e` must support the expression `H h = e(from, to)`,
where `from` and `to` are both of type `const T&`.

The return types of the `Distance` and `Estimator` functions do not need to be the same but they do need to be compatible
through `operator+`. That is, there should exist a function `J operator+(const G&, const H&)`.
Two instances of type `J` must be comparable through `operator>`, meaning `bool operator>(const J& left, const J& right)`
must be defined.

The `Generator` is used to create the next potential nodes to evaluate. At the time they are generated,
the `Distance` function will be called for each with the current state as the first parameter and the potential as the second.
Similarly, the `Estimator` function will be called for each with the potential as the first parameter and the
provided ending state as the second.

These can all be passed to the convenience function `make_solver` which has the partial signature

    make_solver(const T& start, const T& goal, Generator, Distance, Estimator)
    
and will instantiate an appropriate solver of type `AStarSolver<T,G,H>`.

It is recommended to use `auto` for type deduction as this will allow the implementation to change with minimal
impact to client code. Ideally the interface will not change too drastically but backwards compatibility is not
a priority at this stage.

After instantiation, simply call `solve()`.

Presently the only way to see the solution is to call `print_solution`, which will write the backtrace to the
specified `std::ostream`, defaulting to `std::cout`. This function requires `operator<<` to exist for both
`T` and `J`. Specifically, `std::ostream& operator<<(std::ostream&, const T&)` must be defined for each type.

# Minimal example

```cpp
/*
  assume anything not explicitly defined below is appropriately defined,
  specifically that Type exists and has a get_neighbors() method returning a std::vector<Type>,
  that get_start() and get_end() each return the expected Type,
  and that operator< and operator<< are both supplied for Type comparison and output
*/
#include <astar/solver.h>

Type start = get_start();
Type end = get_end();
auto gen = [](const Type& t) { return t.get_neighbors(); };
auto dist = [](const Type& from, const Type& to) { return 1; };
auto est = [](const Type& from, const Type& to) { return 0; };

auto solver = make_solver(start, end, gen, dist, est);

solver.solve();

solver.print_solution();
```
