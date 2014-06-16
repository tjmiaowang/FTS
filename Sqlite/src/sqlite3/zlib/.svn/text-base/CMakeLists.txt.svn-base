cmake_minimum_required(VERSION 2.8)
project(zlib)

include(CMakeWorkarounds)

set(zlib_SRCS
    adler32.c
    compress.c
    crc32.c
    crc32.h
    deflate.c
    deflate.h
    gzclose.c
    gzguts.h
    gzlib.c
    gzread.c
    gzwrite.c
    infback.c
    inffast.c
    inffast.h
    inffixed.h
    inflate.c
    inflate.h
    inftrees.c
    inftrees.h
    trees.c
    trees.h
    uncompr.c
    zconf.h
    zlib.h
    zutil.c
    zutil.h
)

if (CMAKE_COMPILER_IS_GNUCC AND NOT WIN32)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fvisibility=hidden")
endif (CMAKE_COMPILER_IS_GNUCC AND NOT WIN32)

add_library(zlib_lib STATIC ${zlib_SRCS})
set_target_pic(zlib_lib)
