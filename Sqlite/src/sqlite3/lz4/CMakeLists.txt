cmake_minimum_required(VERSION 2.8)
project(lz4)

include(CMakeWorkarounds)

set(lz4_SRCS
    lz4.c
    lz4.h
    lz4hc.c
    lz4hc.h
)

if (CMAKE_COMPILER_IS_GNUCC AND NOT WIN32)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fvisibility=hidden")
endif (CMAKE_COMPILER_IS_GNUCC AND NOT WIN32)

add_library(lz4_lib STATIC ${lz4_SRCS})
set_target_pic(lz4_lib)
