cmake_minimum_required(VERSION 2.8)
project(devkit)

include(JoinStaticLibraries)
include(CMakeWorkarounds)

set(AMALGAM_FILE "nds_extensions_amalgam.c")
set(AMALGAM_PATH "${CMAKE_CURRENT_BINARY_DIR}")
set(AMALGAM_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/extensions/nds_extensions.h")
set(EXTENSIONS_SRC "${CMAKE_CURRENT_SOURCE_DIR}/nds_extensions.c")
set(EXTENSIONS_H "${CMAKE_CURRENT_SOURCE_DIR}/nds_extensions.h")

add_custom_target(extensions_amalgamate
    COMMAND python3 ../tools/extensions_amalgamate.py sqlite3/devkit/extensions "${AMALGAM_PATH}/${AMALGAM_FILE}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${AMALGAM_PATH}/${AMALGAM_FILE}" ${EXTENSIONS_SRC}
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${AMALGAM_HEADER}" ${EXTENSIONS_H}
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    COMMENT "Generating amalgamated extensions."
)

if (WITH_EXTENSIONS_SOURCE)
    # use unamalgamated version
    set(extensions_collations_SRCS
        extensions/collations/collation_data_de_de_phonebook.c
        extensions/collations/collation_data_de_de_standard.c
        extensions/collations/collation_data_de_de.h
        extensions/collations/collation_data_zh_cn.h
        extensions/collations/collation_data_zh_cn_gb2312.c
        extensions/collations/collation_data.h
        extensions/collations/collation_element_iterator.c
        extensions/collations/collation_element_iterator.h
        extensions/collations/localized_compare.c
        extensions/collations/localized_compare.h
        extensions/collations/lookahead_buffer.c
        extensions/collations/lookahead_buffer.h
        extensions/collations/nds_collations.c
        extensions/collations/nds_collations_impl.c
        extensions/collations/nds_collations_impl.h
        extensions/collations/unichar_queue.c
        extensions/collations/unichar_queue.h
        extensions/collations/utf8_decompose_iterator.c
        extensions/collations/utf8_decompose_iterator.h
    )

    set(extensions_tokenizer_SRCS
        extensions/tokenizer/nds_unicode61.h
        extensions/tokenizer/nds_unicode61.c
        extensions/tokenizer/string_buffer.h
    )

    set(extensions_utils_SRCS
        extensions/utils/nds_extension_macros.h
        extensions/utils/nds_bool.h
        extensions/utils/unichar.h
        extensions/utils/unichar_decomposition_data.c
        extensions/utils/unichar_decomposition_data.h
        extensions/utils/unichar_utils.c
        extensions/utils/unichar_utils.h
        extensions/utils/utf8_utils.c
        extensions/utils/utf8_utils.h
    )

    set(extensions_init_SRCS
        extensions/nds_extensions.c
        extensions/nds_extensions.h
    )

    set (extensions_SRCS
        ${extensions_init_SRCS}
        ${extensions_utils_SRCS}
    )

    if (WITH_COLLATIONS)
        set (extensions_SRCS ${extensions_SRCS} ${extensions_collations_SRCS})
    endif (WITH_COLLATIONS)

    if (WITH_TOKENIZER)
        set (extensions_SRCS ${extensions_SRCS} ${extensions_tokenizer_SRCS})
    endif (WITH_TOKENIZER)

else (WITH_EXTENSIONS_SOURCE)
    set(extensions_SRCS
        nds_extensions.c
        nds_extensions.h
    )
endif (WITH_EXTENSIONS_SOURCE)

if (WITH_COLLATIONS)
    set_property(SOURCE ${extensions_SRCS} APPEND PROPERTY COMPILE_DEFINITIONS
        NDS_COLLATIONS_EXPORT
        HAVE_NDS_COLLATIONS
    )
endif (WITH_COLLATIONS)

if (WITH_TOKENIZER)
    set_property(SOURCE ${extensions_SRCS} APPEND PROPERTY COMPILE_DEFINITIONS
        HAVE_NDS_TOKENIZER
    )
endif (WITH_TOKENIZER)

# enable warnings for extension sources
if (MSVC)
    set_property(SOURCE ${extensions_SRCS} APPEND_STRING PROPERTY
        COMPILE_FLAGS " /W3"
    )
else (MSSVC)
    set_property(SOURCE ${extensions_SRCS} APPEND_STRING PROPERTY
        COMPILE_FLAGS " -Wall -Wextra"
    )
endif (MSVC)

if (CMAKE_COMPILER_IS_GNUCC AND NOT WIN32)
    set_property(SOURCE ${extensions_SRCS} APPEND_STRING PROPERTY
        COMPILE_FLAGS " -fvisibility=hidden"
    )
endif (CMAKE_COMPILER_IS_GNUCC AND NOT WIN32)

set(devkit_SRCS
    nds_sqlite3.c
    nds_sqlite3.h
    nds_compress.c
)

set(devkit_INCLUDE_DIRS
    ../lz4
    ../zlib
    ../rijndael
    ../ndsc
)

set_property(SOURCE ${devkit_SRCS} APPEND PROPERTY COMPILE_DEFINITIONS
    SQLITE_DEFAULT_FILE_FORMAT=4
    SQLITE_MAX_ATTACHED=30
    SQLITE_ENABLE_COLUMN_METADATA
    SQLITE_ENABLE_FTS4
    SQLITE_ENABLE_RTREE
    SQLITE_THREADSAFE=1
    SQLITE_ENABLE_FTS3_PARENTHESIS
    SQLITE_DIRECT_OVERFLOW_READ
    SQLITE_ENABLE_STAT4
    SQLITE_DEFAULT_AUTOVACUUM=2
    NDS_ENABLE_ZLIB
    NDS_ENABLE_AES
    NDS_ENABLE_LZ4
    NDS_ENABLE_NDSC
)

if (WITH_ICU)
    set_property(SOURCE ${devkit_SRCS} APPEND PROPERTY
        COMPILE_DEFINITIONS SQLITE_ENABLE_ICU)
    link_directories("${ICU_RELEASE_ROOT}/lib")
    list(APPEND devkit_INCLUDE_DIRS "${ICU_RELEASE_ROOT}/include")
endif (WITH_ICU)

if (WIN32)
    # for windows dll to have correct exports, for Linux exports everything (DevKit does not support this)
    set_property(SOURCE ${devkit_SRCS} APPEND PROPERTY
        COMPILE_DEFINITIONS "SQLITE_API=__declspec(dllexport)")
else (WIN32)
    # currently the visibility of devkit is left to default, so everything is visible
    #set_property(SOURCE ${devkit_SRCS} APPEND PROPERTY
    #    COMPILE_DEFINITIONS SQLITE_API=__attribute__\(\(visibility\(\"default\"\)\)\))
endif (WIN32)

set(sqlite3_shell_SRCS
    shell.c
)

set(sqlite3_analyzer_SRCS
    nds_sqlite3_analyzer.c
)

set(devkit_LIBS
    lz4_lib
    ndsc_lib
    rijndael_lib
    zlib_lib
)

# static sqlite3 library
add_library(sqlite3_core STATIC ${devkit_SRCS} ${extensions_SRCS})
target_include_directories(sqlite3_core PRIVATE
    ${devkit_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}
)
set_target_pic(sqlite3_core)

join_static_libraries(sqlite3 sqlite3_core ${devkit_LIBS})

# dynamic sqlite3 library
add_library(sqlite3dyn SHARED ${devkit_SRCS} ${extensions_SRCS})
target_include_directories(sqlite3dyn PRIVATE
    ${devkit_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(sqlite3dyn ${devkit_LIBS})

# sqlite3 shell
add_executable(sqlite3_shell ${sqlite3_shell_SRCS})
target_link_libraries(sqlite3_shell sqlite3 ${NDS_SQLITE_SHELL_LIBS})
set_target_properties(sqlite3_shell PROPERTIES OUTPUT_NAME nds_sqlite3)

if (WITH_EDITLINE)
    set_property(TARGET sqlite3_shell APPEND PROPERTY
        COMPILE_DEFINITIONS HAVE_EDITLINE)
    target_include_directories(sqlite3_shell PRIVATE ../editline)
endif (WITH_EDITLINE)

# sqlite3 analyzer
link_directories(${TCL_RELEASE_ROOT}/lib)

if (UNIX)
    set(TCL tcl8.5)
else (UNIX)
    set(TCL tcl85)
endif (UNIX)

add_executable(sqlite3_analyzer ${sqlite3_analyzer_SRCS})
target_include_directories(sqlite3_analyzer PRIVATE "${TCL_RELEASE_ROOT}/include")
include_directories("${TCL_RELEASE_ROOT}/include")
target_link_libraries(sqlite3_analyzer sqlite3 ${TCL})
set_target_properties(sqlite3_analyzer PROPERTIES OUTPUT_NAME nds_sqlite3_analyzer)

if (WIN32)
    # for nds_sqlite_analyzer: following define must be set for windows builds
     set_property(TARGET sqlite3_analyzer APPEND PROPERTY
        COMPILE_DEFINITIONS BUILD_sqlite)
endif (WIN32)

if (WITH_ICU)
    set_target_properties(sqlite3 PROPERTIES LINKER_LANGUAGE CXX)
    set_target_properties(sqlite3dyn PROPERTIES LINKER_LANGUAGE CXX)
    set_target_properties(sqlite3_shell PROPERTIES LINKER_LANGUAGE CXX)
    set_target_properties(sqlite3_analyzer PROPERTIES LINKER_LANGUAGE CXX)
endif (WITH_ICU)

if (UNIX)
    #find_library(U_DL_LIB dl)
    set(U_DL_LIB dl)

    target_link_libraries(sqlite3dyn ${U_DL_LIB})
    combined_target_link_libraries(sqlite3 ${U_DL_LIB})
    target_link_libraries(sqlite3_analyzer sqlite3)

    if (WITH_ICU)
        set(U_ICUI18N_LIB_STATIC ${ICU_RELEASE_ROOT}/lib/libicui18n.a)
        set(U_ICUUC_LIB_STATIC ${ICU_RELEASE_ROOT}/lib/libicuuc.a)
        set(U_ICUDATA_LIB_STATIC ${ICU_RELEASE_ROOT}/lib/libicudata.a)
        set(U_ICU_LIBS_STATIC ${U_ICUI18N_LIB_STATIC} ${U_ICUUC_LIB_STATIC} ${U_ICUDATA_LIB_STATIC})

        set(U_ICUI18N_LIB_SHARED ${ICU_RELEASE_ROOT}/lib/libicui18n.so.48)
        set(U_ICUUC_LIB_SHARED ${ICU_RELEASE_ROOT}/lib/libicuuc.so.48)
        set(U_ICUDATA_LIB_SHARED ${ICU_RELEASE_ROOT}/lib/libicudata.so.48)
        set(U_ICU_LIBS_SHARED ${U_ICUI18N_LIB_SHARED} ${U_ICUUC_LIB_SHARED} ${U_ICUDATA_LIB_SHARED})

        combined_target_link_libraries(sqlite3 ${U_ICU_LIBS_STATIC})
        target_link_libraries(sqlite3dyn ${U_ICU_LIBS_SHARED})
    endif (WITH_ICU)

else (UNIX)

    target_link_libraries(sqlite3_analyzer sqlite3)

    if (WITH_ICU)
        set(W_ICUI18N_LIB_STATIC ${ICU_RELEASE_ROOT}/lib/sicuin.lib)
        set(W_ICUUC_LIB_STATIC ${ICU_RELEASE_ROOT}/lib/sicuuc.lib)
        set(W_ICUDATA_LIB_STATIC ${ICU_RELEASE_ROOT}/lib/sicudt.lib)
        set(W_ICU_LIBS_STATIC ${W_ICUI18N_LIB_STATIC} ${W_ICUUC_LIB_STATIC} ${W_ICUDATA_LIB_STATIC})

        set(W_ICUI18N_LIB_SHARED ${ICU_RELEASE_ROOT}/lib/icuin48.dll)
        set(W_ICUUC_LIB_SHARED ${ICU_RELEASE_ROOT}/lib/icuuc48.dll)
        set(W_ICUDATA_LIB_SHARED ${ICU_RELEASE_ROOT}/lib/icudt48.dll)
        set(W_ICU_LIBS_SHARED ${W_ICUI18N_LIB_SHARED} ${W_ICUUC_LIB_SHARED} ${W_ICUDATA_LIB_SHARED})

        combined_target_link_libraries(sqlite3 ${W_ICU_LIBS_STATIC})
        target_link_libraries(sqlite3dyn ${W_ICU_LIBS_SHARED})
    endif (WITH_ICU)
endif (UNIX)

# cmake 2.8.0 workaround: to make sure the combined static lib is build before
#  the targets that require it
# This must come AFTER all dependencies of the combined sqlite3 target have
# already been defined!
add_combined_target_dependencies(sqlite3 sqlite3_shell sqlite3_analyzer)

# Specify different output directories so that the .lib files that are generated
# for both sqlite3 and sqlite3dyn (under MSVC) don't clash.
set_target_properties(sqlite3
    PROPERTIES
    OUTPUT_NAME sqlite3
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/sqlite_static"
)
set_target_properties(sqlite3dyn
    PROPERTIES
    OUTPUT_NAME sqlite3
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/sqlite_shared" # the .lib (under MSVC)
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/sqlite_shared" # the .dll (under MSVC)
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/sqlite_shared" # the .so (in Linux)
)

set_target_properties(sqlite3dyn sqlite3_shell sqlite3_analyzer
    PROPERTIES
    BUILD_WITH_INSTALL_RPATH TRUE
    INSTALL_RPATH_USE_LINK_PATH FALSE
    INSTALL_RPATH "$ORIGIN"
)

if (WITH_EXTENSIONS_SOURCE)
    install(FILES extensions/nds_extensions.h DESTINATION sqlite3/include)
else (WITH_EXTENSIONS_SOURCE)
    install(FILES nds_extensions.h DESTINATION sqlite3/include)
endif (WITH_EXTENSIONS_SOURCE)
install(FILES nds_sqlite3.h DESTINATION sqlite3/include)
install_combined_library(TARGETS sqlite3 DESTINATION sqlite3/lib)
install(TARGETS sqlite3dyn LIBRARY DESTINATION sqlite3/lib RUNTIME DESTINATION sqlite3/lib)
if (MSVC)
    # install import library
    # TODO: name clashes with static library
    install(TARGETS sqlite3dyn ARCHIVE DESTINATION sqlite3/lib)
endif (MSVC)
install(TARGETS sqlite3_shell DESTINATION sqlite3/bin)
install(TARGETS sqlite3_analyzer DESTINATION sqlite3/bin)

if (UNIX)
    if (WITH_ICU)
        install(FILES ${U_ICU_LIBS_SHARED} DESTINATION sqlite3/bin)
        install(FILES ${U_ICU_LIBS_STATIC} DESTINATION sqlite3/lib)
        install(FILES ${U_ICU_LIBS_SHARED} DESTINATION sqlite3/lib)
    endif (WITH_ICU)
    install(FILES ${TCL_RELEASE_ROOT}/lib/libtcl8.5.so DESTINATION sqlite3/bin)
else (UNIX)
    if (WITH_ICU)
        # find and copy C++ runtime
        # convert CMAKE_C_FLAGS into a list so that it expands correctly in execute_process()
        separate_arguments(C_FLAGS_LIST UNIX_COMMAND "${CMAKE_C_FLAGS}")
        set(LIBGCC_S_SJLJ_FILE libgcc_s_sjlj-1.dll)
        set(LIBSTDCXX_FILE libstdc++-6.dll)
        execute_process(
            COMMAND ${CMAKE_C_COMPILER} ${C_FLAGS_LIST} -print-file-name=${LIBGCC_S_SJLJ_FILE}
            OUTPUT_VARIABLE LIBGCC_S_SJLJ
            OUTPUT_STRIP_TRAILING_WHITESPACE)
        execute_process(
            COMMAND ${CMAKE_C_COMPILER} ${C_FLAGS_LIST} -print-file-name=${LIBSTDCXX_FILE}
            OUTPUT_VARIABLE LIBSTDCXX
            OUTPUT_STRIP_TRAILING_WHITESPACE)

        # hack to work with old toolchain
        if ("${LIBGCC_S_SJLJ}" STREQUAL "${LIBGCC_S_SJLJ_FILE}")
            set(LIBGCC_S_SJLJ "${CMAKE_FIND_ROOT_PATH}/bin/${LIBGCC_S_SJLJ_FILE}")
        endif()
        if ("${LIBSTDCXX}" STREQUAL "${LIBSTDCXX_FILE}")
            set(LIBSTDCXX "${CMAKE_FIND_ROOT_PATH}/bin/${LIBSTDCXX_FILE}")
        endif()

        install(FILES ${LIBGCC_S_SJLJ} DESTINATION sqlite3/lib)
        install(FILES ${LIBSTDCXX} DESTINATION sqlite3/lib)
        install(FILES ${LIBGCC_S_SJLJ} DESTINATION sqlite3/bin)
        install(FILES ${LIBSTDCXX} DESTINATION sqlite3/bin)

        install(FILES ${W_ICU_LIBS_SHARED} DESTINATION sqlite3/bin)
        install(FILES ${W_ICU_LIBS_STATIC} DESTINATION sqlite3/lib)
        install(FILES ${W_ICU_LIBS_SHARED} DESTINATION sqlite3/lib)
    endif (WITH_ICU)
    install(FILES ${TCL_RELEASE_ROOT}/bin/tcl85.dll DESTINATION sqlite3/bin)
endif (UNIX)
