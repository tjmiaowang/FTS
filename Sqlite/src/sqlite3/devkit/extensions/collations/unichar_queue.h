#ifndef UNICHAR_QUEUE_H
#define UNICHAR_QUEUE_H

#include "../utils/nds_bool.h"
#include "../utils/unichar.h"
#include "../utils/nds_extension_macros.h"

#define UNICHAR_QUEUE_MAX_LENGTH 5

/**
 * A queue of unicode characters.
 *
 * The maximal number of queued characters is UNICHAR_QUEUE_MAX_LENGTH.
 * The queue supports peeking. The peek index is always between the tail
 * (where reading occurs) and head (where writing occurs).
 *
 * The queue is not resizable. Attempts to store more elements in the
 * queue than UNICHAR_QUEUE_MAX_LENGTH fail.
 */
typedef struct
{
    unichar buffer[UNICHAR_QUEUE_MAX_LENGTH];
    int     head;   /**< Index of the next slot where input data is to be stored. Always a number in the
                         range 0..UNICHAR_QUEUE_MAX_LENGTH-1. */
    int     tail;   /**< Index of the slot to be read next. -1 if buffer is empty. */
    int     peek;   /**< Index for the peek operation. */
} UnicharQueue;

NDS_EXTENSION_BEGIN_DECLS

/**
 * Initialize an instance of the queue.
 */
void UnicharQueue_init(UnicharQueue* self);

/**
 * Push a character into the queue.
 *
 * \param[in] self      Pointer to the instance.
 * \param[in] character Character to be stored.
 *
 * \return True if the character has been stored in the queue. False if the queue is full.
 */
nds_bool UnicharQueue_push(UnicharQueue* self, unichar character);

/**
 * Pop a character from the queue.
 *
 * \param[in] self      Pointer to the instance.
 * \param[in] character Pointer to a variable that receives the character.
 *
 * \return True if the character has been retrieved from the queue. False if the queue is empty.
 */
nds_bool UnicharQueue_pop(UnicharQueue* self, unichar* character);

/**
 * Retrieve the number of characters in the queue.
 *
 * \param[in] self Pointer to the instance.
 *
 * \return Number of characters in the queue.
 */
unsigned UnicharQueue_length(UnicharQueue const* self);

/**
 * Check if a queue is full.
 *
 * \param[in] self Pointer to the instance.
 *
 * \return True if the queue is full.
 */
nds_bool UnicharQueue_isFull(UnicharQueue const* self);

/**
 * Discard characters from the queue.
 *
 * \param[in] self  Pointer to the instance.
 * \param[in] count Number of characters to be popped off the queue.
 */
void UnicharQueue_consumeChars(UnicharQueue* self, unsigned count);

/**
 * Peek a character from the queue.
 *
 * The queue maintains a peek index. The index always lies
 * between the tail and the head index, advancing from
 * tail to head. Subsequent calls to UnicharQueue_peek()
 * return characters closer do the head. When the peek index
 * reaches head, further calls to UnicharQueue_peek() fail.
 *
 * If a peek index is equal to tail index, and the tail index
 * is advanced, the peek index is advanced as well.
 *
 * \param[in] self  Pointer to the instance.
 * \param[in] character Pointer to a variable that receives the character.
 *
 * \return True if a character has been retrieved.
 */
nds_bool UnicharQueue_peek(UnicharQueue* self, unichar* character);

/**
 * Resets the peek index.
 *
 * This sets the peek index to be the same as tail index. Subsequent
 * call to UnicharQueue_peek() will return a character at position
 * one before the tail (if there is a character available).
 *
 * \param[in] self  Pointer to the instance.
 */
void UnicharQueue_resetPeekIndex(UnicharQueue* self);

NDS_EXTENSION_END_DECLS

#endif /* UNICHAR_QUEUE_H */
