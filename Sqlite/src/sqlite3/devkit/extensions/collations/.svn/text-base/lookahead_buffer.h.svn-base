#ifndef LOOKAHEAD_BUFFER_H
#define LOOKAHEAD_BUFFER_H

#include "../utils/nds_bool.h"
#include "../utils/unichar.h"
#include "unichar_queue.h"
#include "utf8_decompose_iterator.h"
#include "../utils/nds_extension_macros.h"

/**
 * A buffer that adds a look-ahead functionality to Utf8DecomposeIterator.
 *
 * This is used when walking the collation trie. Under some circumstances the
 * code needs to check what the next characters are without consuming them.
 */
typedef struct
{
    Utf8DecomposeIterator*  it;
    UnicharQueue            queue;
} LookaheadBuffer;

NDS_EXTENSION_BEGIN_DECLS

/**
 * Initialize an instance of the buffer.
 *
 * \param[in] self Pointer to the instance.
 * \param[in] it   Iterator to wrap.
 */
void LookaheadBuffer_init(LookaheadBuffer* self, Utf8DecomposeIterator* it);

/**
 * Retrieve next character.
 *
 * \param[in]  self      Pointer to the instance.
 * \param[out] character Pointer to a variable that receives the character.
 *
 * \return True if a character has been retrieved.
 */
nds_bool LookaheadBuffer_getNextChar(LookaheadBuffer* self, unichar* character);

/**
 * Peek next character.
 *
 * The instance maintains a peek pointer. Subsequent calls to
 * LookaheadBuffer_peekNextChar() return further characters
 * (up to the end of input, or a peek depth limit).
 *
 * \param[in]  self      Pointer to the instance.
 * \param[out] character Pointer to a variable that receives the character.
 *
 * \return True if a character has been retrieved.
 */
nds_bool LookaheadBuffer_peekNextChar(LookaheadBuffer* self, unichar* character);

/**
 * Reset peek pointer.
 *
 * This call sets the peek pointer to the same value as the read pointer.
 *
 * \param[in] self Pointer to the instance.
 */
void LookaheadBuffer_resetPeekPointer(LookaheadBuffer* self);

/**
 * Read and discard given number of characters.
 *
 * \param[in] self  Pointer to the instance.
 * \param[in] count Number of characters to read.
 */
void LookaheadBuffer_skipChars(LookaheadBuffer* self, int count);

NDS_EXTENSION_END_DECLS

#endif /* LOOKAHEAD_BUFFER_H */
