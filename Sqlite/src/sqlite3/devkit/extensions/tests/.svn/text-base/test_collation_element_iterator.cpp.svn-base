#include "test_collation_element_iterator.h"
#include "extensions_test.h"

#include <string.h>
#include <assert.h>

#include "../utils/utf8_utils.h"
#include "../collations/utf8_decompose_iterator.h"
#include "../collations/lookahead_buffer.h"
#include "../collations/collation_element_iterator.h"
#include "../collations/collation_data.h"
#include "../collations/collation_data_de_de.h"
#include "../collations/unichar_queue.h"

void TestCollationElementIterator()
{
    static const char Utf8String[] = "abcdefghijklmnop";
    static const unsigned Utf8ByteCount = sizeof(Utf8String)/sizeof(Utf8String[0]) -1 /* exclude terminating nul byte */;
    const unsigned Utf8CharacterCount = GetUtf8StringCharCount(Utf8String, Utf8ByteCount);
    const unsigned Utf8CECount = Utf8CharacterCount;

    Utf8DecomposeIterator decomposeIt;
    Utf8DecomposeIterator_init(&decomposeIt, Utf8String, Utf8ByteCount);

    LookaheadBuffer buf;
    LookaheadBuffer_init(&buf, &decomposeIt);

    CollationElementIterator it;
    CollationElementIterator_init(&it, &CollationData_de_DE_standard, &buf);

    collation_element ceLast;
    EXPECT_EQ(true, CollationElementIterator_getNext(&it, &ceLast));

    unsigned count = 1;
    collation_element ceNext;
    while (CollationElementIterator_getNext(&it, &ceNext))
    {
        EXPECT_EQ(true, ceLast < ceNext);
        ceLast = ceNext;
        count++;
    }
    EXPECT_EQ(count, Utf8CECount);
}

void TestCollationElementIterator_NullArgs()
{
    static const char Utf8String[] = "abcdefghijklmnop";
    static const unsigned Utf8ByteCount = sizeof(Utf8String)/sizeof(Utf8String[0]) -1 /* exclude terminating nul byte */;

    Utf8DecomposeIterator decomposeIt;
    Utf8DecomposeIterator_init(&decomposeIt, Utf8String, Utf8ByteCount);

    LookaheadBuffer buf;
    LookaheadBuffer_init(&buf, &decomposeIt);

    CollationElementIterator it;
    collation_element ce;

    CollationElementIterator_init(&it, NULL, NULL);
    EXPECT_EQ(false, CollationElementIterator_getNext(&it, &ce));

    CollationElementIterator_init(&it, NULL, &buf);
    EXPECT_EQ(false, CollationElementIterator_getNext(&it, &ce));

    CollationElementIterator_init(&it, &CollationData_de_DE_standard, NULL);
    EXPECT_EQ(false, CollationElementIterator_getNext(&it, &ce));

    CollationElementIterator_init(&it, &CollationData_de_DE_standard, &buf);
    EXPECT_EQ(false, CollationElementIterator_getNext(&it, NULL));
}

void TestCollationElementIterator_InvalidUtf8()
{
    static const char Utf8String[] =
    {
        C(0x50) /* P */, C(0xc5) /* 1st byte of U+0159 */, /* Ommitted C(0x99) */
        C(0xc3), C(0xad), C(0xc5), C(0xa1), C(0x65)
    };
    static const unsigned Utf8ByteCount = sizeof(Utf8String)/sizeof(Utf8String[0]);

    Utf8DecomposeIterator decomposeIt;
    Utf8DecomposeIterator_init(&decomposeIt, Utf8String, Utf8ByteCount);

    LookaheadBuffer buf;
    LookaheadBuffer_init(&buf, &decomposeIt);

    CollationElementIterator it;
    collation_element ce;

    CollationElementIterator_init(&it, &CollationData_de_DE_standard, &buf);
    EXPECT_EQ(true, CollationElementIterator_getNext(&it, &ce));

    // invalid input reached -> must return false
    EXPECT_EQ(false, CollationElementIterator_getNext(&it, &ce));
}

void TestCollationElementIterator_TruncatedInput()
{
    static const char Utf8String[] =
    {
            C(0x61),
            // U+10000 LINEAR B SYLLABLE B008 A: f0 90 80 80
            C(0xf0), C(0x90) // truncated C(0x80), C(0x80)
    };
    static const unsigned Utf8ByteCount = sizeof(Utf8String)/sizeof(Utf8String[0]);

    Utf8DecomposeIterator decomposeIt;
    Utf8DecomposeIterator_init(&decomposeIt, Utf8String, Utf8ByteCount);

    LookaheadBuffer buf;
    LookaheadBuffer_init(&buf, &decomposeIt);

    CollationElementIterator it;
    collation_element ce;

    CollationElementIterator_init(&it, &CollationData_de_DE_standard, &buf);
    EXPECT_EQ(true, CollationElementIterator_getNext(&it, &ce));

    // invalid input reached -> must return false
    EXPECT_EQ(false, CollationElementIterator_getNext(&it, &ce));
}

namespace
{
    // test collation with the rules:
    // a < abc

    static const collation_element CE_a = 1;
    static const collation_element CE_abc = 2;

    static const CollationTrieNode CMSTest_CHILDREN_ab[] =
    {
        {
            /*.collationElements =*/ &CE_abc,
            /*.children =*/ NULL,
            /*.collationElementCount =*/ 1,
            /*.childCount =*/ 0,
            /*.character =*/ static_cast<unichar>('c'),
        },
    };

    static const CollationTrieNode CMSTest_CHILDREN_a[] =
    {
        {
            /*.collationElements =*/ NULL,
            /*.children =*/ CMSTest_CHILDREN_ab,
            /*.collationElementCount =*/ 0,
            /*.childCount =*/ ARRAY_SIZE(CMSTest_CHILDREN_ab),
            /*.character =*/ static_cast<unichar>('b'),
        },
    };

    static const CollationTrieNode CMSTest_CHILDREN[] =
    {
        {
            /*.collationElements =*/ &CE_a,
            /*.children =*/ CMSTest_CHILDREN_a,
            /*.collationElementCount =*/ 1,
            /*.childCount =*/ ARRAY_SIZE(CMSTest_CHILDREN_a),
            /*.character =*/ static_cast<unichar>('a'),
        },
    };

    const CollationTrieNode CombiningMarkSequenceTestTrie =
    {
        /*.collationElements =*/ NULL,
        /*.children =*/ CMSTest_CHILDREN,
        /*.collationElementCount =*/ 0,
        /*.childCount =*/ ARRAY_SIZE(CMSTest_CHILDREN),
        /*.character =*/ 0,
    };

    const CollationData CollationData_CMSTest =
    {
        CollationCombiningMarkSubset,
        &CombiningMarkSequenceTestTrie
    };

    int getCollationElementsFromString(CollationData const& collationData, char const* str,
            collation_element *ce, int maxElements)
    {
        Utf8DecomposeIterator decomposeIt;
        Utf8DecomposeIterator_init(&decomposeIt, str, strlen(str));

        LookaheadBuffer buf;
        LookaheadBuffer_init(&buf, &decomposeIt);

        CollationElementIterator it;
        CollationElementIterator_init(&it, &collationData, &buf);

        int i = 0;
        while (i < maxElements && CollationElementIterator_getNext(&it, ce+i))
            i++;
        return i;
    }

    bool compareStringWithCollationElements(CollationData const& collationData, char const *str,
            collation_element const* ce, int ceCount)
    {
        collation_element receivedCe[1024];

        int received = getCollationElementsFromString(collationData, str, receivedCe, ceCount);
        if (received != ceCount)
            return false;

        for (int i = 0; i < received; i++)
            if (receivedCe[i] != ce[i])
                return false;

        return true;
    }

    const char U_COMBINING_DIAERESIS_UTF8_1 = C(0xCC);
    const char U_COMBINING_DIAERESIS_UTF8_2 = C(0x88);
}

void TestCollationElementIterator_CombiningMarkSequence()
{
    static const char str_a[] = "a";
    static const collation_element ce_a[] = { CE_a };
    EXPECT_EQ(true, compareStringWithCollationElements(CollationData_CMSTest, str_a, ce_a, ARRAY_SIZE(ce_a)));

    static const char str_abc[] = "abc";
    static const collation_element ce_abc[] = { CE_abc };
    EXPECT_EQ(true, compareStringWithCollationElements(CollationData_CMSTest, str_abc, ce_abc, ARRAY_SIZE(ce_abc)));

    // combining mark should be consumed as part of 'abc'
    static const char str_abMc[] = { 'a', 'b', U_COMBINING_DIAERESIS_UTF8_1, U_COMBINING_DIAERESIS_UTF8_2, 'c', 0 };
    static const collation_element ce_abMc[] = { CE_abc };
    EXPECT_EQ(true, compareStringWithCollationElements(CollationData_CMSTest, str_abMc, ce_abMc, ARRAY_SIZE(ce_abMc)));

    // combining mark should not be consumed as part of 'abc'
    static const char str_abMa[] = { 'a', 'b', U_COMBINING_DIAERESIS_UTF8_1, U_COMBINING_DIAERESIS_UTF8_2, 'a', 0 };
    static const collation_element ce_abMa[] = { CE_a, UNDEFINED_CE, CE_a };
    EXPECT_EQ(true, compareStringWithCollationElements(CollationData_CMSTest, str_abMa, ce_abMa, ARRAY_SIZE(ce_abMa)));
}

void TestCollationElementIterator_LookaheadBufferOverflow()
{
    /*
     * have more than UNICHAR_QUEUE_MAX_LENGTH combining marks in input to
     * test situation that the lookahead buffer used in CollationElementIterator
     * overflows.
     *
     * In such case the sorting is undefined, but it must not crash.
     */
    static const unsigned COMBINING_MARK_COUNT = UNICHAR_QUEUE_MAX_LENGTH+3;
    char str[2+2*COMBINING_MARK_COUNT+1];

    int idx = 0;
    str[idx++] = 'a';
    for (unsigned i = 0; i < COMBINING_MARK_COUNT; i++)
    {
        str[idx++] = U_COMBINING_DIAERESIS_UTF8_1;
        str[idx++] = U_COMBINING_DIAERESIS_UTF8_2;
    }
    str[idx++] = 'b';
    str[idx++] = 0;
    assert(idx == ARRAY_SIZE(str));

    Utf8DecomposeIterator decomposeIt;
    Utf8DecomposeIterator_init(&decomposeIt, str, strlen(str));

    LookaheadBuffer buf;
    LookaheadBuffer_init(&buf, &decomposeIt);

    CollationElementIterator it;

    CollationElementIterator_init(&it, &CollationData_de_DE_standard, &buf);

    collation_element ce;
    while (CollationElementIterator_getNext(&it, &ce))
        ;
    // test passed if it did not crash (behaviour is otherwise undefined)
}

void TestCollationElementIterator_SimpleElements()
{
    static const collation_element CE_a = 10;
    static const collation_element CE_b = 11;
    static const collation_element CE_c = 12;
    static const collation_element CE_d = 13;
    static const collation_element CE_e = 14;
    static const collation_element CE_f = 15;
    static const collation_element CE_g = 16;
    static const collation_element CE_h = 17;
    static const collation_element CE_ch = 18;
    static const collation_element CE_i = 19;

    static const SimpleCollationEntry simpleEntries[] =
    {
        { 'A', CE_a },
        { 'B', CE_b },
        { 'D', CE_d },
        { 'E', CE_e },
        { 'F', CE_f },
        { 'G', CE_g },
        { 'H', CE_h },
        { 'I', CE_i },
        { 'a', CE_a },
        { 'b', CE_b },
        { 'd', CE_d },
        { 'e', CE_e },
        { 'f', CE_f },
        { 'g', CE_g },
        { 'h', CE_h },
        { 'i', CE_i },
    };

    static const collation_element testCE_C[] = { CE_c };
    static const collation_element testCE_CH[] = { CE_ch };
    static const CollationTrieNode testTrieChildren_C[] =
    {
        {
            /* .collationElements =*/ testCE_CH,
            /* .children =*/ NULL,
            /* .collationElementCount =*/ ARRAY_SIZE(testCE_CH),
            /* .childCount =*/ 0,
            /* .character =*/ 'H',
        },
        {
            /* .collationElements =*/ testCE_CH,
            /* .children =*/ NULL,
            /* .collationElementCount =*/ ARRAY_SIZE(testCE_CH),
            /* .childCount =*/ 0,
            /* .character =*/ 'h',
        },
    };

    static const CollationTrieNode testTrieChildren[] =
    {
        {
            /* .collationElements =*/ testCE_C,
            /* .children =*/ testTrieChildren_C,
            /* .collationElementCount =*/ ARRAY_SIZE(testCE_C),
            /* .childCount =*/ ARRAY_SIZE(testTrieChildren_C),
            /* .character =*/ 'C',
        },
        {
            /* .collationElements =*/ testCE_C,
            /* .children =*/ testTrieChildren_C,
            /* .collationElementCount =*/ ARRAY_SIZE(testCE_C),
            /* .childCount =*/ ARRAY_SIZE(testTrieChildren_C),
            /* .character =*/ 'c',
        },
    };

    static const CollationTrieNode testTrieRoot =
    {
        /* .collationElements =*/ NULL,
        /* .children =*/ testTrieChildren,
        /* .collationElementCount =*/ 0,
        /* .childCount =*/ ARRAY_SIZE(testTrieChildren),
        /* .character =*/ 0, // root has no value
    };

    static const CollationData testData =
    {
        /* .parseStyle =*/ CollationCombiningMarkSubset,
        /* .trie =*/ &testTrieRoot,
        /* .simpleEntries =*/ simpleEntries,
        /* .simpleEntryCount =*/ ARRAY_SIZE(simpleEntries),
    };

    // The test data also include some accented characters so that the handling of
    // CollationCombiningMarkSubset mode together with the simple elements is tested.
    static const char str_Test1[] =
    {
            'a', 'b', 'c', C(0xc4), C(0x8d) /*č*/, 'd', 'e', C(0xc3), C(0xa9) /*é*/, 'f', 'g', 'h', 'c', 'h', 'i', 0
    };

    static const collation_element ce_Test1[] =
    {
            CE_a, CE_b, CE_c, CE_c, CE_d, CE_e, CE_e, CE_f, CE_g, CE_h, CE_ch, CE_i
    };

    EXPECT_EQ(true, compareStringWithCollationElements(testData, str_Test1, ce_Test1, ARRAY_SIZE(ce_Test1)));
}
