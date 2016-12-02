cmake_minimum_required(VERSION 2.6)
project(Indivisible)

cmake_policy(SET CMP0012 OLD)

set(TARGET_NAME indivisible)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug)
endif()

find_package(OpenMP REQUIRED)

set(SRCS
    src/Main.cpp)

set(CMAKE_CXX_FLAGS "-std=c++11 ${OpenMP_CXX_FLAGS} -fno-elide-constructors -pedantic-errors -Wall -Wextra -Werror -Wpedantic -Winit-self -Wmissing-declarations -Wuninitialized -Wfatal-errors")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O3")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g -O0")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS} -g -O3")
set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS} -Os")

if(NOT CMAKE_BUILD_TYPE MATCHES Debug AND NOT CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)
	add_definitions(-DNDEBUG)
endif()

add_executable(${TARGET_NAME} ${SRCS})
