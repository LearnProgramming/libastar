cmake_minimum_required(VERSION 2.8)

project(libastar CXX)
set(libastar_VERSION_MAJOR 0) 
set(libastar_VERSION_MINOR 1) 

set(INCLUDE_DIRS include)
set(INCLUDE_INSTALL_DEST include/astar)
set(GCC_CFLAGS -std=c++11 -Wall -Wextra -Werror -pedantic)
set(CLANG_CFLAGS ${GCC_CFLAGS})
set(VS_CFLAGS /Wall)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CFLAGS ${GCC_CFLAGS})
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CFLAGS ${CLANG_CFLAGS})
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(CFLAGS ${VS_CFLAGS})
endif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")

include_directories(${INCLUDE_DIRS})

install(DIRECTORY ${INCLUDE_DIRS} DESTINATION ${INCLUDE_INSTALL_DEST})

# for Visual Studio folders
file(GLOB INCLUDE_FILES "${INCLUDE_DIRS}/*.h")
source_group(headers\\astar FILES ${INCLUDE_FILES})

add_subdirectory(examples)
