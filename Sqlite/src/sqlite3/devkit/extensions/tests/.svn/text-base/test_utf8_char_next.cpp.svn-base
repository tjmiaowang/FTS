#include <stddef.h>
#include <assert.h>

#include "test_utf8_char_next.h"
#include "extensions_test.h"

#include "../utils/utf8_utils.h"

void TestUtf8CharNext()
{
    // Příšerně žluťoučký kůň
    static const char Utf8String[] =
    {
        C(0x50), C(0xc5), C(0x99), C(0xc3), C(0xad), C(0xc5), C(0xa1), C(0x65),
        C(0x72), C(0x6e), C(0xc4), C(0x9b), C(0x20), C(0xc5), C(0xbe), C(0x6c),
        C(0x75), C(0xc5), C(0xa5), C(0x6f), C(0x75), C(0xc4), C(0x8d), C(0x6b),
        C(0xc3), C(0xbd), C(0x20), C(0x6b), C(0xc5), C(0xaf), C(0xc5), C(0x88)
    };
    static const unsigned Utf8ByteCount = sizeof(Utf8String)/sizeof(Utf8String[0]);

    static const unichar UnicodeString[] =
    {
        0x00000050, 0x00000159, 0x000000ed, 0x00000161,
        0x00000065, 0x00000072, 0x0000006e, 0x0000011b,
        0x00000020, 0x0000017e, 0x0000006c, 0x00000075,
        0x00000165, 0x0000006f, 0x00000075, 0x0000010d,
        0x0000006b, 0x000000fd, 0x00000020, 0x0000006b,
        0x0000016f, 0x00000148
    };

    static const unsigned CharCount = sizeof(UnicodeString)/sizeof(UnicodeString[0]);

    char const * s = Utf8String;
    int bytesLeft = Utf8ByteCount;
    unsigned i;
    for (i = 0; i < CharCount; i++)
    {
        unichar c = 0;
        const nds_bool ok = DecodeNextUtf8CharToUnichar(&s, &bytesLeft, &c);
        EXPECT_EQ(true, ok);
        EXPECT_EQ(c, UnicodeString[i]);
    }

    EXPECT_EQ(bytesLeft, 0);
}

void TestUtf8CharNext_NullArgs()
{
    static const char Utf8Simple[] = { 'a' };
    static const unsigned Utf8SimpleByteCount = sizeof(Utf8Simple)/sizeof(Utf8Simple[0]);

    char const* str = Utf8Simple;
    int count = static_cast<int>(Utf8SimpleByteCount);
    unichar c;

    EXPECT_EQ(false, DecodeNextUtf8CharToUnichar(NULL, NULL, NULL));
    EXPECT_EQ(false, DecodeNextUtf8CharToUnichar(NULL, NULL, &c));
    EXPECT_EQ(false, DecodeNextUtf8CharToUnichar(NULL, &count, NULL));
    EXPECT_EQ(false, DecodeNextUtf8CharToUnichar(NULL, &count, &c));
    EXPECT_EQ(false, DecodeNextUtf8CharToUnichar(&str, NULL, NULL));
    EXPECT_EQ(false, DecodeNextUtf8CharToUnichar(&str, NULL, &c));
    EXPECT_EQ(false, DecodeNextUtf8CharToUnichar(&str, &count, NULL));
    EXPECT_EQ(true, DecodeNextUtf8CharToUnichar(&str, &count, &c));
    EXPECT_EQ(U(Utf8Simple[0]), c);
}

void TestUtf8CharNext_InvalidUtf8()
{
    static const char Utf8String[] =
    {
        C(0x50) /* P */, C(0xc5) /* 1st byte of U+0159 */, /* Omitted C(0x99) */
        C(0xc3), C(0xad), C(0xc5), C(0xa1), C(0x65)
    };
    static const unsigned Utf8ByteCount = sizeof(Utf8String)/sizeof(Utf8String[0]);

    char const* str = Utf8String;
    int count = static_cast<int>(Utf8ByteCount);
    unichar c;

    EXPECT_EQ(true, DecodeNextUtf8CharToUnichar(&str, &count, &c));
    EXPECT_EQ(U(Utf8String[0]), c);

    // reading a broken input must fail
    EXPECT_EQ(false, DecodeNextUtf8CharToUnichar(&str, &count, &c));
}

void TestUtf8CharNext_TruncatedInput()
{
    static const char Utf8String[] =
    {
            C(0x61),
            // U+10000 LINEAR B SYLLABLE B008 A: f0 90 80 80
            C(0xf0), C(0x90) // truncated C(0x80), C(0x80)
    };
    static const unsigned Utf8ByteCount = sizeof(Utf8String)/sizeof(Utf8String[0]);

    char const* str = Utf8String;
    int count = static_cast<int>(Utf8ByteCount);
    unichar c;

    EXPECT_EQ(true, DecodeNextUtf8CharToUnichar(&str, &count, &c));
    EXPECT_EQ(U(Utf8String[0]), c);

    // reading a broken input must fail
    EXPECT_EQ(false, DecodeNextUtf8CharToUnichar(&str, &count, &c));
}

static const size_t MAX_UTF8_CHAR_LEN = 4;

void TestUtf8CharNext_Write(unichar c, const char *expected, size_t expectedLen)
{
    assert(expectedLen <= MAX_UTF8_CHAR_LEN);
    char buffer[MAX_UTF8_CHAR_LEN];

    char* p = buffer;
    char* end = buffer+sizeof(buffer);
    EXPECT_EQ(true, WriteUnicharAsUtf8(c, &p, end));
    EXPECT_EQ(p, buffer+expectedLen);
    for (size_t i = 0; i < expectedLen; i++)
    {
        EXPECT_EQ(buffer[i], expected[i]);
    }
}


void TestUtf8CharNext_Write1()
{
    static const unichar c = 0x61; // U+0061 LATIN SMALL LETTER A
    static const char expected[] = {
            C(0x61)
    };

    TestUtf8CharNext_Write(c, expected, sizeof(expected));
}

void TestUtf8CharNext_Write2()
{
    static const unichar c = 0xfc; // U+00FC LATIN SMALL LETTER U WITH DIAERESIS ü
    static const char expected[] = {
            C(0xc3), C(0xbc)
    };

    TestUtf8CharNext_Write(c, expected, sizeof(expected));
}

void TestUtf8CharNext_Write3()
{
    static const unichar c = 0x6771; // U+6771 CJK UNIFIED IDEOGRAPH-6771 東
    static const char expected[] = {
            C(0xe6), C(0x9d), C(0xb1)
    };

    TestUtf8CharNext_Write(c, expected, sizeof(expected));
}

void TestUtf8CharNext_Write4()
{
    static const unichar c = 0x1d11e; // U+1D11E MUSICAL SYMBOL G CLEF 𝄞
    static const char expected[] = {
            C(0xf0), C(0x9d), C(0x84), C(0x9e)
    };

    TestUtf8CharNext_Write(c, expected, sizeof(expected));
}
