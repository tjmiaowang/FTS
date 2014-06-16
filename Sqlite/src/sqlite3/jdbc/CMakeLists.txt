cmake_minimum_required(VERSION 2.8)
project(jdbc)

include(JoinStaticLibraries)

set(sqlite3_jdbc_SRCS
    NativeDB.c
    NativeDB.h
)

include_directories(
    ..
    $ENV{JAVA_HOME}/include
)

if (UNIX)
    include_directories(lib/inc_linux)
else (UNIX)
    include_directories(lib/inc_win)
endif (UNIX)

# ugly CMake 2.8.0 workaround for missing IMPORTED GLOBAL libraries
summon_combined_target(sqlite3)

add_library(sqlitejdbc MODULE ${sqlite3_jdbc_SRCS})
target_link_libraries(sqlitejdbc sqlite3)

# and another CMake 2.8.0 hack for missing dependency
add_combined_target_dependencies(sqlite3 sqlitejdbc)

if (WITH_ICU)
    set_target_properties(sqlitejdbc PROPERTIES LINKER_LANGUAGE CXX)
endif (WITH_ICU)

if (WIN32 AND CMAKE_COMPILER_IS_GNUCC)
    if (WITH_ICU)
        set_target_properties(sqlitejdbc PROPERTIES LINK_FLAGS "-static-libstdc++ -static-libgcc")
    else (WITH_ICU)    
        set_target_properties(sqlitejdbc PROPERTIES LINK_FLAGS "-static-libgcc")
    endif (WITH_ICU)
endif (WIN32 AND CMAKE_COMPILER_IS_GNUCC)

set_target_properties(sqlitejdbc
    PROPERTIES
    BUILD_WITH_INSTALL_RPATH TRUE
    INSTALL_RPATH_USE_LINK_PATH FALSE
    INSTALL_RPATH "$ORIGIN"
)

if (UNIX)
    install(TARGETS sqlitejdbc DESTINATION sqlite3/jdbc)
else (UNIX)
    # using generator expressions seems like the way to go, but we need
    # old CMake support...
    get_target_property(sqlitejdbc_FILE sqlitejdbc LOCATION)

    install(PROGRAMS ${sqlitejdbc_FILE} DESTINATION sqlite3/jdbc RENAME sqlitejdbc.dll)
endif (UNIX)
