#include <stddef.h>
#include "test_utf8_decompose_iterator.h"
#include "extensions_test.h"

#include "../collations/utf8_decompose_iterator.h"

void TestUtf8DecomposeIterator()
{
    static const char Utf8String[] =
    {
        C(0x50), C(0xc5), C(0x99), C(0xc3), C(0xad), C(0xc5), C(0xa1), C(0x65),
        C(0x72), C(0x6e), C(0xc4), C(0x9b), C(0x20), C(0xc5), C(0xbe), C(0x6c),
        C(0x75), C(0xc5), C(0xa5), C(0x6f), C(0x75), C(0xc4), C(0x8d), C(0x6b),
        C(0xc3), C(0xbd), C(0x20), C(0x6b), C(0xc5), C(0xaf), C(0xc5), C(0x88)
    };
    static const unsigned Utf8ByteCount = sizeof(Utf8String)/sizeof(Utf8String[0]);

    static const unichar DecomposedUnicodeString[] =
    {
        0x00000050, 0x00000072, 0x0000030c, 0x00000069,
        0x00000301, 0x00000073, 0x0000030c, 0x00000065,
        0x00000072, 0x0000006e, 0x00000065, 0x0000030c,
        0x00000020, 0x0000007a, 0x0000030c, 0x0000006c,
        0x00000075, 0x00000074, 0x0000030c, 0x0000006f,
        0x00000075, 0x00000063, 0x0000030c, 0x0000006b,
        0x00000079, 0x00000301, 0x00000020, 0x0000006b,
        0x00000075, 0x0000030a, 0x0000006e, 0x0000030c
    };

    static const unsigned CharCount = sizeof(DecomposedUnicodeString)/sizeof(DecomposedUnicodeString[0]);

    Utf8DecomposeIterator it;
    Utf8DecomposeIterator_init(&it, Utf8String, Utf8ByteCount);

    unichar c = 0;
    unsigned i = 0;
    while (Utf8DecomposeIterator_getNextCharacter(&it, &c))
    {
        EXPECT_EQ(true, i < Utf8ByteCount);
        EXPECT_EQ(c, DecomposedUnicodeString[i]);
        i++;
    }

    EXPECT_EQ(i, CharCount);
}

void TestUtf8DecomposeIterator_NullArgs()
{
    static const char Utf8Simple[] = { 'a' };
    static const unsigned Utf8SimpleByteCount = sizeof(Utf8Simple)/sizeof(Utf8Simple[0]);

    unichar c;
    Utf8DecomposeIterator it;

    Utf8DecomposeIterator_init(&it, NULL, 0);
    EXPECT_EQ(false, Utf8DecomposeIterator_getNextCharacter(&it, &c));

    Utf8DecomposeIterator_init(&it, NULL, -1);
    EXPECT_EQ(false, Utf8DecomposeIterator_getNextCharacter(&it, &c));

    Utf8DecomposeIterator_init(&it, NULL, Utf8SimpleByteCount);
    EXPECT_EQ(false, Utf8DecomposeIterator_getNextCharacter(&it, &c));

    Utf8DecomposeIterator_init(&it, Utf8Simple, 0);
    EXPECT_EQ(false, Utf8DecomposeIterator_getNextCharacter(&it, &c));

    Utf8DecomposeIterator_init(&it, Utf8Simple, -1);
    EXPECT_EQ(false, Utf8DecomposeIterator_getNextCharacter(&it, &c));
}

void TestUtf8DecomposeIterator_InvalidUtf8()
{
    static const char Utf8String[] =
    {
        C(0x50) /* P */, C(0xc5) /* 1st byte of U+0159 */, /* Ommitted C(0x99) */
        C(0xc3), C(0xad), C(0xc5), C(0xa1), C(0x65)
    };
    static const unsigned Utf8ByteCount = sizeof(Utf8String)/sizeof(Utf8String[0]);

    unichar c;
    Utf8DecomposeIterator it;
    Utf8DecomposeIterator_init(&it, Utf8String, Utf8ByteCount);

    EXPECT_EQ(true, Utf8DecomposeIterator_getNextCharacter(&it, &c));
    EXPECT_EQ(U(Utf8String[0]), c);

    // reading a broken input must fail
    EXPECT_EQ(false, Utf8DecomposeIterator_getNextCharacter(&it, &c));
}

void TestUtf8DecomposeIterator_TruncatedInput()
{
    static const char Utf8String[] =
    {
            C(0x61),
            // U+10000 LINEAR B SYLLABLE B008 A: f0 90 80 80
            C(0xf0), C(0x90) // truncated C(0x80), C(0x80)
    };
    static const unsigned Utf8ByteCount = sizeof(Utf8String)/sizeof(Utf8String[0]);

    unichar c;
    Utf8DecomposeIterator it;
    Utf8DecomposeIterator_init(&it, Utf8String, Utf8ByteCount);

    EXPECT_EQ(true, Utf8DecomposeIterator_getNextCharacter(&it, &c));
    EXPECT_EQ(U(Utf8String[0]), c);

    // reading a broken input must fail
    EXPECT_EQ(false, Utf8DecomposeIterator_getNextCharacter(&it, &c));
}
