#include "test_compare.h"
#include "extensions_test.h"

#include <string.h>
#include <assert.h>

#include "../collations/collation_data.h"
#include "../collations/collation_data_de_de.h"
#include "../collations/localized_compare.h"
#include "../collations/unichar_queue.h"

static int testCompare(CollationData const & cd, const char * a, const char * b)
{
    const unsigned lenA = strlen(a);
    const unsigned lenB = strlen(b);

    return LocalizedCompare(&cd, a, lenA, b, lenB);
}

static int testCompareS(const char * a, const char * b)
{
    return testCompare(CollationData_de_DE_standard, a, b);
}

static int testCompareP(const char * a, const char * b)
{
    return testCompare(CollationData_de_DE_phonebook, a, b);
}

void TestCompare()
{
    EXPECT_EQ(true, testCompareS("a", "b") < 0);
    EXPECT_EQ(true, testCompareS("b", "a") > 0);
    EXPECT_EQ(0, testCompareS("", ""));
    EXPECT_EQ(0, testCompareS("a", "a"));
    EXPECT_EQ(true, testCompareS("a", "") > 0);
    EXPECT_EQ(true, testCompareS("", "a") < 0);
    EXPECT_EQ(true, testCompareS("a", "aa") < 0);
    EXPECT_EQ(true, testCompareS("aa", "a") > 0);

    EXPECT_EQ(true, testCompareS("abc", "abd") < 0);
    EXPECT_EQ(true, testCompareS("abd", "abc") > 0);

    EXPECT_EQ(0, testCompareS("a", "á"));
    EXPECT_EQ(true, testCompareS("l", "š") < 0);

    EXPECT_EQ(0, testCompareS("a", "ä"));

    EXPECT_EQ(0, testCompareS("fussball", "fußball"));

    // from https://de.wikipedia.org/wiki/Alphabetische_Sortierung#Beispiel
    EXPECT_EQ(-1, testCompareS("Göbel", "Goethe"));
    EXPECT_EQ(-1, testCompareS("Goethe", "Goldmann"));
    EXPECT_EQ(-1, testCompareS("Goldmann", "Göthe"));
    EXPECT_EQ(-1, testCompareS("Göthe", "Götz"));

    EXPECT_EQ(-1, testCompareP("Göbel", "Goethe"));
    EXPECT_EQ(0, testCompareP("Goethe", "Göthe")); // FIXME: do we need a 2nd iteration to put oe before ö in this case?
    EXPECT_EQ(-1, testCompareP("Göthe", "Götz"));
    EXPECT_EQ(-1, testCompareP("Götz", "Goldmann"));
}

void TestCompare_InvalidUtf8()
{
    static const char str_P_r_caron[] = { C(0x50), C(0xc5), C(0x99), 0 };
    static const char str_P_r_broken[] = { C(0x50), C(0xc5), 0 };
    static const char str_P[] = { C(0x50), 0 };

    // str_P_r_broken must be treated as if it was str_P_r (terminated before incorrect input)
    EXPECT_EQ(0, testCompareS(str_P, str_P_r_broken));
    EXPECT_EQ(true, testCompareS(str_P_r_caron, str_P_r_broken) > 0);
    EXPECT_EQ(true, testCompareS(str_P_r_broken, str_P_r_caron) < 0);
}

void TestCompare_LookaheadBufferOverflow()
{
    static const char U_COMBINING_DIAERESIS_UTF8_1 = static_cast<char>(0xCCu);
    static const char U_COMBINING_DIAERESIS_UTF8_2 = static_cast<char>(0x88u);

    static const char str1[] = "ab";

    static const unsigned COMBINING_MARK_COUNT = UNICHAR_QUEUE_MAX_LENGTH+3;
    char str2[2+2*COMBINING_MARK_COUNT+1];

    int idx = 0;
    str2[idx++] = 'a';
    for (unsigned i = 0; i < COMBINING_MARK_COUNT; i++)
    {
        str2[idx++] = U_COMBINING_DIAERESIS_UTF8_1;
        str2[idx++] = U_COMBINING_DIAERESIS_UTF8_2;
    }
    str2[idx++] = 'b';
    str2[idx++] = 0;
    assert(idx == ARRAY_SIZE(str2));

    testCompareS(str1, str2);
    testCompareP(str1, str2);
    // test passed if it did not crash (behaviour is otherwise undefined)
}
