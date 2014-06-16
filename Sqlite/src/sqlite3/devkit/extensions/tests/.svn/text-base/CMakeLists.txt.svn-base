cmake_minimum_required(VERSION 2.8)
project(extensions_tests)

include(JoinStaticLibraries)

set(extensions_unit_tests_SRCS
    extensions_test.cpp
    extensions_test.h
    test_unichar_utils.cpp
    test_unichar_utils.h
    test_utf8_char_next.cpp
    test_utf8_char_next.h
)

if (WITH_COLLATIONS)
    set(extensions_unit_tests_SRCS
        ${extensions_unit_tests_SRCS}
        test_collation_element_iterator.cpp
        test_collation_element_iterator.h
        test_compare.cpp
        test_compare.h
        test_lookahead_buffer.cpp
        test_lookahead_buffer.h
        test_unichar_queue.cpp
        test_unichar_queue.h
        test_utf8_decompose_iterator.cpp
        test_utf8_decompose_iterator.h
    )
endif (WITH_COLLATIONS)

if (WITH_TOKENIZER)
    set(extensions_unit_tests_SRCS
        ${extensions_unit_tests_SRCS}
        test_ndsunicode61_tokenizer.cpp
        test_ndsunicode61_tokenizer.h
    )
endif (WITH_TOKENIZER)

if (WITH_COLLATIONS)
    add_definitions(-DNDS_COLLATIONS_EXPORT)
    add_definitions(-DHAVE_NDS_COLLATIONS)
endif (WITH_COLLATIONS)

if (WITH_TOKENIZER)
    add_definitions(-DHAVE_NDS_TOKENIZER)
endif (WITH_TOKENIZER)

include_directories(../../..)

# ugly CMake 2.8.0 workaround for missing IMPORTED GLOBAL libraries
summon_combined_target(sqlite3)

add_executable(extensions_unit_tests ${extensions_unit_tests_SRCS})
target_link_libraries(extensions_unit_tests sqlite3)

# and another CMake 2.8.0 hack for missing dependency
add_combined_target_dependencies(sqlite3 extensions_unit_tests)

add_test(NAME Utf8CharNext COMMAND extensions_unit_tests Utf8CharNext)
add_test(NAME Utf8CharNext_NullArgs COMMAND extensions_unit_tests Utf8CharNext_NullArgs)
add_test(NAME Utf8CharNext_InvalidUtf8 COMMAND extensions_unit_tests Utf8CharNext_InvalidUtf8)
add_test(NAME Utf8CharNext_TruncatedInput COMMAND extensions_unit_tests Utf8CharNext_TruncatedInput)
add_test(NAME Utf8CharNext_Write1 COMMAND extensions_unit_tests Utf8CharNext_Write1)
add_test(NAME Utf8CharNext_Write2 COMMAND extensions_unit_tests Utf8CharNext_Write2)
add_test(NAME Utf8CharNext_Write3 COMMAND extensions_unit_tests Utf8CharNext_Write3)
add_test(NAME Utf8CharNext_Write4 COMMAND extensions_unit_tests Utf8CharNext_Write4)
add_test(NAME TestUnicharDecompose_null COMMAND extensions_unit_tests TestUnicharDecompose_null)
add_test(NAME TestUnicharDecompose_latin COMMAND extensions_unit_tests TestUnicharDecompose_latin)
add_test(NAME TestUnicharDecompose_hiragana COMMAND extensions_unit_tests TestUnicharDecompose_hiragana)
add_test(NAME TestUnicharDecompose_katakana COMMAND extensions_unit_tests TestUnicharDecompose_katakana)
add_test(NAME TestUnicharDecompose_circledKatakana COMMAND extensions_unit_tests TestUnicharDecompose_circledKatakana)
add_test(NAME TestUnicharDecompose_fullwidthLatin COMMAND extensions_unit_tests TestUnicharDecompose_fullwidthLatin)
add_test(NAME TestUnicharDecompose_fullwidthDigit COMMAND extensions_unit_tests TestUnicharDecompose_fullwidthDigit)
add_test(NAME TestUnicharDecompose_fullwidthSymbol COMMAND extensions_unit_tests TestUnicharDecompose_fullwidthSymbol)

if (WITH_COLLATIONS)
    add_test(NAME Utf8DecomposeIterator COMMAND extensions_unit_tests Utf8DecomposeIterator)
    add_test(NAME Utf8DecomposeIterator_NullArgs COMMAND extensions_unit_tests Utf8DecomposeIterator_NullArgs)
    add_test(NAME Utf8DecomposeIterator_InvalidUtf8 COMMAND extensions_unit_tests Utf8DecomposeIterator_InvalidUtf8)
    add_test(NAME Utf8DecomposeIterator_TruncatedInput COMMAND extensions_unit_tests Utf8DecomposeIterator_TruncatedInput)

    add_test(NAME UnicharQueue COMMAND extensions_unit_tests UnicharQueue)
    add_test(NAME UnicharQueue_NullArgs COMMAND extensions_unit_tests UnicharQueue_NullArgs)

    add_test(NAME LookaheadBuffer COMMAND extensions_unit_tests LookaheadBuffer)
    add_test(NAME LookaheadBuffer_NullArgs COMMAND extensions_unit_tests LookaheadBuffer_NullArgs)

    add_test(NAME CollationElementIterator COMMAND extensions_unit_tests CollationElementIterator)
    add_test(NAME CollationElementIterator_NullArgs COMMAND extensions_unit_tests CollationElementIterator_NullArgs)
    add_test(NAME CollationElementIterator_InvalidUtf8 COMMAND extensions_unit_tests CollationElementIterator_InvalidUtf8)
    add_test(NAME CollationElementIterator_TruncatedInput COMMAND extensions_unit_tests CollationElementIterator_TruncatedInput)
    add_test(NAME CollationElementIterator_CombiningMarkSequence COMMAND extensions_unit_tests CollationElementIterator_CombiningMarkSequence)
    add_test(NAME CollationElementIterator_LookaheadBufferOverflow COMMAND extensions_unit_tests CollationElementIterator_LookaheadBufferOverflow)
    add_test(NAME CollationElementIterator_SimpleElements COMMAND extensions_unit_tests CollationElementIterator_SimpleElements)

    add_test(NAME Compare COMMAND extensions_unit_tests Compare)
    add_test(NAME Compare_InvalidUtf8 COMMAND extensions_unit_tests Compare_InvalidUtf8)
    add_test(NAME Compare_LookaheadBufferOverflow COMMAND extensions_unit_tests Compare_LookaheadBufferOverflow)
endif (WITH_COLLATIONS)

if (WITH_TOKENIZER)
    add_test(NAME NdsUnicode61Tokenizer_open COMMAND extensions_unit_tests NdsUnicode61Tokenizer_open)
    add_test(NAME NdsUnicode61Tokenizer_create COMMAND extensions_unit_tests NdsUnicode61Tokenizer_create)
    add_test(NAME NdsUnicode61Tokenizer_latin COMMAND extensions_unit_tests NdsUnicode61Tokenizer_latin)
    add_test(NAME NdsUnicode61Tokenizer_japanese COMMAND extensions_unit_tests NdsUnicode61Tokenizer_japanese)
    add_test(NAME NdsUnicode61Tokenizer_mixed COMMAND extensions_unit_tests NdsUnicode61Tokenizer_mixed)
    add_test(NAME NdsUnicode61Tokenizer_tokencharsSeparators COMMAND extensions_unit_tests NdsUnicode61Tokenizer_tokencharsSeparators)
    add_test(NAME NdsUnicode61Tokenizer_ideographicSpace COMMAND extensions_unit_tests NdsUnicode61Tokenizer_ideographicSpace)
endif (WITH_TOKENIZER)
