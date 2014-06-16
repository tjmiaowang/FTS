cmake_minimum_required(VERSION 2.8)
project(ndsc)

include(CMakeWorkarounds)

set(ndsc_SRCS
    packerNDSC.c
    packerNDSC.h
)

if (CMAKE_COMPILER_IS_GNUCC AND NOT WIN32)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fvisibility=hidden")
endif (CMAKE_COMPILER_IS_GNUCC AND NOT WIN32)

add_library(ndsc_lib STATIC ${ndsc_SRCS})
set_target_pic(ndsc_lib)
