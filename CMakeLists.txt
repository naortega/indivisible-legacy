cmake_minimum_required(VERSION 2.6)
project(Indivisible)

cmake_policy(SET CMP0012 OLD)

set(TARGET_NAME indivisible)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_MODULE_PATH
	${CMAKE_MODULE_PATH}
	${CMAKE_SOURCE_DIR}/cmake/)

find_package(GMP REQUIRED)
find_package(OpenMP REQUIRED)

include_directories(
	${GMP_INCLUDE_DIR})

set(SRCS
	src/main.c
	src/files.c
	src/list.c)

# Define the C flags.
set(CMAKE_C_FLAGS "-std=gnu99 ${OpenMP_C_FLAGS} -Wall -Wextra -Werror -Wfatal-errors -Wmissing-declarations -pedantic-errors")
set(CMAKE_C_FLAGS_DEBUG "-g -O0")
set(CMAKE_C_FLAGS_RELEASE "-O3")
set(CMAKE_C_FLAGS_RELWITHDEBINFO "-g -O3")
set(CMAKE_C_FLAGS_MINSIZEREL "-Os")

if(NOT CMAKE_BUILD_TYPE MATCHES Debug AND NOT CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)
	add_definitions(-DNDEBUG)
endif()

add_executable(${TARGET_NAME} ${SRCS})

target_link_libraries(${TARGET_NAME}
	${GMP_LIBRARY})
