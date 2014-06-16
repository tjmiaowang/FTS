#include <stddef.h>
#include "test_unichar_utils.h"
#include "extensions_test.h"

#include "../utils/unichar_utils.h"
#include "../utils/nds_extension_macros.h"

static void TestUnicharDecompose(unichar c, unichar const* expected, size_t expectedLen)
{
    unichar const* decomposed = NULL;
    unsigned decompositionLen = 0;
    nds_bool r = UnicharDecompose(c, &decomposed, &decompositionLen);
    EXPECT_EQ(r, true);
    EXPECT_EQ(static_cast<unsigned long>(decompositionLen), static_cast<unsigned long>(expectedLen));
    for (size_t i = 0; i < decompositionLen; i++)
    {
        EXPECT_EQ(expected[i], decomposed[i]);
    }
}

void TestUnicharDecompose_null()
{
    static const unichar c = 0x61; // U+0061 LATIN SMALL LETTER A
    unichar const* decomposed = NULL;
    unsigned decompositionLen = 0;
    nds_bool r = UnicharDecompose(c, &decomposed, &decompositionLen);

    // can't be decomposed
    EXPECT_EQ(r, false);
}

void TestUnicharDecompose_latin()
{
    static const unichar c = 0xe4; // U+00E4 LATIN SMALL LETTER A WITH DIAERESIS ä
    static const unichar expected[] = {
            0x61, // U+0061 LATIN SMALL LETTER A
            0x308 // U+0308 COMBINING DIAERESIS
    };

    TestUnicharDecompose(c, expected, ARRAY_SIZE(expected));
}

void TestUnicharDecompose_hiragana()
{
    static const unichar c = 0x307a; // U+307A HIRAGANA LETTER PE ペ
    static const unichar expected[] = {
            0x3078, // U+3078 HIRAGANA LETTER HE
            0x309a  // U+309A COMBINING KATAKANA-HIRAGANA SEMI-VOICED SOUND MARK
    };

    TestUnicharDecompose(c, expected, ARRAY_SIZE(expected));
}

/*
 * Due to JP equivalents (see data/decomposition/decomposition.txt),
 * Katakana characters are transformed to their Hiragana equivalents.
 * This is a feature, not a bug.
 */
void TestUnicharDecompose_katakana()
{
    static const unichar c = 0x30da; // U+30DA KATAKANA LETTER PE ペ
    static const unichar expected[] = {
            0x3078, // U+3078 HIRAGANA LETTER HE
            0x309a  // U+309A COMBINING KATAKANA-HIRAGANA SEMI-VOICED SOUND MARK
    };

    TestUnicharDecompose(c, expected, ARRAY_SIZE(expected));
}

void TestUnicharDecompose_circledKatakana()
{
    static const unichar c = 0x32f3; // U+32F3 CIRCLED KATAKANA YA ㋳
    static const unichar expected[] = {
            0x3084  // U+3084 HIRAGANA LETTER YA
    };

    TestUnicharDecompose(c, expected, ARRAY_SIZE(expected));
}

void TestUnicharDecompose_fullwidthLatin()
{
    static const unichar c = 0xff21; // U+FF21 FULLWIDTH LATIN CAPITAL LETTER A
    static const unichar expected[] = {
            0x41  // U+0041 LATIN CAPITAL LETTER A
    };

    TestUnicharDecompose(c, expected, ARRAY_SIZE(expected));
}

void TestUnicharDecompose_fullwidthDigit()
{
    static const unichar c = 0xff11; // U+FF11 FULLWIDTH DIGIT ONE
    static const unichar expected[] = {
            0x31  // U+0031 DIGIT ONE
    };

    TestUnicharDecompose(c, expected, ARRAY_SIZE(expected));
}

void TestUnicharDecompose_fullwidthSymbol()
{
    static const unichar c = 0xff05; // U+FF05 FULLWIDTH PERCENT SIGN
    static const unichar expected[] = {
            0x25  // U+0025 PERCENT SIGN
    };

    TestUnicharDecompose(c, expected, ARRAY_SIZE(expected));
}
