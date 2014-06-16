#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "test_lookahead_buffer.h"
#include "extensions_test.h"

#include "../tokenizer/nds_unicode61.h"
#include "../utils/nds_extension_macros.h"

struct Token
{
    /*
     * for extra fun make sure the token is NOT nul-terminated
     * current sqlite doc says the tokenizers anyway require nul-terminated
     * strings... but the nds tokenizer doesn't need it, so we can happily
     * test it against a non-nul-terminated string
     */
    const char* token;
    int nBytes;
    int startOffset;
    int endOffset;
};

namespace
{
    /**
     * A C++ wrapper around the ndsunicode61 tokenizer.
     */
    class NdsUnicode61Tokenizer
    {
    public:
        NdsUnicode61Tokenizer():
            m_tokenizerModule(getTokenizer()),
            m_tokenizer(NULL)
        {}

        bool open(int argc, char const* const* argv)
        {
            if (m_tokenizer)
                return false;

            return m_tokenizerModule->xCreate(argc, argv, &m_tokenizer) == SQLITE_OK;
        }

        bool close()
        {
            if (!m_tokenizer)
                return false;

            const int r = m_tokenizerModule->xDestroy(m_tokenizer);
            m_tokenizer = NULL;

            return r == SQLITE_OK;
        }

        ~NdsUnicode61Tokenizer()
        {
            close();
        }

        class cursor
        {
        public:
            cursor(sqlite3_tokenizer_module const* tokenizerModule, sqlite3_tokenizer_cursor* c):
                m_tokenizerModule(tokenizerModule),
                m_cursor(c)
            {}

            bool nextToken(const char*& pToken, int& nBytes, int& iStartOffset,
                    int& iEndOffset, int& iPosition)
            {
                return m_tokenizerModule->xNext(m_cursor, &pToken, &nBytes, &iStartOffset,
                        &iEndOffset, &iPosition) == SQLITE_OK;
            }

            ~cursor()
            {
                const int result = m_tokenizerModule->xClose(m_cursor);
                EXPECT_EQ(SQLITE_OK, result);
            }

        private:
            sqlite3_tokenizer_module const* m_tokenizerModule;
            sqlite3_tokenizer_cursor* m_cursor;
        };

        // only one cursor instance can exist at any given time
        // because the test data is stored in a static variable
        cursor* getCursor(const char* input, int nBytes)
        {
            sqlite3_tokenizer_cursor* cur = NULL;
            const int r = m_tokenizerModule->xOpen(m_tokenizer, input, nBytes, &cur);

            if (r != SQLITE_OK)
                return NULL;

            cur->pTokenizer = m_tokenizer;
            return new cursor(m_tokenizerModule, cur);
        }

    private:
        static sqlite3_tokenizer_module const* getTokenizer()
        {
            sqlite3_tokenizer_module const* tokenizer = NULL;
            sqlite3Fts3NdsUnicodeTokenizer(&tokenizer);
            return tokenizer;
        }

        sqlite3_tokenizer_module const* m_tokenizerModule;
        sqlite3_tokenizer* m_tokenizer;
    };

    void TestReturnedTokens(NdsUnicode61Tokenizer::cursor &cur, Token const* tokens, int count)
    {
        const char* token;
        int len;
        int startOffset;
        int endOffset;
        int idx;

        for (int i = 0; i < count; ++i)
        {
            Token const& t = tokens[i];

            // get next token
            EXPECT_EQ(true, cur.nextToken(token, len, startOffset, endOffset, idx));

            // check results
            EXPECT_EQ(t.nBytes, len);
            EXPECT_EQ(0, strncmp(t.token, token, len));
            EXPECT_EQ(t.startOffset, startOffset);
            EXPECT_EQ(t.endOffset, endOffset);
            EXPECT_EQ(i, idx);
        }

        // no more tokens
        EXPECT_EQ(false, cur.nextToken(token, len, startOffset, endOffset, idx));
    }
}

void TestNdsUnicode61Tokenizer_open()
{
    // just tests open/close
    NdsUnicode61Tokenizer tokenizer;
    EXPECT_EQ(true, tokenizer.open(0, NULL));
    EXPECT_EQ(true, tokenizer.close());
}

void TestNdsUnicode61Tokenizer_create()
{
    static const char TEST_INPUT[] = "foo";
    static const int TEST_INPUT_LEN = ARRAY_SIZE(TEST_INPUT);

    NdsUnicode61Tokenizer tokenizer;
    EXPECT_EQ(true, tokenizer.open(0, NULL));
    NdsUnicode61Tokenizer::cursor* cur = tokenizer.getCursor(TEST_INPUT, TEST_INPUT_LEN);
    EXPECT_EQ(true, cur != NULL);
    delete cur;
    EXPECT_EQ(true, tokenizer.close());
}

void TestNdsUnicode61Tokenizer_latin()
{
    static const char TEST_INPUT[] = "foo test a";

    static const Token OUTPUT_TOKENS[] =
    {
            {
                    "foo",
                    3,
                    0,
                    3
            },
            {
                    "test",
                    4,
                    4,
                    8
            },
            {
                    "a",
                    1,
                    9,
                    10
            }
    };

    NdsUnicode61Tokenizer tokenizer;
    EXPECT_EQ(true, tokenizer.open(0, NULL));
    NdsUnicode61Tokenizer::cursor* cur = tokenizer.getCursor(TEST_INPUT, ARRAY_SIZE(TEST_INPUT));
    EXPECT_EQ(true, cur != NULL);

    // the latin tokens should just pass-through the ndsunicode61 tokenizer
    TestReturnedTokens(*cur, OUTPUT_TOKENS, ARRAY_SIZE(OUTPUT_TOKENS));

    delete cur;
    EXPECT_EQ(true, tokenizer.close());
}

void TestNdsUnicode61Tokenizer_japanese()
{
    // "出身の物理化学者、電気化学者。"
    static const char TEST_INPUT[] =
    {
        // U+51FA CJK UNIFIED IDEOGRAPH-51FA 出
        C(0xe5), C(0x87), C(0xba),
        // U+8EAB CJK UNIFIED IDEOGRAPH-8EAB 身
        C(0xe8), C(0xba), C(0xab),
        // U+306E HIRAGANA LETTER NO の
        C(0xe3), C(0x81), C(0xae),
        // U+7269 CJK UNIFIED IDEOGRAPH-7269 物
        C(0xe7), C(0x89), C(0xa9),
        // U+7406 CJK UNIFIED IDEOGRAPH-7406 理
        C(0xe7), C(0x90), C(0x86),
        // U+5316 CJK UNIFIED IDEOGRAPH-5316 化
        C(0xe5), C(0x8c), C(0x96),
        // U+5B66 CJK UNIFIED IDEOGRAPH-5B66 学
        C(0xe5), C(0xad), C(0xa6),
        // U+8005 CJK UNIFIED IDEOGRAPH-8005 者
        C(0xe8), C(0x80), C(0x85),
        // U+3001 IDEOGRAPHIC COMMA 、
        C(0xe3), C(0x80), C(0x81),
        // U+96FB CJK UNIFIED IDEOGRAPH-96FB 電
        C(0xe9), C(0x9b), C(0xbb),
        // U+6C17 CJK UNIFIED IDEOGRAPH-6C17 気
        C(0xe6), C(0xb0), C(0x97),
        // U+5316 CJK UNIFIED IDEOGRAPH-5316 化
        C(0xe5), C(0x8c), C(0x96),
        // U+5B66 CJK UNIFIED IDEOGRAPH-5B66 学
        C(0xe5), C(0xad), C(0xa6),
        // U+8005 CJK UNIFIED IDEOGRAPH-8005 者
        C(0xe8), C(0x80), C(0x85),
        // U+3002 IDEOGRAPHIC FULL STOP 。
        C(0xe3), C(0x80), C(0x82),
    };

    /*
     * A bit of a hack to keep tests simple: This assumes each of the
     * characters in this test takes SINGLE_CJK_CHAR_BYTES bytes.
     */
    static const size_t SINGLE_CJK_CHAR_BYTES = 3;

    static const char OUTPUT_TOKEN_00[] =
    {
            // U+51FA CJK UNIFIED IDEOGRAPH-51FA 出
            C(0xe5), C(0x87), C(0xba),
            // U+8EAB CJK UNIFIED IDEOGRAPH-8EAB 身
            C(0xe8), C(0xba), C(0xab),
    };
    static const char OUTPUT_TOKEN_01[] =
    {
            // U+8EAB CJK UNIFIED IDEOGRAPH-8EAB 身
            C(0xe8), C(0xba), C(0xab),
            // U+306E HIRAGANA LETTER NO の
            C(0xe3), C(0x81), C(0xae),
    };
    static const char OUTPUT_TOKEN_02[] =
    {
            // U+306E HIRAGANA LETTER NO の
            C(0xe3), C(0x81), C(0xae),
            // U+7269 CJK UNIFIED IDEOGRAPH-7269 物
            C(0xe7), C(0x89), C(0xa9),
    };
    static const char OUTPUT_TOKEN_03[] =
    {
            // U+7269 CJK UNIFIED IDEOGRAPH-7269 物
            C(0xe7), C(0x89), C(0xa9),
            // U+7406 CJK UNIFIED IDEOGRAPH-7406 理
            C(0xe7), C(0x90), C(0x86),
    };
    static const char OUTPUT_TOKEN_04[] =
    {
            // U+7406 CJK UNIFIED IDEOGRAPH-7406 理
            C(0xe7), C(0x90), C(0x86),
            // U+5316 CJK UNIFIED IDEOGRAPH-5316 化
            C(0xe5), C(0x8c), C(0x96),
    };
    static const char OUTPUT_TOKEN_05[] =
    {
            // U+5316 CJK UNIFIED IDEOGRAPH-5316 化
            C(0xe5), C(0x8c), C(0x96),
            // U+5B66 CJK UNIFIED IDEOGRAPH-5B66 学
            C(0xe5), C(0xad), C(0xa6),
    };
    static const char OUTPUT_TOKEN_06[] =
    {
            // U+5B66 CJK UNIFIED IDEOGRAPH-5B66 学
            C(0xe5), C(0xad), C(0xa6),
            // U+8005 CJK UNIFIED IDEOGRAPH-8005 者
            C(0xe8), C(0x80), C(0x85),
    };
    static const char OUTPUT_TOKEN_07[] =
    {
            // U+96FB CJK UNIFIED IDEOGRAPH-96FB 電
            C(0xe9), C(0x9b), C(0xbb),
            // U+6C17 CJK UNIFIED IDEOGRAPH-6C17 気
            C(0xe6), C(0xb0), C(0x97),
    };
    static const char OUTPUT_TOKEN_08[] =
    {
            // U+6C17 CJK UNIFIED IDEOGRAPH-6C17 気
            C(0xe6), C(0xb0), C(0x97),
            // U+5316 CJK UNIFIED IDEOGRAPH-5316 化
            C(0xe5), C(0x8c), C(0x96),
    };
    static const char OUTPUT_TOKEN_09[] =
    {
            // U+5316 CJK UNIFIED IDEOGRAPH-5316 化
            C(0xe5), C(0x8c), C(0x96),
            // U+5B66 CJK UNIFIED IDEOGRAPH-5B66 学
            C(0xe5), C(0xad), C(0xa6),
    };
    static const char OUTPUT_TOKEN_10[] =
    {
            // U+5B66 CJK UNIFIED IDEOGRAPH-5B66 学
            C(0xe5), C(0xad), C(0xa6),
            // U+8005 CJK UNIFIED IDEOGRAPH-8005 者
            C(0xe8), C(0x80), C(0x85),
    };

    static const Token OUTPUT_TOKENS[] =
    {
            {
                    OUTPUT_TOKEN_00,
                    ARRAY_SIZE(OUTPUT_TOKEN_00),
                    0*3,
                    0*3 + SINGLE_CJK_CHAR_BYTES // don't include last character
            },
            {
                    OUTPUT_TOKEN_01,
                    ARRAY_SIZE(OUTPUT_TOKEN_01),
                    1*3, // all the CJK test characters are 3 bytes long
                    1*3 + SINGLE_CJK_CHAR_BYTES
            },
            {
                    OUTPUT_TOKEN_02,
                    ARRAY_SIZE(OUTPUT_TOKEN_02),
                    2*3,
                    2*3 + SINGLE_CJK_CHAR_BYTES
            },
            {
                    OUTPUT_TOKEN_03,
                    ARRAY_SIZE(OUTPUT_TOKEN_03),
                    3*3,
                    3*3 + SINGLE_CJK_CHAR_BYTES
            },
            {
                    OUTPUT_TOKEN_04,
                    ARRAY_SIZE(OUTPUT_TOKEN_04),
                    4*3,
                    4*3 + SINGLE_CJK_CHAR_BYTES
            },
            {
                    OUTPUT_TOKEN_05,
                    ARRAY_SIZE(OUTPUT_TOKEN_05),
                    5*3,
                    5*3 + SINGLE_CJK_CHAR_BYTES
            },
            {
                    OUTPUT_TOKEN_06,
                    ARRAY_SIZE(OUTPUT_TOKEN_06),
                    6*3,
                    6*3 + ARRAY_SIZE(OUTPUT_TOKEN_06) // last before a separator -> include both chars
            },
            {
                    OUTPUT_TOKEN_07,
                    ARRAY_SIZE(OUTPUT_TOKEN_07),
                    9*3, // there it a comma between token 7 and 8 (comma + last pair -> 2 more chars)
                    9*3 + SINGLE_CJK_CHAR_BYTES
            },
            {
                    OUTPUT_TOKEN_08,
                    ARRAY_SIZE(OUTPUT_TOKEN_08),
                    10*3,
                    10*3 + SINGLE_CJK_CHAR_BYTES
            },
            {
                    OUTPUT_TOKEN_09,
                    ARRAY_SIZE(OUTPUT_TOKEN_09),
                    11*3,
                    11*3 + SINGLE_CJK_CHAR_BYTES
            },
            {
                    OUTPUT_TOKEN_10,
                    ARRAY_SIZE(OUTPUT_TOKEN_10),
                    12*3,
                    12*3 + ARRAY_SIZE(OUTPUT_TOKEN_10) // last before a separator -> include both chars
            },
    };

    NdsUnicode61Tokenizer tokenizer;
    EXPECT_EQ(true, tokenizer.open(0, NULL));
    NdsUnicode61Tokenizer::cursor* cur = tokenizer.getCursor(TEST_INPUT, ARRAY_SIZE(TEST_INPUT));
    EXPECT_EQ(true, cur != NULL);

    TestReturnedTokens(*cur, OUTPUT_TOKENS, ARRAY_SIZE(OUTPUT_TOKENS));

    delete cur;
    EXPECT_EQ(true, tokenizer.close());
}

void TestNdsUnicode61Tokenizer_mixed()
{
    // "foo出身のbar者、電a"
    static const char TEST_INPUT[] =
    {
            'f', 'o', 'o',
            // U+51FA CJK UNIFIED IDEOGRAPH-51FA 出
            C(0xe5), C(0x87), C(0xba),
            // U+8EAB CJK UNIFIED IDEOGRAPH-8EAB 身
            C(0xe8), C(0xba), C(0xab),
            // U+306E HIRAGANA LETTER NO の
            C(0xe3), C(0x81), C(0xae),
            'b', 'a', 'r',
            // U+8005 CJK UNIFIED IDEOGRAPH-8005 者
            C(0xe8), C(0x80), C(0x85),
            // U+3001 IDEOGRAPHIC COMMA 、
            C(0xe3), C(0x80), C(0x81),
            // U+96FB CJK UNIFIED IDEOGRAPH-96FB 電
            C(0xe9), C(0x9b), C(0xbb),
            'a'
    };

    /*
     * A bit of a hack to keep tests simple: This assumes each of the
     * characters in this test takes SINGLE_CJK_CHAR_BYTES bytes.
     */
    static const size_t SINGLE_CJK_CHAR_BYTES = 3;

    static const char OUTPUT_TOKEN_00[] =
    {
            'f','o','o',
    };
    static const char OUTPUT_TOKEN_01[] =
    {
            // U+51FA CJK UNIFIED IDEOGRAPH-51FA 出
            C(0xe5), C(0x87), C(0xba),
            // U+8EAB CJK UNIFIED IDEOGRAPH-8EAB 身
            C(0xe8), C(0xba), C(0xab),
    };
    static const char OUTPUT_TOKEN_02[] =
    {
            // U+8EAB CJK UNIFIED IDEOGRAPH-8EAB 身
            C(0xe8), C(0xba), C(0xab),
            // U+306E HIRAGANA LETTER NO の
            C(0xe3), C(0x81), C(0xae),
    };
    static const char OUTPUT_TOKEN_03[] =
    {
            'b','a','r',
    };
    static const char OUTPUT_TOKEN_04[] =
    {
            // U+8005 CJK UNIFIED IDEOGRAPH-8005 者
            C(0xe8), C(0x80), C(0x85),
    };
    static const char OUTPUT_TOKEN_05[] =
    {
            // U+96FB CJK UNIFIED IDEOGRAPH-96FB 電
            C(0xe9), C(0x9b), C(0xbb),
    };
    static const char OUTPUT_TOKEN_06[] =
    {
            'a'
    };

    static const Token OUTPUT_TOKENS[] =
    {
            {
                    OUTPUT_TOKEN_00,
                    ARRAY_SIZE(OUTPUT_TOKEN_00),
                    0,
                    ARRAY_SIZE(OUTPUT_TOKEN_00)
            },
            {
                    OUTPUT_TOKEN_01,
                    ARRAY_SIZE(OUTPUT_TOKEN_01),
                    ARRAY_SIZE(OUTPUT_TOKEN_00),
                    ARRAY_SIZE(OUTPUT_TOKEN_00) + SINGLE_CJK_CHAR_BYTES // don't include last character
            },
            {
                    OUTPUT_TOKEN_02,
                    ARRAY_SIZE(OUTPUT_TOKEN_02),
                    ARRAY_SIZE(OUTPUT_TOKEN_00) + 3,
                    ARRAY_SIZE(OUTPUT_TOKEN_00) + 3 + ARRAY_SIZE(OUTPUT_TOKEN_02) // last - include both
            },
            {
                    OUTPUT_TOKEN_03,
                    ARRAY_SIZE(OUTPUT_TOKEN_03),
                    ARRAY_SIZE(OUTPUT_TOKEN_00) + 3 + ARRAY_SIZE(OUTPUT_TOKEN_02),
                    ARRAY_SIZE(OUTPUT_TOKEN_00) + 3 + ARRAY_SIZE(OUTPUT_TOKEN_02)
                        + ARRAY_SIZE(OUTPUT_TOKEN_03)
            },
            {
                    OUTPUT_TOKEN_04,
                    ARRAY_SIZE(OUTPUT_TOKEN_04),
                    ARRAY_SIZE(OUTPUT_TOKEN_00) + 3 + ARRAY_SIZE(OUTPUT_TOKEN_02)
                        + ARRAY_SIZE(OUTPUT_TOKEN_03),
                    ARRAY_SIZE(OUTPUT_TOKEN_00) + 3 + ARRAY_SIZE(OUTPUT_TOKEN_02)
                        + ARRAY_SIZE(OUTPUT_TOKEN_03) + ARRAY_SIZE(OUTPUT_TOKEN_04) // last - include both
            },
            {
                    OUTPUT_TOKEN_05,
                    ARRAY_SIZE(OUTPUT_TOKEN_05),
                    ARRAY_SIZE(OUTPUT_TOKEN_00) + 3 + ARRAY_SIZE(OUTPUT_TOKEN_02)
                        + ARRAY_SIZE(OUTPUT_TOKEN_03) + ARRAY_SIZE(OUTPUT_TOKEN_04) + 3 /*comma*/,
                    ARRAY_SIZE(OUTPUT_TOKEN_00) + 3 + ARRAY_SIZE(OUTPUT_TOKEN_02)
                        + ARRAY_SIZE(OUTPUT_TOKEN_03) + ARRAY_SIZE(OUTPUT_TOKEN_04) + 3
                        + ARRAY_SIZE(OUTPUT_TOKEN_05) // last - include both
            },
            {
                    OUTPUT_TOKEN_06,
                    ARRAY_SIZE(OUTPUT_TOKEN_06),
                    ARRAY_SIZE(OUTPUT_TOKEN_00) + 3 + ARRAY_SIZE(OUTPUT_TOKEN_02)
                        + ARRAY_SIZE(OUTPUT_TOKEN_03) + ARRAY_SIZE(OUTPUT_TOKEN_04) + 3
                        + ARRAY_SIZE(OUTPUT_TOKEN_05),
                        ARRAY_SIZE(OUTPUT_TOKEN_00) + 3 + ARRAY_SIZE(OUTPUT_TOKEN_02)
                            + ARRAY_SIZE(OUTPUT_TOKEN_03) + ARRAY_SIZE(OUTPUT_TOKEN_04) + 3
                            + ARRAY_SIZE(OUTPUT_TOKEN_05) + ARRAY_SIZE(OUTPUT_TOKEN_06)
            },
    };

    NdsUnicode61Tokenizer tokenizer;
    EXPECT_EQ(true, tokenizer.open(0, NULL));
    NdsUnicode61Tokenizer::cursor* cur = tokenizer.getCursor(TEST_INPUT, ARRAY_SIZE(TEST_INPUT));
    EXPECT_EQ(true, cur != NULL);

    TestReturnedTokens(*cur, OUTPUT_TOKENS, ARRAY_SIZE(OUTPUT_TOKENS));

    delete cur;
    EXPECT_EQ(true, tokenizer.close());
}

void TestNdsUnicode61Tokenizer_tokencharsSeparators()
{
    NdsUnicode61Tokenizer tokenizer;

    static char const* argv[] = {
            "tokenchars= \t",
            "separators=/;\""
    };

    static const char TEST_INPUT[] = "a a\t a/;/b b;c c\"d d";

    static const Token OUTPUT_TOKENS[] =
    {
            {
                    "a a\t a",
                    6,
                    0,
                    6
            },
            {
                    "b b",
                    3,
                    9,
                    12
            },
            {
                    "c c",
                    3,
                    13,
                    16
            },
            {
                    "d d",
                    3,
                    17,
                    20
            }
    };

    EXPECT_EQ(true, tokenizer.open(ARRAY_SIZE(argv), argv));
    NdsUnicode61Tokenizer::cursor* cur = tokenizer.getCursor(TEST_INPUT, ARRAY_SIZE(TEST_INPUT));
    EXPECT_EQ(true, cur != NULL);

    TestReturnedTokens(*cur, OUTPUT_TOKENS, ARRAY_SIZE(OUTPUT_TOKENS));

    delete cur;
    EXPECT_EQ(true, tokenizer.close());
}

void TestNdsUnicode61Tokenizer_ideographicSpace()
{
    NdsUnicode61Tokenizer tokenizer;

    static const char TEST_INPUT[] = {
            'f', 'o', 'o',
            C(0xe3), C(0x80), C(0x80), // U+3000 IDEOGRAPHIC SPACE
            'b', 'a', 'r'
    };

    static const Token OUTPUT_TOKENS[] =
    {
            {
                    "foo",
                    3,
                    0,
                    3
            },
            {
                    "bar",
                    3,
                    6,
                    9
            },
    };

    EXPECT_EQ(true, tokenizer.open(0, NULL));
    NdsUnicode61Tokenizer::cursor* cur = tokenizer.getCursor(TEST_INPUT, ARRAY_SIZE(TEST_INPUT));
    EXPECT_EQ(true, cur != NULL);

    TestReturnedTokens(*cur, OUTPUT_TOKENS, ARRAY_SIZE(OUTPUT_TOKENS));

    delete cur;
    EXPECT_EQ(true, tokenizer.close());
}
