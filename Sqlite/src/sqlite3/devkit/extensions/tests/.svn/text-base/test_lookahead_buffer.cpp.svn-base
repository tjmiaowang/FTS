#include <stddef.h>
#include "test_lookahead_buffer.h"
#include "extensions_test.h"

#include "../collations/utf8_decompose_iterator.h"
#include "../collations/lookahead_buffer.h"

void TestLookaheadBuffer()
{
    static const char AsciiString[] = "123456789";
    static const unsigned Utf8ByteCount = sizeof(AsciiString)/sizeof(AsciiString[0]) -1 /* exclude terminating nul byte */;

    Utf8DecomposeIterator decomposeIt;
    Utf8DecomposeIterator_init(&decomposeIt, AsciiString, Utf8ByteCount);

    LookaheadBuffer buf;
    LookaheadBuffer_init(&buf, &decomposeIt);

    unichar c;
    EXPECT_EQ(true, LookaheadBuffer_peekNextChar(&buf, &c));
    EXPECT_EQ(c, U(AsciiString[0]));

    EXPECT_EQ(true, LookaheadBuffer_peekNextChar(&buf, &c));
    EXPECT_EQ(c, U(AsciiString[1]));

    EXPECT_EQ(true, LookaheadBuffer_peekNextChar(&buf, &c));
    EXPECT_EQ(c, U(AsciiString[2]));

    EXPECT_EQ(true, LookaheadBuffer_getNextChar(&buf, &c));
    EXPECT_EQ(c, U(AsciiString[0]));

    LookaheadBuffer_resetPeekPointer(&buf);

    EXPECT_EQ(true, LookaheadBuffer_getNextChar(&buf, &c));
    EXPECT_EQ(c, U(AsciiString[1]));

    EXPECT_EQ(true, LookaheadBuffer_getNextChar(&buf, &c));
    EXPECT_EQ(c, U(AsciiString[2]));

    EXPECT_EQ(true, LookaheadBuffer_getNextChar(&buf, &c));
    EXPECT_EQ(c, U(AsciiString[3]));
}

void TestLookaheadBuffer_NullArgs()
{
    static const char AsciiString[] = "123456789";
    static const unsigned Utf8ByteCount = sizeof(AsciiString)/sizeof(AsciiString[0]) -1 /* exclude terminating nul byte */;

    Utf8DecomposeIterator decomposeIt;
    Utf8DecomposeIterator_init(&decomposeIt, AsciiString, Utf8ByteCount);

    unichar c;
    LookaheadBuffer buf;

    LookaheadBuffer_init(&buf, NULL);
    EXPECT_EQ(false, LookaheadBuffer_getNextChar(&buf, &c));
    EXPECT_EQ(false, LookaheadBuffer_peekNextChar(&buf, &c));

    LookaheadBuffer_init(&buf, &decomposeIt);
    EXPECT_EQ(false, LookaheadBuffer_getNextChar(&buf, NULL));
    EXPECT_EQ(false, LookaheadBuffer_peekNextChar(&buf, NULL));
}
