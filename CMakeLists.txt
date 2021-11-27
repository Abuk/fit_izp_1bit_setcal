cmake_minimum_required(VERSION 3.20)
project(Project_02 C)

set(CMAKE_C_STANDARD 99)
set(GCC_COMPILE_FLAGS "-Wall -Wextra -Werror")

add_definitions(${GCC_COMPILE_FLAGS})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GCC_COMPILE_FLAGS}")

add_executable(Project_02 setcal.c utils/loader.c utils/loader.h utils/parser.c utils/parser.h utils/types.c utils/types.h)
