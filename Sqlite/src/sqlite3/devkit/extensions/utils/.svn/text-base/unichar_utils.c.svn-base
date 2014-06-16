#include <stddef.h>
#include <assert.h>

#include "nds_bool.h"
#include "unichar_utils.h"
#include "unichar_decomposition_data.h"

nds_bool UnicharIsCombiningMark(unichar character)
{
    /*
     * TODO: the combining characters are located also outside these ranges!
     * so another table is needed to get this right
     */
    return
            (character >= 0x0300 && character <= 0x036f)    /* Combining Diacritical Marks */
            || (character >= 0x1dc0 && character <= 0x1dff) /* Combining Diacritical Marks Supplement */
            || (character >= 0x20d0 && character <= 0x20ff) /* Combining Diacritical Marks for Symbols */
            || (character >= 0xfe20 && character <= 0xfe2f) /* Combining Half Marks */
            /* for Japanese */
            || character == 0x3099 /* COMBINING KATAKANA-HIRAGANA VOICED SOUND MARK */
            || character == 0x309a /* COMBINING KATAKANA-HIRAGANA SEMI-VOICED SOUND MARK */
        ;
}

static nds_bool UnicharDecomposeInRange(UnicodeDecompositionRange const* range,
        unichar character, unichar const** decomposed, unsigned* decompositionLen)
{
    const size_t idx = character - range->first;
    unichar const* d;

    assert(idx < range->last-range->first + 1);
    d = range->data[idx];

    if (d != NULL)
    {
        *decompositionLen = (unsigned)*d;
        *decomposed = d + 1;
        return NDS_TRUE;
    }

    return NDS_FALSE;
}

nds_bool UnicharDecompose(unichar character, unichar const** decomposed, unsigned* decompositionLen)
{
    /*
     * Use linear search here as the number of ranges is small and the overhead
     * of binary search would not pay off.
     */
    size_t idx;
    for (idx = 0; idx < UnicharDecompositionDataCount; ++idx)
    {
        UnicodeDecompositionRange const* range = &UnicharDecompositionData[idx];
        if (range->first <= character && character <= range->last)
        {
            return UnicharDecomposeInRange(range, character, decomposed, decompositionLen);
        }
    }

    return NDS_FALSE;
}
