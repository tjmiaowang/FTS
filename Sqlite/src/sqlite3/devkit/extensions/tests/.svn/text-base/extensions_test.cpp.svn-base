#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "extensions_test.h"

#include "../utils/nds_bool.h"

#include "test_utf8_char_next.h"
#include "test_unichar_utils.h"

#ifdef HAVE_NDS_COLLATIONS
    #include "test_utf8_decompose_iterator.h"
    #include "test_unichar_queue.h"
    #include "test_lookahead_buffer.h"
    #include "test_collation_element_iterator.h"
    #include "test_compare.h"
#endif
#ifdef HAVE_NDS_TOKENIZER
    #include "test_ndsunicode61_tokenizer.h"
#endif

struct TestEntry
{
    const char * name;
    void(*testFunction)();
};

static const TestEntry Tests[] =
{
        { "Utf8CharNext", TestUtf8CharNext },
        { "Utf8CharNext_NullArgs", TestUtf8CharNext_NullArgs },
        { "Utf8CharNext_InvalidUtf8", TestUtf8CharNext_InvalidUtf8 },
        { "Utf8CharNext_TruncatedInput", TestUtf8CharNext_TruncatedInput },
        { "Utf8CharNext_Write1", TestUtf8CharNext_Write1 },
        { "Utf8CharNext_Write2", TestUtf8CharNext_Write2 },
        { "Utf8CharNext_Write3", TestUtf8CharNext_Write3 },
        { "Utf8CharNext_Write4", TestUtf8CharNext_Write4 },
        { "TestUnicharDecompose_null", TestUnicharDecompose_null },
        { "TestUnicharDecompose_latin", TestUnicharDecompose_latin },
        { "TestUnicharDecompose_hiragana", TestUnicharDecompose_hiragana },
        { "TestUnicharDecompose_katakana", TestUnicharDecompose_katakana },
        { "TestUnicharDecompose_circledKatakana", TestUnicharDecompose_circledKatakana },
        { "TestUnicharDecompose_fullwidthLatin", TestUnicharDecompose_fullwidthLatin },
        { "TestUnicharDecompose_fullwidthDigit", TestUnicharDecompose_fullwidthDigit },
        { "TestUnicharDecompose_fullwidthSymbol", TestUnicharDecompose_fullwidthSymbol },
#ifdef HAVE_NDS_COLLATIONS
        { "Utf8DecomposeIterator", TestUtf8DecomposeIterator },
        { "Utf8DecomposeIterator_NullArgs", TestUtf8DecomposeIterator_NullArgs },
        { "Utf8DecomposeIterator_InvalidUtf8", TestUtf8DecomposeIterator_InvalidUtf8 },
        { "Utf8DecomposeIterator_TruncatedInput", TestUtf8DecomposeIterator_TruncatedInput },
        { "UnicharQueue", TestUnicharQueue },
        { "UnicharQueue_NullArgs", TestUnicharQueue_NullArgs },
        { "LookaheadBuffer", TestLookaheadBuffer },
        { "LookaheadBuffer_NullArgs", TestLookaheadBuffer_NullArgs },
        { "CollationElementIterator", TestCollationElementIterator },
        { "CollationElementIterator_NullArgs", TestCollationElementIterator_NullArgs },
        { "CollationElementIterator_InvalidUtf8", TestCollationElementIterator_InvalidUtf8 },
        { "CollationElementIterator_TruncatedInput", TestCollationElementIterator_TruncatedInput },
        { "CollationElementIterator_CombiningMarkSequence", TestCollationElementIterator_CombiningMarkSequence },
        { "CollationElementIterator_LookaheadBufferOverflow", TestCollationElementIterator_LookaheadBufferOverflow },
        { "CollationElementIterator_SimpleElements", TestCollationElementIterator_SimpleElements },
        { "Compare", TestCompare },
        { "Compare_InvalidUtf8", TestCompare_InvalidUtf8 },
        { "Compare_LookaheadBufferOverflow", TestCompare_LookaheadBufferOverflow },
#endif
#ifdef HAVE_NDS_TOKENIZER
        { "NdsUnicode61Tokenizer_open", TestNdsUnicode61Tokenizer_open },
        { "NdsUnicode61Tokenizer_create", TestNdsUnicode61Tokenizer_create },
        { "NdsUnicode61Tokenizer_latin", TestNdsUnicode61Tokenizer_latin },
        { "NdsUnicode61Tokenizer_japanese", TestNdsUnicode61Tokenizer_japanese },
        { "NdsUnicode61Tokenizer_mixed", TestNdsUnicode61Tokenizer_mixed },
        { "NdsUnicode61Tokenizer_tokencharsSeparators", TestNdsUnicode61Tokenizer_tokencharsSeparators },
        { "NdsUnicode61Tokenizer_ideographicSpace", TestNdsUnicode61Tokenizer_ideographicSpace },
#endif
};
static const unsigned TestCount = sizeof(Tests)/sizeof(Tests[0]);

static bool passed = true;
static char const* testName = NULL;

void markFailure(const char * file, unsigned line, const char * function)
{
    fprintf(stderr, "[%s] expectEq FAILED in %s at line %d (function %s)\n", testName, file, line, function);
    passed = false;
}

bool runTestcase(TestEntry const& t)
{
    passed = true;
    testName = t.name;
    t.testFunction();

    if (passed)
        fprintf(stdout, "[%s] PASSED\n", t.name);

    return passed;
}

bool runAllTests()
{
    bool passed = true;
    for (unsigned i = 0; i < TestCount; i++)
    {
        bool r = runTestcase(Tests[i]);
        passed = passed && r;
    }
    return passed;
}

bool runTestByName(const char *name)
{
    for (unsigned i = 0; i < TestCount; i++)
    {
        if (strcmp(name, Tests[i].name) == 0)
            return runTestcase(Tests[i]);
    }

    fprintf(stderr, "ERROR Test %s does not exist.\n", name);
    return false;
}

bool runTestsByName(char const* const* testNames, int count)
{
    bool passed = true;
    for (int i = 0; i < count; i++)
    {
        bool r = runTestByName(testNames[i]);
        passed = passed && r;
    }
    return passed;
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        return runAllTests() ? EXIT_SUCCESS : EXIT_FAILURE;
    }
    else
    {
        return runTestsByName(argv+1, argc-1) ? EXIT_SUCCESS : EXIT_FAILURE;
    }
}
