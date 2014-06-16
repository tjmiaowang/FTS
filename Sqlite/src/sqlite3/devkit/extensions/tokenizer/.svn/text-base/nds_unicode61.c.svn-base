#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "../utils/nds_bool.h"
#include "../utils/unichar.h"
#include "../utils/unichar_utils.h"
#include "../utils/utf8_utils.h"
#include "nds_unicode61.h"
#include "string_buffer.h"

/**
 * Default value for the remove_diacritics option.
 *
 * Unicode61 tokenizer does remove diacritics when the option is not specified
 * and this code should match that.
 */
static const nds_bool NDSUNICODE61_REMOVE_DIACRITICS_DEFAULT = NDS_TRUE;

/**
 * Structure holding the state of the NDS unicode61 tokenizer.
 */
typedef struct
{
    sqlite3_tokenizer base; /* this needs to come first */

    nds_bool removeDiacritics;

    /**
     * Exceptions to NdsUnicode61_isTokenChar().
     *
     * This is a sorted array of unicode characters that are to be treated differently
     * than what NdsUnicode61_isTokenChar() returns. This is filled based on the value
     * of tokenchar and separators arguments.
     */
    unichar *alnumExceptions;

    /**
     * Number of entries in the exceptions array.
     */
    size_t alnumExceptionsCount;
} NdsUnicode61;

/**
 * State of the NDS unicode61 tokenizer cursor.
 */
typedef enum
{
    Unknown = 0,

    /**
     * Current character is not covered by the bigram algorithm.
     */
    Latin,

    /**
     * Current character is covered by the bigram algorithm. It's the first such
     * character after token start of Latin.
     */
    BigramStart,

    /**
     * Current character is covered by the bigram algorithm. It follows another character
     * covered by the bigram algorithm.
     */
    Bigram
} BigramTokenizerState;

/**
 * Structure holding the state of the NDS unicode61 tokenizer's cursor.
 */
typedef struct
{
    sqlite3_tokenizer_cursor base;

    /**
     * The raw input.
     */
    char* inputStart;

    /**
     * One-past the last byte of the input.
     */
    const char* inputEnd;

    /**
     * State of the cursor.
     */
    BigramTokenizerState state;

    /**
     * Pointer to the current character.
     *
     * This is either NULL before any character is read, or inputEnd when
     * all characters have been read, or a value in the range in inputStart..inputEnd-1.
     */
    const char* current;

    /**
     * The decoded unicode character that is located at current (if current is valid).
     */
    unichar currentCharacter;

    /**
     * Pointer to the next character.
     *
     * This is either NULL before a keyword is retrieved, or inputEnd when
     * all characters have been read, or a value in the range inputStart..inputEnd-1.
     */
    const char* next;

    /**
     * Index of the last returned token.
     */
    int index;

    /**
     * Buffer for storing the current bigram, if any.
     */
    StringBuffer token;

    /**
     * In Bigram state:
     * Number of bytes in token that is occupied by the first character of the current bigram.
     *
     * This is used to quickly kill off the first character when a new bigram is generated.
     * (So that "12" becomes "23".)
     */
    size_t bigramFirstCharSize;

    /**
     * In Bigram state:
     * Offset (in bytes) from the start of the input that the first character
     * in the token buffer starts at.
     *
     * This is used to be able to return the initial offset (in the input stream) of the
     * current bigram.
     */
    int bigramOffset1;

    /**
     * In Bigram state:
     * Offset (in bytes) from the start of the input that the second character
     * in the token buffer starts at.
     *
     * This is used when shifting characters in the buffer ("12" -> "23), as the new value
     * for bigramOffset1.
     */
    int bigramOffset2;
} NdsUnicode61Cursor;

static int NdsUnicode61_create(int argc, char const* const* argv, sqlite3_tokenizer** ppTokenizer);
static int NdsUnicode61_destroy(sqlite3_tokenizer *pTokenizer);
static int NdsUnicode61_open(sqlite3_tokenizer* pTokenizer, const char* pInput, int nBytes,
        sqlite3_tokenizer_cursor** ppCursor);
static int NdsUnicode61_close(sqlite3_tokenizer_cursor* pCursor);
static int NdsUnicode61_next(sqlite3_tokenizer_cursor* pCursor, const char** ppToken, int* pnBytes,
        int* piStartOffset, int* piEndOffset, int* piPosition);

/**
 * Add a string of characters into the exceptions array.
 *
 * \param[in] self       Instance of the tokenizer module.
 * \param[in] tokenChars NDS_TRUE if chars are to be treated as token chars, NDS_FALSE if they
 *                       are to be treated as separators.
 * \param[in] chars      Nul-terminated UTF8-encoded string of characters to be processed.
 * \return SQLITE_OK when operation succeeded, an error code otherwise.
 */
static int NdsUnicode61_addExceptions(NdsUnicode61* self, nds_bool tokenChars, const char* chars);

/**
 * Parse the arguments passed.
 *
 * The ndsunicode61 tokenizer recognizes the following options:
 * remove_diacritics=VALUE where VALUE is either 1 or 0 - enables or disables diacritic stripping
 * tokenchars=STRING - turns characters from STRING into token characters (not separators)
 * separators=STRING - turns characters from STRING into separators
 *
 * The behaviour of tokenchars and separators should mimic the behaviour of these in the unicode61
 * tokenizer.
 *
 * \return SQLITE_OK when operation succeeded, an error code otherwise.
 */
static int NdsUnicode61_parseArguments(NdsUnicode61* self, int argc, char const* const* argv)
{
    static const char REMOVE_DIACRITICS[] = "remove_diacritics=";
    static const char TOKENCHARS[] = "tokenchars=";
    static const char SEPARATORS[] = "separators=";
    int idx;

    self->removeDiacritics = NDSUNICODE61_REMOVE_DIACRITICS_DEFAULT;
    self->alnumExceptions = NULL;
    self->alnumExceptionsCount = 0;

    for (idx = 0; idx < argc; idx++)
    {
        if (strncmp(argv[idx], REMOVE_DIACRITICS, ARRAY_SIZE(REMOVE_DIACRITICS) - 1) == 0)
        {
            int removeDiacritics;
            if (sscanf(argv[idx] + ARRAY_SIZE(REMOVE_DIACRITICS) - 1, "%d",
                    &removeDiacritics) == 1)
            {
                self->removeDiacritics = removeDiacritics != 0;
            }
            else
            {
                /* can't parse as number - fail */
                return SQLITE_ERROR;
            }
        }
        else if (strncmp(argv[idx], TOKENCHARS, ARRAY_SIZE(TOKENCHARS) - 1) == 0)
        {
            int r = NdsUnicode61_addExceptions(self, NDS_TRUE, argv[idx] + ARRAY_SIZE(TOKENCHARS) - 1);
            if (r != SQLITE_OK)
                return r;
        }
        else if (strncmp(argv[idx], SEPARATORS, ARRAY_SIZE(SEPARATORS) - 1) == 0)
        {
            int r = NdsUnicode61_addExceptions(self, NDS_FALSE, argv[idx] + ARRAY_SIZE(TOKENCHARS) - 1);
            if (r != SQLITE_OK)
                return r;
        }
        else
        {
            /* unknown option */
            return SQLITE_ERROR;
        }
    }

    return SQLITE_OK;
}

/**
 * Create an instance of the NDS unicode61 tokenizer.
 */
int NdsUnicode61_create(int argc, char const* const* argv, sqlite3_tokenizer** ppTokenizer)
{
    NdsUnicode61* self;
    int result;

    self = (NdsUnicode61*)malloc(sizeof(NdsUnicode61));
    if (self == NULL)
        return SQLITE_NOMEM;

    memset(self, 0, sizeof(*self));
    result = NdsUnicode61_parseArguments(self, argc, argv);
    if (result != SQLITE_OK)
    {
        NdsUnicode61_destroy(&self->base);
        return result;
    }

    *ppTokenizer = &self->base;
    return SQLITE_OK;
}

/**
 * Destroy an instance of the NDS unicode61 tokenizer.
 */
int NdsUnicode61_destroy(sqlite3_tokenizer* pTokenizer)
{
    NdsUnicode61* self;
    int result;

    if (pTokenizer == NULL)
        return SQLITE_ERROR;

    result = SQLITE_OK;

    self = (NdsUnicode61*)pTokenizer;
    free(self);

    return result;
}

/**
 * Create an instance of the NDS unicode61 tokenizer cursor.
 */
int NdsUnicode61_open(sqlite3_tokenizer* pTokenizer, const char* pInput, int nBytes,
        sqlite3_tokenizer_cursor **ppCursor)
{
    NdsUnicode61Cursor* cursor;
    size_t len;

    if (pTokenizer == NULL)
        return SQLITE_ERROR;

    cursor = (NdsUnicode61Cursor*)malloc(sizeof(NdsUnicode61Cursor));
    if (cursor == NULL)
    {
        return SQLITE_NOMEM;
    }
    memset(cursor, 0, sizeof(*cursor));

    StringBuffer_init(&cursor->token);

    /* store a copy of the passed argument */
    len = (nBytes >= 0) ? (size_t)nBytes : strlen(pInput);
    if (len > 0)
    {
        cursor->inputStart = (char*)malloc(len);
        if (cursor->inputStart == NULL)
        {
            NdsUnicode61_close(&cursor->base);
            return SQLITE_NOMEM;
        }
        memcpy(cursor->inputStart, pInput, len);
        cursor->inputEnd = cursor->inputStart + len;
    }
    else
    {
        cursor->inputStart = NULL;
        cursor->inputEnd = NULL;
    }

    cursor->next = cursor->inputStart;
    cursor->current = NULL;

    *ppCursor = &cursor->base;

    return SQLITE_OK;
}

/**
 * Destroy an instance of the NDS unicode61 tokenizer cursor.
 */
int NdsUnicode61_close(sqlite3_tokenizer_cursor* pCursor)
{
    NdsUnicode61Cursor* cursor;

    if (pCursor == NULL)
        return SQLITE_ERROR;

    cursor = (NdsUnicode61Cursor*)pCursor;

    StringBuffer_destroy(&cursor->token);
    free(cursor->inputStart);
    free(cursor);

    return SQLITE_OK;
}

/**
 * Retrieve the NdsUnicode61 tokenizer module instance from a NdsUnicode61Cursor instance.
 */
static NdsUnicode61* NdsUnicode61_getModulefromCursor(NdsUnicode61Cursor* cursor)
{
    return (NdsUnicode61*)cursor->base.pTokenizer;
}

/**
 * Return true if given character is a diacritic character.
 */
static nds_bool NdsUnicode61_isDiacritic(unichar character)
{
    /* TODO: How should non-combining JP diacritics (U+ff9e, U+ff9f) be tokenized? */
    return UnicharIsCombiningMark(character);
}

/**
 * A check if given character should be processed by the bigram algorithm, or
 * treated as ASCII words.
 *
 * \return true iff bigram algorithm should be used for the character.
 */
static nds_bool NdsUnicode61_isBigrammable(unichar c)
{
    return
            (c >= 0xe00 && c < 0xf00) || /* Thai, Lao */
            (c >= 0x1780 && c < 0x1800) || /* Khmer */
            c > 0x2000; /* CJK, but also many others... TODO: clarify */
}

/**
 * Reset the state of the cursor in an invalid state.
 *
 * This is used after an error.
 */
static void NdsUnicode61_resetCursor(NdsUnicode61Cursor* cursor)
{
    cursor->current = cursor->next = NULL;
    cursor->currentCharacter = 0;
    cursor->state = Unknown;
}

static nds_bool NdsUnicode61_isEnd(NdsUnicode61Cursor const* cursor)
{
    return cursor->current == cursor->inputEnd;
}

/*
 * Read next unicode character from the input.
 *
 * \param[in,out] cursor The tokenizer's cursor. The reading starts from cursor->next. The value is adapted
 *                       to point to the next character if the read succeeded. The currently-read characted
 *                       is placed in cursor->currentCharacter. The address from which it has been read is
 *                       placed in cursor->current.
 *
 * \return true iff a character has been read.
 *
 * \note When false is returned, it means either there's no more data to read, or input is not a valid utf8
 * string. To distinguish these, the caller can check cursor->current. If cursor->current != cursor->inputEnd
 * and false has been returned, the cause must have been an invalid utf8 input. If cursor->current == cursor->inputEnd
 * all characters have been read.
 */
static nds_bool NdsUnicode61_getNextChar(NdsUnicode61Cursor* cursor)
{
    const char* current;
    int bytesLeft;

    if (cursor->next == cursor->inputEnd)
    {
        cursor->current = cursor->inputEnd;
        return NDS_FALSE;
    }

    current = cursor->next;
    bytesLeft = cursor->inputEnd - cursor->next;
    if (DecodeNextUtf8CharToUnichar(&cursor->next, &bytesLeft, &cursor->currentCharacter))
    {
        cursor->current = current;
        return NDS_TRUE;
    }

    return NDS_FALSE;
}

/*
 * Copied from SQLite.
*/
static int remove_diacritic(int c)
{
  unsigned short aDia[] = {
        0,  1797,  1848,  1859,  1891,  1928,  1940,  1995,
     2024,  2040,  2060,  2110,  2168,  2206,  2264,  2286,
     2344,  2383,  2472,  2488,  2516,  2596,  2668,  2732,
     2782,  2842,  2894,  2954,  2984,  3000,  3028,  3336,
     3456,  3696,  3712,  3728,  3744,  3896,  3912,  3928,
     3968,  4008,  4040,  4106,  4138,  4170,  4202,  4234,
     4266,  4296,  4312,  4344,  4408,  4424,  4472,  4504,
     6148,  6198,  6264,  6280,  6360,  6429,  6505,  6529,
    61448, 61468, 61534, 61592, 61642, 61688, 61704, 61726,
    61784, 61800, 61836, 61880, 61914, 61948, 61998, 62122,
    62154, 62200, 62218, 62302, 62364, 62442, 62478, 62536,
    62554, 62584, 62604, 62640, 62648, 62656, 62664, 62730,
    62924, 63050, 63082, 63274, 63390,
  };
  char aChar[] = {
    '\0', 'a',  'c',  'e',  'i',  'n',  'o',  'u',  'y',  'y',  'a',  'c',
    'd',  'e',  'e',  'g',  'h',  'i',  'j',  'k',  'l',  'n',  'o',  'r',
    's',  't',  'u',  'u',  'w',  'y',  'z',  'o',  'u',  'a',  'i',  'o',
    'u',  'g',  'k',  'o',  'j',  'g',  'n',  'a',  'e',  'i',  'o',  'r',
    'u',  's',  't',  'h',  'a',  'e',  'o',  'y',  '\0', '\0', '\0', '\0',
    '\0', '\0', '\0', '\0', 'a',  'b',  'd',  'd',  'e',  'f',  'g',  'h',
    'h',  'i',  'k',  'l',  'l',  'm',  'n',  'p',  'r',  'r',  's',  't',
    'u',  'v',  'w',  'w',  'x',  'y',  'z',  'h',  't',  'w',  'y',  'a',
    'e',  'i',  'o',  'u',  'y',
  };

  unsigned int key = (((unsigned int)c)<<3) | 0x00000007;
  int iRes = 0;
  int iHi = sizeof(aDia)/sizeof(aDia[0]) - 1;
  int iLo = 0;
  while( iHi>=iLo ){
    int iTest = (iHi + iLo) / 2;
    if( key >= aDia[iTest] ){
      iRes = iTest;
      iLo = iTest+1;
    }else{
      iHi = iTest-1;
    }
  }
  assert( key>=aDia[iRes] );
  return ((c > (aDia[iRes]>>3) + (aDia[iRes]&0x07)) ? c : (int)aChar[iRes]);
}

/*
 * Copied from SQLite.
*/
int sqlite3FtsUnicodeFold(int c, int bRemoveDiacritic)
{
  /* Each entry in the following array defines a rule for folding a range
  ** of codepoints to lower case. The rule applies to a range of nRange
  ** codepoints starting at codepoint iCode.
  **
  ** If the least significant bit in flags is clear, then the rule applies
  ** to all nRange codepoints (i.e. all nRange codepoints are upper case and
  ** need to be folded). Or, if it is set, then the rule only applies to
  ** every second codepoint in the range, starting with codepoint C.
  **
  ** The 7 most significant bits in flags are an index into the aiOff[]
  ** array. If a specific codepoint C does require folding, then its lower
  ** case equivalent is ((C + aiOff[flags>>1]) & 0xFFFF).
  **
  ** The contents of this array are generated by parsing the CaseFolding.txt
  ** file distributed as part of the "Unicode Character Database". See
  ** http://www.unicode.org for details.
  */
  static const struct TableEntry {
    unsigned short iCode;
    unsigned char flags;
    unsigned char nRange;
  } aEntry[] = {
    {65, 14, 26},          {181, 64, 1},          {192, 14, 23},
    {216, 14, 7},          {256, 1, 48},          {306, 1, 6},
    {313, 1, 16},          {330, 1, 46},          {376, 116, 1},
    {377, 1, 6},           {383, 104, 1},         {385, 50, 1},
    {386, 1, 4},           {390, 44, 1},          {391, 0, 1},
    {393, 42, 2},          {395, 0, 1},           {398, 32, 1},
    {399, 38, 1},          {400, 40, 1},          {401, 0, 1},
    {403, 42, 1},          {404, 46, 1},          {406, 52, 1},
    {407, 48, 1},          {408, 0, 1},           {412, 52, 1},
    {413, 54, 1},          {415, 56, 1},          {416, 1, 6},
    {422, 60, 1},          {423, 0, 1},           {425, 60, 1},
    {428, 0, 1},           {430, 60, 1},          {431, 0, 1},
    {433, 58, 2},          {435, 1, 4},           {439, 62, 1},
    {440, 0, 1},           {444, 0, 1},           {452, 2, 1},
    {453, 0, 1},           {455, 2, 1},           {456, 0, 1},
    {458, 2, 1},           {459, 1, 18},          {478, 1, 18},
    {497, 2, 1},           {498, 1, 4},           {502, 122, 1},
    {503, 134, 1},         {504, 1, 40},          {544, 110, 1},
    {546, 1, 18},          {570, 70, 1},          {571, 0, 1},
    {573, 108, 1},         {574, 68, 1},          {577, 0, 1},
    {579, 106, 1},         {580, 28, 1},          {581, 30, 1},
    {582, 1, 10},          {837, 36, 1},          {880, 1, 4},
    {886, 0, 1},           {902, 18, 1},          {904, 16, 3},
    {908, 26, 1},          {910, 24, 2},          {913, 14, 17},
    {931, 14, 9},          {962, 0, 1},           {975, 4, 1},
    {976, 140, 1},         {977, 142, 1},         {981, 146, 1},
    {982, 144, 1},         {984, 1, 24},          {1008, 136, 1},
    {1009, 138, 1},        {1012, 130, 1},        {1013, 128, 1},
    {1015, 0, 1},          {1017, 152, 1},        {1018, 0, 1},
    {1021, 110, 3},        {1024, 34, 16},        {1040, 14, 32},
    {1120, 1, 34},         {1162, 1, 54},         {1216, 6, 1},
    {1217, 1, 14},         {1232, 1, 88},         {1329, 22, 38},
    {4256, 66, 38},        {4295, 66, 1},         {4301, 66, 1},
    {7680, 1, 150},        {7835, 132, 1},        {7838, 96, 1},
    {7840, 1, 96},         {7944, 150, 8},        {7960, 150, 6},
    {7976, 150, 8},        {7992, 150, 8},        {8008, 150, 6},
    {8025, 151, 8},        {8040, 150, 8},        {8072, 150, 8},
    {8088, 150, 8},        {8104, 150, 8},        {8120, 150, 2},
    {8122, 126, 2},        {8124, 148, 1},        {8126, 100, 1},
    {8136, 124, 4},        {8140, 148, 1},        {8152, 150, 2},
    {8154, 120, 2},        {8168, 150, 2},        {8170, 118, 2},
    {8172, 152, 1},        {8184, 112, 2},        {8186, 114, 2},
    {8188, 148, 1},        {8486, 98, 1},         {8490, 92, 1},
    {8491, 94, 1},         {8498, 12, 1},         {8544, 8, 16},
    {8579, 0, 1},          {9398, 10, 26},        {11264, 22, 47},
    {11360, 0, 1},         {11362, 88, 1},        {11363, 102, 1},
    {11364, 90, 1},        {11367, 1, 6},         {11373, 84, 1},
    {11374, 86, 1},        {11375, 80, 1},        {11376, 82, 1},
    {11378, 0, 1},         {11381, 0, 1},         {11390, 78, 2},
    {11392, 1, 100},       {11499, 1, 4},         {11506, 0, 1},
    {42560, 1, 46},        {42624, 1, 24},        {42786, 1, 14},
    {42802, 1, 62},        {42873, 1, 4},         {42877, 76, 1},
    {42878, 1, 10},        {42891, 0, 1},         {42893, 74, 1},
    {42896, 1, 4},         {42912, 1, 10},        {42922, 72, 1},
    {65313, 14, 26},
  };
  static const unsigned short aiOff[] = {
   1,     2,     8,     15,    16,    26,    28,    32,
   37,    38,    40,    48,    63,    64,    69,    71,
   79,    80,    116,   202,   203,   205,   206,   207,
   209,   210,   211,   213,   214,   217,   218,   219,
   775,   7264,  10792, 10795, 23228, 23256, 30204, 54721,
   54753, 54754, 54756, 54787, 54793, 54809, 57153, 57274,
   57921, 58019, 58363, 61722, 65268, 65341, 65373, 65406,
   65408, 65410, 65415, 65424, 65436, 65439, 65450, 65462,
   65472, 65476, 65478, 65480, 65482, 65488, 65506, 65511,
   65514, 65521, 65527, 65528, 65529,
  };

  int ret = c;

  assert( c>=0 );
  assert( sizeof(unsigned short)==2 && sizeof(unsigned char)==1 );

  if( c<128 ){
    if( c>='A' && c<='Z' ) ret = c + ('a' - 'A');
  }else if( c<65536 ){
    int iHi = sizeof(aEntry)/sizeof(aEntry[0]) - 1;
    int iLo = 0;
    int iRes = -1;

    while( iHi>=iLo ){
      int iTest = (iHi + iLo) / 2;
      int cmp = (c - aEntry[iTest].iCode);
      if( cmp>=0 ){
        iRes = iTest;
        iLo = iTest+1;
      }else{
        iHi = iTest-1;
      }
    }
    assert( iRes<0 || c>=aEntry[iRes].iCode );

    if( iRes>=0 ){
      const struct TableEntry *p = &aEntry[iRes];
      if( c<(p->iCode + p->nRange) && 0==(0x01 & p->flags & (p->iCode ^ c)) ){
        ret = (c + (aiOff[p->flags>>1])) & 0x0000FFFF;
        assert( ret>0 );
      }
    }

    if( bRemoveDiacritic ) ret = remove_diacritic(ret);
  }

  else if( c>=66560 && c<66600 ){
    ret = c + 40;
  }

  return ret;
}

/**
 * Check is a given character can be a part of a token.
 *
 * \return True if the character can be a part of a token, false if it's a separator.
 */
static nds_bool NdsUnicode61_isAlNum(unichar c)
{
    /*
     * Code taken from sqlite3FtsUnicodeIsalnum() but values updated
     * using the tools/tokenizer/isalnum_codec.py script.
     * The values specify ranges of what is NOT a token character.
     */
    static const unsigned int aEntry[] =
    {
            0x00000030, 0x0000e807, 0x00016c06, 0x0001ec2f, 0x0002ac07,
            0x0002d001, 0x0002d803, 0x0002ec01, 0x0002fc01, 0x00035c01,
            0x0003dc01, 0x000b0804, 0x000b480e, 0x000b9407, 0x000bb401,
            0x000bbc11, 0x000dd401, 0x000df801, 0x000e1002, 0x000e1c01,
            0x000fd801, 0x00120808, 0x00156806, 0x00162402, 0x00163c01,
            0x00164437, 0x0017cc02, 0x00180005, 0x00181816, 0x00187802,
            0x00192c15, 0x0019a804, 0x0019c001, 0x001b5001, 0x001b580f,
            0x001b9c07, 0x001bf402, 0x001c000e, 0x001c3c01, 0x001c4401,
            0x001cc01b, 0x001e980b, 0x001fac09, 0x001fd804, 0x00205804,
            0x00206c09, 0x00209403, 0x0020a405, 0x0020c00f, 0x00216403,
            0x00217801, 0x0023901b, 0x00240004, 0x0024e803, 0x0024f812,
            0x00254407, 0x00258804, 0x0025c001, 0x00260403, 0x0026f001,
            0x0026f807, 0x00271c02, 0x00272c03, 0x00275c01, 0x00278802,
            0x0027c802, 0x0027e802, 0x00280403, 0x0028f001, 0x0028f805,
            0x00291c02, 0x00292c03, 0x00294401, 0x0029c002, 0x0029d401,
            0x002a0403, 0x002af001, 0x002af808, 0x002b1c03, 0x002b2c03,
            0x002b8802, 0x002bc002, 0x002c0403, 0x002cf001, 0x002cf807,
            0x002d1c02, 0x002d2c03, 0x002d5802, 0x002d8802, 0x002dc001,
            0x002e0801, 0x002ef805, 0x002f1803, 0x002f2804, 0x002f5c01,
            0x002fcc08, 0x00300403, 0x0030f807, 0x00311803, 0x00312804,
            0x00315402, 0x00318802, 0x0031fc01, 0x00320802, 0x0032f001,
            0x0032f807, 0x00331803, 0x00332804, 0x00335402, 0x00338802,
            0x00340802, 0x0034f807, 0x00351803, 0x00352804, 0x00355c01,
            0x00358802, 0x0035e401, 0x00360802, 0x00372801, 0x00373c06,
            0x00375801, 0x00376008, 0x0037c803, 0x0038bc01, 0x00391801,
            0x00393c01, 0x00396802, 0x003ac401, 0x003ad006, 0x003aec02,
            0x003b2006, 0x003c041f, 0x003cd00c, 0x003dc417, 0x003e340b,
            0x003e6424, 0x003ef80f, 0x003f380d, 0x0040ac14, 0x00412806,
            0x00415804, 0x00417803, 0x00418803, 0x00419c07, 0x0041c404,
            0x0042080c, 0x00423c01, 0x00426806, 0x0043ec01, 0x004d740c,
            0x004e400a, 0x00500001, 0x0059b402, 0x005a0001, 0x005a6c02,
            0x005bac03, 0x005c4803, 0x005cc805, 0x005d4802, 0x005dc802,
            0x005ed023, 0x005f6004, 0x005f7401, 0x0060000f, 0x0062a401,
            0x0064800c, 0x0064c00c, 0x00650001, 0x00651002, 0x0066c011,
            0x00672002, 0x00677822, 0x00685c05, 0x00687802, 0x0069540a,
            0x0069801d, 0x0069fc01, 0x006a8007, 0x006aa006, 0x006c0005,
            0x006cd011, 0x006d6823, 0x006e0003, 0x006e840d, 0x006f980e,
            0x006ff004, 0x00709014, 0x0070ec05, 0x0071f802, 0x00730008,
            0x00734019, 0x0073b401, 0x0073c803, 0x007ef401, 0x007efc03,
            0x007f3403, 0x007f7403, 0x007fb403, 0x007ff402, 0x00800065,
            0x0081a806, 0x0081e805, 0x00822805, 0x0082801a, 0x00840002,
            0x00840c04, 0x00842002, 0x00845001, 0x00845803, 0x00847806,
            0x00849401, 0x00849c01, 0x0084a401, 0x0084b801, 0x0084e802,
            0x00850005, 0x00852804, 0x00853c01, 0x00864264, 0x00900027,
            0x0091000b, 0x00940200, 0x009c0475, 0x009e53b9, 0x00ad400a,
            0x00b39406, 0x00b3bc03, 0x00b3e404, 0x00b3f802, 0x00b5c001,
            0x00b5fc01, 0x00b7804f, 0x00b8c00c, 0x00ba001a, 0x00ba6c59,
            0x00bc00d6, 0x00bfc00c, 0x00c00005, 0x00c02019, 0x00c0a807,
            0x00c0d802, 0x00c0f403, 0x00c28001, 0x00c3ec01, 0x00c64002,
            0x00c6580a, 0x00c70024, 0x00c94001, 0x00c9fc01, 0x00cb3004,
            0x00cc0100, 0x01370040, 0x02924037, 0x0293f802, 0x02983403,
            0x0299bc10, 0x029a7c01, 0x029bc008, 0x029c0017, 0x029c8002,
            0x029e2402, 0x02a00801, 0x02a01801, 0x02a02c01, 0x02a08c09,
            0x02a0d804, 0x02a1d004, 0x02a20002, 0x02a2d011, 0x02a33802,
            0x02a38012, 0x02a3e003, 0x02a4980a, 0x02a51c0d, 0x02a57c01,
            0x02a60004, 0x02a6cc1b, 0x02a77802, 0x02a8a40e, 0x02a90c01,
            0x02a93002, 0x02a97004, 0x02a9dc03, 0x02a9ec01, 0x02aac001,
            0x02aac803, 0x02aadc02, 0x02aaf802, 0x02ab0401, 0x02ab7802,
            0x02abac07, 0x02abd402, 0x02af8c0b, 0x03600001, 0x036dfc02,
            0x036ffc02, 0x037ffc01, 0x03ec7801, 0x03eca401, 0x03eec810,
            0x03f4f802, 0x03f7f002, 0x03f8001a, 0x03f8c023, 0x03f95013,
            0x03f9a004, 0x03fbfc01, 0x03fc040f, 0x03fc6807, 0x03fcec06,
            0x03fd6c0b, 0x03ff8007, 0x03ffa007, 0x03ffe405, 0x04040003,
            0x0404dc09, 0x0405e411, 0x0406400c, 0x0407402e, 0x040e7c01,
            0x040f4001, 0x04215c01, 0x04247c01, 0x0424fc01, 0x04280403,
            0x04281402, 0x04283004, 0x0428e003, 0x0428fc01, 0x04294009,
            0x0429fc01, 0x042ce407, 0x04400003, 0x0440e016, 0x04420003,
            0x0442c012, 0x04440003, 0x04449c0e, 0x04450004, 0x04460003,
            0x0446cc0e, 0x04471404, 0x045aac0d, 0x0491c004, 0x05bd442e,
            0x05be3c04, 0x074000f6, 0x07440027, 0x0744a4b5, 0x07480046,
            0x074c0057, 0x075b0401, 0x075b6c01, 0x075bec01, 0x075c5401,
            0x075cd401, 0x075d3c01, 0x075dbc01, 0x075e2401, 0x075ea401,
            0x075f0c01, 0x07bbc002, 0x07c0002c, 0x07c0c064, 0x07c2800f,
            0x07c2c40e, 0x07c3040f, 0x07c3440f, 0x07c4401f, 0x07c4c03c,
            0x07c5c02b, 0x07c7981d, 0x07c8402b, 0x07c90009, 0x07c94002,
            0x07cc0021, 0x07ccc006, 0x07ccdc46, 0x07ce0014, 0x07ce8025,
            0x07cf1805, 0x07cf8011, 0x07d0003f, 0x07d10001, 0x07d108b6,
            0x07d3e404, 0x07d4003e, 0x07d50004, 0x07d54018, 0x07d7ec46,
            0x07d9140b, 0x07da0046, 0x07dc0074, 0x38000401, 0x38008060,
            0x380400f0,
    };

    static const unsigned int aAscii[4] =
    {
            0xFFFFFFFF, 0xFC00FFFF, 0xF8000001, 0xF8000001,
    };

    if (c<128)
    {
        return (aAscii[c >> 5] & (1 << (c & 0x001F))) == 0;
    }
    else if (c < (1<<22))
    {
        unsigned int key = (((unsigned int)c)<<10) | 0x000003FF;
        int iRes = 0;
        int iHi = sizeof(aEntry)/sizeof(aEntry[0]) - 1;
        int iLo = 0;

        while (iHi >= iLo)
        {
            int iTest = (iHi + iLo)/2;
            if (key >= aEntry[iTest])
            {
                iRes = iTest;
                iLo = iTest + 1;
            }
            else
            {
                iHi = iTest - 1;
            }
        }
        assert(aEntry[0] < key);
        assert(key >= aEntry[iRes]);
        return ((unsigned int)c) >= ((aEntry[iRes] >> 10) + (aEntry[iRes] & 0x3FF));
    }
    assert(0);
    return NDS_TRUE;
}

static nds_bool NdsUnicode61_isException(const NdsUnicode61* self, unichar c)
{
    if (self->alnumExceptionsCount > 0)
    {
        const unichar *exceptions = self->alnumExceptions;
        size_t idxStart = 0;
        size_t idxEnd = self->alnumExceptionsCount;
        while (idxEnd > idxStart)
        {
            const size_t idxTest = (idxStart + idxEnd)/2;
            const unichar test = exceptions[idxTest];
            if (test == c)
                return NDS_TRUE;
            else if (c < test)
                idxEnd = idxTest; /* end is non-inclusive */
            else
                idxStart = idxTest + 1;
        }
    }
    return NDS_FALSE;
}

static nds_bool NdsUnicode61_isTokenChar(const NdsUnicode61* self, unichar c)
{
    return NdsUnicode61_isAlNum(c) ^ NdsUnicode61_isException(self, c);
}

static int NdsUnicode61_addExceptions(NdsUnicode61* self, nds_bool tokenChars, const char* chars)
{
    const int len = strlen(chars);
    size_t extraCharsCount = 0;
    const char *c = chars;
    int l = len;
    unichar u;
    while (DecodeNextUtf8CharToUnichar(&c, &l, &u))
    {
        /*
         * A character present twice in input causes inefficient allocation (unused byte at the
         * end of the alnumExceptions array) but apart from that it causes no other issues.
         */
        if (NdsUnicode61_isTokenChar(self, u) != tokenChars)
            extraCharsCount++;
    }

    if (extraCharsCount > 0)
    {
        unichar* exceptions = (unichar*)realloc(self->alnumExceptions,
                (self->alnumExceptionsCount + extraCharsCount) * sizeof(unichar));
        if (exceptions == NULL)
            return SQLITE_NOMEM;

        self->alnumExceptions = exceptions;

        l = len;
        c = chars;
        while (DecodeNextUtf8CharToUnichar(&c, &l, &u))
        {
            if (NdsUnicode61_isTokenChar(self, u) != tokenChars)
            {
                /* find where to insert u */
                size_t insertIdx = 0;
                size_t i;

                while (insertIdx < self->alnumExceptionsCount && exceptions[insertIdx] < u)
                    ++insertIdx;

                if (insertIdx == self->alnumExceptionsCount || exceptions[insertIdx] != u)
                {
                    /* shift the reminder of the array */
                    for (i = self->alnumExceptionsCount; i > insertIdx; i--)
                        exceptions[i] = exceptions[i-1];

                    /* insert u */
                    exceptions[insertIdx] = u;
                    self->alnumExceptionsCount++;
                }
                /*
                 * else: character is specified multiple times or both in tokenchars and
                 * separators - this is not supported
                 */
            }
        }
    }
    return SQLITE_OK;
}

/**
 * Append a unicode codepoint to the token buffer.
 *
 * This function handles unicode normalization (decomposition) and
 * stripping of diacritics (if enabled).
 *
 * \return True unless storing the character(s) failed.
 */
static nds_bool NdsUnicode61_addTokenCodepoint(NdsUnicode61Cursor* cursor, unichar character)
{
    const nds_bool removeDiacritics = NdsUnicode61_getModulefromCursor(cursor)->removeDiacritics;
    unsigned i;
    unichar const* decomposed = NULL;
    unsigned decompositionLen = 0;

    /* resuse FTS code: case folding FTS and optional diacritic removal */
    character = sqlite3FtsUnicodeFold(character, removeDiacritics);
    if (character == 0)
        return NDS_TRUE; /* 0 means character is completely ignored */

    if (!UnicharDecompose(character, &decomposed, &decompositionLen))
    {
        decomposed = &character;
        decompositionLen = 1;
    }


    for (i = 0; i < decompositionLen; ++i)
    {
        const unichar c = decomposed[i];
        if (!removeDiacritics || !NdsUnicode61_isDiacritic(c))
        {
            if (!StringBuffer_pushBackUnichar(&cursor->token, c))
                return NDS_FALSE;
        }
    }

    return NDS_TRUE;
}

/**
 * Append the current character and any successive combining diacritics to the bigram buffer.
 *
 * The processing starts as cursor->current (cursor->currentCharacter), and if it's
 * a combining diacritics it continues reading subsequent characters as long as they are
 * combining diacritic and adding them to the token buffer.
 *
 * In the end, cursor->current points to the next character (no longer a combining diacritic)
 * or end (cursor->inputEnd) if there are no more characters in the input.
 *
 * \return true unless decoding (invalid utf8) or storing a character failed.
 */
static nds_bool NdsUnicode61_accumulateTokenCharacter(NdsUnicode61Cursor* cursor)
{
    nds_bool success = NDS_TRUE;
    nds_bool cont = NDS_TRUE;

    if (!NdsUnicode61_addTokenCodepoint(cursor, cursor->currentCharacter))
        return NDS_FALSE;

    while (success && cont)
    {
        success = NdsUnicode61_getNextChar(cursor);
        if (success)
        {
            cont = NdsUnicode61_isDiacritic(cursor->currentCharacter);
            if (cont)
                NdsUnicode61_addTokenCodepoint(cursor, cursor->currentCharacter);
        }
    }

    return NdsUnicode61_isEnd(cursor) || success;
}

/**
 * Move the current pointer until a non-separator or end is reached.
 *
 * \return False if decoding input failed, true otherwise.
 */
static nds_bool NdsUnicode61_seekTokenStart(NdsUnicode61Cursor* cursor)
{
    const NdsUnicode61* module = NdsUnicode61_getModulefromCursor(cursor);
    nds_bool success;
    assert(cursor->state == Unknown);

    success = NdsUnicode61_getNextChar(cursor);
    while (success && !NdsUnicode61_isTokenChar(module, cursor->currentCharacter))
    {
        success = NdsUnicode61_getNextChar(cursor);
    }

    return NdsUnicode61_isEnd(cursor) || success;
}

/**
 * Find the end of the current latin word.
 *
 * This should search through input, starting at cursor->current until either
 * end of the token is reached or a bi-grammable character is found.
 *
 * \param[in,out] cursor     The state of the cursor.
 * \param[out]    pnBytes    Number of bytes making up the token.
 */
static int NdsUnicode61_nextLatin(NdsUnicode61Cursor* cursor, int* pnBytes)
{
    const NdsUnicode61* module = NdsUnicode61_getModulefromCursor(cursor);
    nds_bool currentIsCJK = NDS_FALSE;
    nds_bool currentIsSeparator = NDS_FALSE;
    nds_bool success = NDS_TRUE;
    const char* tokenStart = cursor->current;

    assert(cursor->state == Latin);

    StringBuffer_flush(&cursor->token);
    if (!NdsUnicode61_accumulateTokenCharacter(cursor))
        return SQLITE_ERROR;

    while (success && !currentIsCJK && !currentIsSeparator && !NdsUnicode61_isEnd(cursor))
    {
        currentIsCJK = NdsUnicode61_isBigrammable(cursor->currentCharacter);
        currentIsSeparator = !NdsUnicode61_isTokenChar(module, cursor->currentCharacter);
        if (!currentIsCJK && !currentIsSeparator)
        {
            success = NdsUnicode61_accumulateTokenCharacter(cursor);
        }
    }

    if (!success)
    {
        /* invalid utf8 input */
        NdsUnicode61_resetCursor(cursor);
        return SQLITE_ERROR;
    }

    if (NdsUnicode61_isEnd(cursor))
    {
        assert(NdsUnicode61_isEnd(cursor));
        *pnBytes = cursor->next - tokenStart;
        /* all input has been read, next call to next() will yield SQLITE_DONE */
        cursor->state = Unknown;
    }
    else
    {
        assert(currentIsSeparator || currentIsCJK);
        /* the last character read is not to be included in the returned token */
        *pnBytes = cursor->current - tokenStart;
        cursor->state = (currentIsSeparator) ? Unknown : BigramStart;
    }

    return SQLITE_OK;
}

/**
 * Implementation of "next" callback for CJK characters.
 *
 * This function is called in the following states:
 *
 * * BigramStart: cursor->current is a CJK (bigrammable) character,
 * but the bigram buffer is empty. (It's the first bigrammable character
 * after the start of input, or a non-bigrammable character).
 * In this state two characters need to be read and put into the
 * bigram buffer.
 *
 * * Bigram: cursor->current is a CJK (bigrammable) character but there
 * have been preceding bigrammable characters.
 * This should search through input, starting at cursor->current until either
 * end of the token is reached or a bi-grammable character is found.
 * In this state the 1st character stored in the bigram buffer needs to be
 * removed and another character needs to be read and appended to the buffer.
 *
 * If there's an isolated character (state is BigramStart and the next character
 * is not bigrammable), it's returned on its own.
 *
 * If the next character is not bigrammable, the state is changed to Latin.
 *
 * \param[in,out] cursor    The state of the cursor.
 * \param[out]    haveToken set to true if a token has been extracted.
 * \return True unless an error has occurred.
 */
static nds_bool NdsUnicode61_nextBigram(NdsUnicode61Cursor* cursor, nds_bool* haveToken)
{
    assert(cursor->state == Bigram || cursor->state == BigramStart);

    if (cursor->state == BigramStart)
    {
        StringBuffer_flush(&cursor->token);

        cursor->bigramOffset1 = cursor->current - cursor->inputStart;
        if (!NdsUnicode61_accumulateTokenCharacter(cursor))
            return NDS_FALSE;

        cursor->bigramFirstCharSize = StringBuffer_getSize(&cursor->token);
    }
    else
    {
        StringBuffer_popFront(&cursor->token, cursor->bigramFirstCharSize);
        cursor->bigramFirstCharSize = StringBuffer_getSize(&cursor->token);
        cursor->bigramOffset1 = cursor->bigramOffset2;
    }

    if (!NdsUnicode61_isEnd(cursor))
    {
        const NdsUnicode61* module = NdsUnicode61_getModulefromCursor(cursor);
        const nds_bool isSeparator = !NdsUnicode61_isTokenChar(module, cursor->currentCharacter);
        if (!isSeparator && NdsUnicode61_isBigrammable(cursor->currentCharacter))
        {
            /* we have a CJK bi-gram */
            cursor->bigramOffset2 = cursor->current - cursor->inputStart;
            if (!NdsUnicode61_accumulateTokenCharacter(cursor))
                return NDS_FALSE;

            cursor->state = Bigram;
            *haveToken = NDS_TRUE;
        }
        else
        {
            /* we left the CJK substring */
            if (cursor->state == BigramStart)
            {
                /* it's a single CJK character ('a&a' or '&') -> return it as a token */
                *haveToken = NDS_TRUE;
            }
            /* else: nothing to do */

            cursor->state = (isSeparator) ? Unknown : Latin;
        }
    }
    else
    {
        /* nothing more to read */
        if (cursor->state == BigramStart)
        {
            /* isolated CJK character -> return it as a token */
            *haveToken = NDS_TRUE;
        }
        /* else: not isolated, already part of (previous) bigram */

        cursor->state = Unknown;
    }

    return SQLITE_OK;
}

/**
 * Implementation of the tokenizer's next callback. It's responsible for fetching the next token.
 */
int NdsUnicode61_next(sqlite3_tokenizer_cursor* pCursor, const char** ppToken, int* pnBytes,
        int* piStartOffset, int* piEndOffset, int* piPosition)
{
    NdsUnicode61Cursor* cursor = (NdsUnicode61Cursor*)pCursor;
    const NdsUnicode61* module = NdsUnicode61_getModulefromCursor(cursor);
    nds_bool haveToken = NDS_FALSE;

    if (cursor == NULL)
        return SQLITE_ERROR;

    while (!haveToken)
    {
        if (NdsUnicode61_isEnd(cursor))
            return SQLITE_DONE;

        if (cursor->state == Unknown)
        {
            nds_bool r = NdsUnicode61_seekTokenStart(cursor);
            if (!r)
                return SQLITE_ERROR;

            if (NdsUnicode61_isEnd(cursor))
                return SQLITE_DONE;

            cursor->state = (NdsUnicode61_isBigrammable(cursor->currentCharacter)) ? BigramStart : Latin;
        }

        assert(cursor->state != Unknown);

        if (cursor->state == Latin)
        {
            int result;
            size_t nBytes;

            *piStartOffset = cursor->current - cursor->inputStart;
            result = NdsUnicode61_nextLatin(cursor, pnBytes);
            if (result != SQLITE_OK)
                return result;
            *piEndOffset = *piStartOffset + *pnBytes;
            StringBuffer_getBuffer(&cursor->token, ppToken, &nBytes);
            *pnBytes = nBytes;
            haveToken = NDS_TRUE;
        }
        else
        {
            int r = NdsUnicode61_nextBigram(cursor, &haveToken);
            if (r != SQLITE_OK)
                return r;

            if (haveToken)
            {
                nds_bool lastBigram;
                size_t nBytes;

                StringBuffer_getBuffer(&cursor->token, ppToken, &nBytes);
                *pnBytes = (int)nBytes;
                *piStartOffset = cursor->bigramOffset1;

                /*
                 * For all but the last bigrams return indices for the first letter.
                 * This makes SQLite correctly handle tokenizing match expressions.
                 */
                lastBigram = NdsUnicode61_isEnd(cursor) ||
                        !NdsUnicode61_isTokenChar(module, cursor->currentCharacter) ||
                        !NdsUnicode61_isBigrammable(cursor->currentCharacter);

                *piEndOffset = (lastBigram) ?
                        cursor->current - cursor->inputStart
                        : cursor->bigramOffset2;
            }
        }
    }

    *piPosition = cursor->index++;

    return SQLITE_OK;
}

/**
 * Description of the NDS unicode61 tokenizer.
 */
static const sqlite3_tokenizer_module NDS_UNICODE61 =
{
        /*
         * Unicode61 also uses version '0' here (no support for xLanguageid()).
         */
        0,                      /* version */
        NdsUnicode61_create,    /* xCreate */
        NdsUnicode61_destroy,   /* xDestroy */
        NdsUnicode61_open,      /* xOpen */
        NdsUnicode61_close,     /* xClose */
        NdsUnicode61_next,      /* xNext */
        0                       /* xLanguageid (not supported for modules version 0) */
};

/* to be called from SQLite */
void sqlite3Fts3NdsUnicodeTokenizer(sqlite3_tokenizer_module const** ndsunicode61)
{
    *ndsunicode61 = &NDS_UNICODE61;
}
