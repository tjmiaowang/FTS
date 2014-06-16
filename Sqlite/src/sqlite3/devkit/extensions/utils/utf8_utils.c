#include <stddef.h>
#include <assert.h>

#include "nds_int.h"
#include "unichar.h"
#include "utf8_utils.h"

/* characters encoded in 1 byte (0b0xxxxxxx) */
static const nds_uint8 UTF8_1B_MASK = 0x80;
static const nds_uint8 UTF8_1B_DATA_MASK = (nds_uint8)(~0x80);
static const nds_uint8 UTF8_1B_VALUE = 0x00;

/* characters encoded in 2 bytes (0b110xxxxx) */
static const nds_uint8 UTF8_2B_MASK = 0xe0;
static const nds_uint8 UTF8_2B_DATA_MASK = (nds_uint8)(~0xe0);
static const nds_uint8 UTF8_2B_VALUE = 0xc0;

/* characters encoded in 3 bytes (0b1110xxxx) */
static const nds_uint8 UTF8_3B_MASK = 0xf0;
static const nds_uint8 UTF8_3B_DATA_MASK = (nds_uint8)(~0xf0);
static const nds_uint8 UTF8_3B_VALUE = 0xe0;

/* characters encoded in 4 bytes (0b11110xxx) */
static const nds_uint8 UTF8_4B_MASK = 0xf8;
static const nds_uint8 UTF8_4B_DATA_MASK = (nds_uint8)(~0xf8);
static const nds_uint8 UTF8_4B_VALUE = 0xf0;

/* an utf8 continuation character (0b10xxxxxx) */
static const nds_uint8 UTF8_CONTINUATION_MASK = 0xc0;
static const nds_uint8 UTF8_CONTINUATION_DATA_MASK = (nds_uint8)(~0xc0);
static const nds_uint8 UTF8_CONTINUATION_VALUE = 0x80;
static const unsigned UTF8_CONTINUATION_DATA_BITS = 6;

/* highest codepoint that can be encoded in UTF-8 (as per RFC 3692) */
static const unichar UTF8_MAX_CODEPOINT = 0x10ffff;

static nds_bool Utf8IsContinuationByte(nds_uint8 byte)
{
    return (byte & UTF8_CONTINUATION_MASK) == UTF8_CONTINUATION_VALUE;
}

static int Utf8CharLenValue(nds_uint8 firstByte, unichar* value)
{
    assert(value);
    if (!value)
        return 0;

    if ((firstByte & UTF8_1B_MASK) == UTF8_1B_VALUE)
    {
        *value = firstByte & UTF8_1B_DATA_MASK;
        return 1;
    }

    if ((firstByte & UTF8_2B_MASK) == UTF8_2B_VALUE)
    {
        *value = firstByte & UTF8_2B_DATA_MASK;
        return 2;
    }

    if ((firstByte & UTF8_3B_MASK) == UTF8_3B_VALUE)
    {
        *value = firstByte & UTF8_3B_DATA_MASK;
        return 3;
    }

    if ((firstByte & UTF8_4B_MASK) == UTF8_4B_VALUE)
    {
        *value = firstByte & UTF8_4B_DATA_MASK;
        return 4;
    }

    /* not a valid utf8-encoded string */
    *value = 0;

    return 0;
}

nds_bool DecodeNextUtf8CharToUnichar(char const** pStr, int* strLen, unichar* character)
{
    const char* p;
    unichar uc;
    int charLen;

    if (!pStr || !*pStr || !strLen || *strLen <= 0 || !character)
        return NDS_FALSE;

    p = *pStr;
    charLen = Utf8CharLenValue(*p, &uc);
    if (*strLen < charLen || charLen == 0)
        return NDS_FALSE;

    p++;
    *strLen -= charLen;

    /* decode remaining bytes */
    charLen--; /* 1st byte has already been decoded */
    while (charLen > 0)
    {
        const nds_uint8 currentByte = (nds_uint8)*p;
        if (!Utf8IsContinuationByte(currentByte))
        {
            /* invalid input (not an utf8-encoded string) */
            return NDS_FALSE;
        }
        uc <<= UTF8_CONTINUATION_DATA_BITS;
        uc |= (currentByte & UTF8_CONTINUATION_DATA_MASK);
        p++;
        charLen--;
    }

    *pStr = p;
    *character = uc;
    return NDS_TRUE;
}

nds_bool WriteUnicharAsUtf8(unichar character, char** pStr, const char* pEnd)
{
    int bytesRemaining;

    if (pStr == NULL || *pStr >= pEnd)
        return NDS_FALSE;

    assert(*pStr < pEnd);

    if (character < 0x80)
    {
        /* 1 byte */
        **pStr = (char)character;
        (*pStr)++;
        return NDS_TRUE;
    }
    else if (character < 0x800)
    {
        /* 2 bytes */
        **pStr = UTF8_2B_VALUE | (character >> UTF8_CONTINUATION_DATA_BITS);
        character &= UTF8_CONTINUATION_DATA_MASK;
        bytesRemaining = 1;
    }
    else if (character < 0x10000)
    {
        /* 3 bytes */
        **pStr = UTF8_3B_VALUE | (character >> (2*UTF8_CONTINUATION_DATA_BITS));
        character &= ((UTF8_CONTINUATION_DATA_MASK)<<UTF8_CONTINUATION_DATA_BITS) | UTF8_CONTINUATION_DATA_MASK;
        bytesRemaining = 2;
    }
    else
    {
        /* 4 bytes */
        assert(character < UTF8_MAX_CODEPOINT);
        (void)UTF8_MAX_CODEPOINT; /* supress unused variable warning */
        **pStr = UTF8_4B_VALUE | (character >> (3*UTF8_CONTINUATION_DATA_BITS));
        character &=
                ( (UTF8_CONTINUATION_DATA_MASK<<UTF8_CONTINUATION_DATA_BITS) | UTF8_CONTINUATION_DATA_MASK
                ) << UTF8_CONTINUATION_DATA_BITS | UTF8_CONTINUATION_DATA_MASK;
        bytesRemaining = 3;
    }
    (*pStr)++;

    while (bytesRemaining > 0 && *pStr < pEnd)
    {
        const unsigned shift = UTF8_CONTINUATION_DATA_BITS*(bytesRemaining-1);
        **pStr = UTF8_CONTINUATION_VALUE | (character >> shift);
        (*pStr)++;
        character &= ~(UTF8_CONTINUATION_DATA_MASK << shift);
        bytesRemaining--;
    }

    return bytesRemaining == 0;
}

unsigned GetUtf8StringCharCount(char const* str, int strLen)
{
    unsigned charCount = 0;

    if (str == NULL)
        return 0;

    while (strLen > 0)
    {
        if (!Utf8IsContinuationByte((nds_uint8)(*str)))
        {
            charCount++;
        }
        ++str;
        --strLen;
    }

    return charCount;
}

nds_bool IsUtf8ContinuationByte(char c)
{
    return (c & UTF8_CONTINUATION_MASK) == UTF8_CONTINUATION_VALUE;
}
