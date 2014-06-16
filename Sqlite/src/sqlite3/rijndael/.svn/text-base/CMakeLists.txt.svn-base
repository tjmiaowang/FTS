cmake_minimum_required(VERSION 2.8)
project(rijndael)

include(CMakeWorkarounds)

set(rijndael_SRCS
    rijndael.c
    rijndael.h
)

if (CMAKE_COMPILER_IS_GNUCC AND NOT WIN32)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fvisibility=hidden")
endif (CMAKE_COMPILER_IS_GNUCC AND NOT WIN32)

add_library(rijndael_lib STATIC ${rijndael_SRCS})
set_target_pic(rijndael_lib)
