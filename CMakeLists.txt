cmake_minimum_required(VERSION 3.10)
project(toolchain_numeric)

set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_FLAGS "-pedantic -Wall -Werror -ggdb3 -DVARIANT_INTEGRAL")
set(CMAKE_CXX_FLAGS "-pedantic -Wall -Werror -ggdb3 -Ofast")
file(GLOB_RECURSE NUMERIC_SRC "src/*.cpp")
file(GLOB_RECURSE NUMERIC_INCLUDE "include/*.hpp include/*.h")

set(libnumeric numeric.so)
add_library(${libnumeric} ${NUMERIC_SRC} ${NUMERIC_INCLUDE})

add_executable(numeric examples/numeric.cpp)
target_link_libraries(numeric ${libnumeric})

