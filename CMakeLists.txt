cmake_minimum_required(VERSION 2.8)

project(libastar CXX)
set(libastar_VERSION_MAJOR 0) 
set(libastar_VERSION_MINOR 1) 

set(GCC_CFLAGS "-std=c++11 -Wall -Wextra -Werror -pedantic")
set(GCC_CFLAGS_DEBUG "-g")
set(GCC_CFLAGS_RELEASE "-O2")
set(CLANG_CFLAGS "-std=c++11 -Wall -Wextra -Werror -pedantic")
set(CLANG_CFLAGS_DEBUG "-g")
set(CLANG_CFLAGS_RELEASE "-O2")
set(VS_CFLAGS "/Wall")
set(VS_CFLAGS_DEBUG "")
set(VS_CFLAGS_RELEASE "")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS ${GCC_CFLAGS})
    set(CMAKE_CXX_FLAGS_DEBUG ${GCC_CFLAGS_DEBUG})
    set(CMAKE_CXX_FLAGS_RELEASE ${GCC_CFLAGS_RELEASE})
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS ${CLANG_CFLAGS})
    set(CMAKE_CXX_FLAGS_DEBUG ${CLANG_CFLAGS_DEBUG})
    set(CMAKE_CXX_FLAGS_RELEASE ${CLANG_CFLAGS_RELEASE})
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS ${VS_CFLAGS})
    set(CMAKE_CXX_FLAGS_DEBUG ${VS_CFLAGS_DEBUG})
    set(CMAKE_CXX_FLAGS_RELEASE ${VS_CFLAGS_RELEASE})
endif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")

set(INCLUDE_DIRS include)
include_directories(${INCLUDE_DIRS})

# default install is /usr/local/include
install(DIRECTORY ${INCLUDE_DIRS} DESTINATION ${CMAKE_INSTALL_PREFIX})

# for Visual Studio folders
file(GLOB INCLUDE_FILES "${INCLUDE_DIRS}/*.h")
source_group(headers\\astar FILES ${INCLUDE_FILES})

add_subdirectory(examples)
