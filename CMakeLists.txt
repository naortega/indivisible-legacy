# Copyright (C) 2017  Ortega Froysa, Nicolás <deathsbreed@themusicinnoise.net>
# Author: Ortega Froysa, Nicolás <deathsbreed@themusicinnoise.net>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 2.6)
project(Indivisible)

set(TARGET_NAME indivisible)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "release")
endif()
string(TOLOWER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE)
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

set(CMAKE_MODULE_PATH
	${CMAKE_MODULE_PATH}
	${CMAKE_SOURCE_DIR}/cmake/)

find_package(GMP REQUIRED)

include_directories(
	${GMP_INCLUDE_DIR})

set(SRCS
	src/main.c)

# Define the C flags.
set(CMAKE_C_FLAGS "-std=gnu99 -Wall -Wextra -Werror -Wfatal-errors -Wmissing-declarations -pedantic-errors")
set(CMAKE_C_FLAGS_DEBUG "-g -O0")
set(CMAKE_C_FLAGS_RELEASE "-O3")
set(CMAKE_C_FLAGS_RELWITHDEBINFO "-g -O3")
set(CMAKE_C_FLAGS_MINSIZEREL "-Os")

if(NOT CMAKE_BUILD_TYPE MATCHES "debug" AND NOT CMAKE_BUILD_TYPE MATCHES "relwithdebinfo")
	add_definitions("-DNDEBUG")
else()
	add_definitions("-DDEBUG")
endif()

add_executable(${TARGET_NAME} ${SRCS})

target_link_libraries(${TARGET_NAME}
	${GMP_LIBRARY})

install(TARGETS ${TARGET_NAME}
	RUNTIME DESTINATION "bin")
