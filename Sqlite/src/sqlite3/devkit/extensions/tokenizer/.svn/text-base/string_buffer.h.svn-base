#ifndef STRING_BUFFER_H
#define STRING_BUFFER_H

#include <stdlib.h>
#include "../utils/utf8_utils.h"

/**
 * Size of the static buffer.
 *
 * This is tuned for use in the NDS unicode61 tokenizer for bigrams.
 *
 * Two characters, each can take up to 4 bytes (max. utf8 length of a unicode character)
 * plus each followed by one diacritic character.
 *
 * If more space is required, malloc() is used.
 */

#define STRING_BUFFER_STATIC_SIZE 2*2*4

/**
 * A growing buffer (queue) for utf8 characters.
 *
 * As long as STRING_BUFFER_STATIC_SIZE suffices, no memory is
 * allocated. If the contents grows, dynamic allocation is used.
 */
typedef struct
{
    /** Fixed-size static buffer. */
    char staticBuffer[STRING_BUFFER_STATIC_SIZE];

    /** Dynamically allocated buffer. NULL when not in use. */
    char *dynBuffer;

    /** Pointer to the buffer that is used - either staticBuffer or dynBuffer */
    char *buffer;

    /** Capacity of the buffer (in bytes). */
    size_t capacity;

    /** Size of the contents (in bytes). */
    size_t size;
} StringBuffer;

/**
 * Initialize an instance of the string buffer.
 *
 * \param[in] self        Pointer to the instance.
 * \return True if initialization succeeded.
 */
static void StringBuffer_init(StringBuffer* self)
{
    self->dynBuffer = NULL;
    self->buffer = self->staticBuffer;
    self->capacity = STRING_BUFFER_STATIC_SIZE;
    self->size = 0;
}

/**
 * Destroy an instance of the string buffer.
 *
 * \param[in] self Pointer to the instance.
 */
static void StringBuffer_destroy(StringBuffer* self)
{
    free(self->dynBuffer);
}

/**
 * Flush all contents of the string buffer.
 *
 * This does not change the capacity, but it
 * canges the size to 0.
 *
 * \param[in] self Pointer to the instance.
 */
static void StringBuffer_flush(StringBuffer* self)
{
    self->size = 0;
}

/**
 * Make sure there is at least etraBytes bytes available in the buffer.
 *
 * \param[in] self Pointer to the instance.
 * \param[in] extraBytes Number of bytes that must still fit in the buffer.
 * \return True unless the buffer had to be resized and the resizing failed.
 */
static nds_bool StringBuffer_ensureExtraCapacity(StringBuffer* self, size_t extraBytes)
{
    const size_t requiredCapacity = self->size + extraBytes;
    if (requiredCapacity > self->capacity)
    {
        char* newBuffer;
        size_t newCapacity = self->capacity;

        while (newCapacity < requiredCapacity)
            newCapacity *= 2;

        /*
         * This works both for 1st time reallocation (dynBuffer is NULL)
         * and for subsequent reallocation (dynBuffer is not NULL).
         */
        newBuffer = (char*)realloc(self->dynBuffer, newCapacity*sizeof(char));
        if (newBuffer == NULL)
            return NDS_FALSE; /* old buffer and capacity stay valid */

        self->dynBuffer = self->buffer = newBuffer;
        self->capacity = newCapacity;
    }

    return NDS_TRUE;
}

/**
 * Append the utf8 representation of a unicode character at the end of the stored string.
 *
 * \param[in] self Pointer to the instance.
 * \param[in] c The character to store.
 * \return True on sucess.
 */
static nds_bool StringBuffer_pushBackUnichar(StringBuffer* self, unichar c)
{
    char *current;

    if (!StringBuffer_ensureExtraCapacity(self, 4 /*max number of bytes for character*/))
        return NDS_FALSE;

    current = self->buffer + self->size;
    if (!WriteUnicharAsUtf8(c, &current, self->buffer + self->capacity))
        return NDS_FALSE;

    self->size = current - self->buffer;
    return NDS_TRUE;
}

/**
 * Retrieve the current buffer.
 *
 * \param[in]  self    Pointer to the instance.
 * \param[out] pBuffer Pointer to the variable that receives the address of the buffer.
 * \param[out] pLen    Pointer to the variable that received the length of the string
 *                     (in bytes).
 *
 * \note The returned values stay valid only until the contents of the buffer is changed.
 */
static void StringBuffer_getBuffer(StringBuffer const* self, char const** pBuffer, size_t* pLen)
{
    *pBuffer = self->buffer;
    *pLen = self->size;
}

/**
 * Return the number of bytes currently stored in the buffer.
 *
 * \param[in] self Pointer to the instance.
 * \return Number of bytes stored in the buffer.
 */
static size_t StringBuffer_getSize(StringBuffer const* self)
{
    return self->size;
}

/**
 * Remove given number of bytes from the start of the buffer.
 *
 * \param[in] self  Pointer to the instance.
 * \param[in] count Number of bytes to pop off the front of the buffer.
 *
 * There is an assert that checks that after the process the first byte
 * (if any left) is not an utf8 continuation byte (i.e. the pop has not
 * produced a broken utf8 string).
 */
static void StringBuffer_popFront(StringBuffer* self, size_t count)
{
    if (count > self->size)
        count = self->size;

    assert(count == self->size || !IsUtf8ContinuationByte(self->buffer[count]));
    self->size -= count;
    memmove(self->buffer, self->buffer+count, self->size);
}

#endif /* STRING_BUFFER_H */
