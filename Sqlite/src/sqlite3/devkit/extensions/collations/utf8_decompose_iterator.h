#ifndef UTF8_DECOMPOSE_ITERATOR_H
#define UTF8_DECOMPOSE_ITERATOR_H

#include "../utils/nds_bool.h"
#include "../utils/unichar.h"
#include "../utils/nds_extension_macros.h"

/**
 * Forward iterator for an utf8-encoded string.
 */
typedef struct
{
    int             inputLen;
    char const*     inputStr;
    unichar         currentChar;
    unichar const*  currentCharDecomposition;
    unsigned        decompositionLen;
} Utf8DecomposeIterator;

NDS_EXTENSION_BEGIN_DECLS

/**
 * Initialize the iterator.
 *
 * \param[in] self     Pointer to the instance.
 * \param[in] inputStr Pointer to the beginning of the utf8-encoded buffer.
 * \param[in] inputLen Size, in bytes, of the buffer.
 */
void Utf8DecomposeIterator_init(Utf8DecomposeIterator* self, char const* inputStr, int inputLen);

/**
 * Retrieve next unicode character from the buffer.
 *
 * \param[in]  self      Pointer to the instance.
 * \param[out] character Pointer to variable where the decoded character is to be stored.
 *
 * \return True iff a character has been retrieved.
 */
nds_bool Utf8DecomposeIterator_getNextCharacter(Utf8DecomposeIterator* self, unichar* character);

NDS_EXTENSION_END_DECLS

#endif /* UTF8_DECOMPOSE_ITERATOR_H */
