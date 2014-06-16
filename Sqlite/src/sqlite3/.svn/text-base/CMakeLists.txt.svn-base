cmake_minimum_required(VERSION 2.8)

project(nds_sqlite3)

# devkit dependencies
# these just create OBJECT targets
if (WITH_EDITLINE)
    add_subdirectory(editline)
    set(NDS_SQLITE_SHELL_LIBS ${NDS_SQLITE_SHELL_LIBS} editline_lib)
endif (WITH_EDITLINE)

add_subdirectory(lz4)
add_subdirectory(ndsc)
add_subdirectory(rijndael)
add_subdirectory(zlib)
add_subdirectory(devkit)

# JDBC driver
add_subdirectory(jdbc)

# tests
if (WITH_UNITTESTS AND (WITH_COLLATIONS OR WITH_TOKENIZER))
    add_subdirectory(devkit/extensions/tests)
endif (WITH_UNITTESTS AND (WITH_COLLATIONS OR WITH_TOKENIZER))
