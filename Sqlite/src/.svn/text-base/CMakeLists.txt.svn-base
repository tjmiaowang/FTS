# -------------------------------------------------------------------------------------------------------------
# CMakeLists.txt for NDS SQLite project.
# -------------------------------------------------------------------------------------------------------------
cmake_minimum_required(VERSION 2.8)

project(NDS_SQLite3)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/../cmake)

# TODO: cmake autodetects lots of things out of the compiler,
# adding -m32 after that point is a bad idea
if ("${CV_HOSTTYPE}" STREQUAL "")
    message(WARNING "CV_HOSTTYPE is not set, falling to default (you can set it e.g. by -DCV_HOSTTYPE=ubuntu64).")
    if (UNIX)
        set(CV_HOSTTYPE ubuntu64)
    else (UNIX)
        set(CV_HOSTTYPE windows64)
    endif (UNIX)
endif ("${CV_HOSTTYPE}" STREQUAL "")

# force variable into the cache
set(CV_HOSTTYPE "${CV_HOSTTYPE}" CACHE STRING "Host type")

message(STATUS "JAVA_HOME: " $ENV{JAVA_HOME})
message(STATUS "CV_HOSTTYPE: " ${CV_HOSTTYPE})

# TODO: change WITH_EXTENSIONS_SOURCE to default OFF once amalgamation is included
option(WITH_EXTENSIONS_SOURCE "Use individual sources when building NDS extensions" ON)
option(WITH_COLLATIONS "Include NSD collations" ON)
option(WITH_TOKENIZER "Include NDS tokenizer" OFF)
option(WITH_ICU "Enable ICU support in SQLite" OFF)
option(WITH_UNITTESTS "Build extensions unittests" ON)

if (UNIX)
    option(WITH_EDITLINE "Use editline library in SQLite shell" ON)
endif (UNIX)

if (WITH_ICU)
    if (NOT CMAKE_COMPILER_IS_GNUCC)
        message(FATAL_ERROR "ICU currently supported only under MinGW.")
    endif (NOT CMAKE_COMPILER_IS_GNUCC)

    if ("$ENV{ICU_RELEASE_ROOT}" STREQUAL "")
        set(ICU_RELEASE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/icu/${CV_HOSTTYPE}")
        message(STATUS "ICU_RELEASE_ROOT (default): " ${ICU_RELEASE_ROOT})
    else ("$ENV{ICU_RELEASE_ROOT}" STREQUAL "")
        set(ICU_RELEASE_ROOT $ENV{ICU_RELEASE_ROOT}/${CV_HOSTTYPE})
        message(STATUS "ICU_RELEASE_ROOT (user defined): " ${ICU_RELEASE_ROOT})
    endif ("$ENV{ICU_RELEASE_ROOT}" STREQUAL "")
else (WITH_ICU)
    message(STATUS "ICU build disabled.")
endif (WITH_ICU)

if ("$ENV{TCL_RELEASE_ROOT}" STREQUAL "")
    set(TCL_RELEASE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/tcl/${CV_HOSTTYPE}")
    message(STATUS "TCL_RELEASE_ROOT (default): "  ${TCL_RELEASE_ROOT})
else ("$ENV{TCL_RELEASE_ROOT}" STREQUAL "")
    set(TCL_RELEASE_ROOT $ENV{TCL_RELEASE_ROOT}/${CV_HOSTTYPE})
    message(STATUS "TCL_RELEASE_ROOT (user defined): "  ${TCL_RELEASE_ROOT})
endif ("$ENV{TCL_RELEASE_ROOT}" STREQUAL "")

if (CMAKE_COMPILER_IS_GNUCC)
    if (UNIX)
        if (${CV_HOSTTYPE} MATCHES "32")
            set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")
        else (${CV_HOSTTYPE} MATCHES "32")
            set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m64")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64")
        endif (${CV_HOSTTYPE} MATCHES "32")
    else (UNIX)
        set(CMAKE_MODULE_LINKER_FLAGS "-Wl,--kill-at")
    endif (UNIX)

    # set standard to match MSVC (use gnu89 instead of c89 to enable C++-style comments)
    # enable warnings to flag declarations after statement
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu89 -Wdeclaration-after-statement")

    if (UNIX)
        # add -pthread to enable threading (this automatically adds libpthread to linking)
        # FindThreads.cmake is not used because of http://www.cmake.org/Bug/view.php?id=14767
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread -Wl,--as-needed")
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pthread -Wl,--as-needed")
    endif (UNIX)
endif (CMAKE_COMPILER_IS_GNUCC)

if (MSVC)
    # don't warn about unsafe CRT functions
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)

    # disable warning about deperecated functions
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /wd4996")

    # do not enable warnings (CMake sets /W3 by default)
    string(REGEX REPLACE "/W[1234]" "" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
endif (MSVC)

if (WITH_UNITTESTS)
    enable_testing()
endif (WITH_UNITTESTS)

add_subdirectory(sqlite3)
