#include <stddef.h>

#include "lookahead_buffer.h"

void LookaheadBuffer_init(LookaheadBuffer* self, Utf8DecomposeIterator* it)
{
    self->it = it;
    UnicharQueue_init(&self->queue);
}

nds_bool LookaheadBuffer_getNextChar(LookaheadBuffer* self, unichar* character)
{
    if (character == NULL || self->it == NULL)
        return NDS_FALSE;

    if (UnicharQueue_pop(&self->queue, character))
        return NDS_TRUE;
    else
        return Utf8DecomposeIterator_getNextCharacter(self->it, character);
}

nds_bool LookaheadBuffer_peekNextChar(LookaheadBuffer* self, unichar* character)
{
    if (character == NULL || self->it == NULL)
        return NDS_FALSE;

    if (UnicharQueue_peek(&self->queue, character))
        return NDS_TRUE;

    if(!UnicharQueue_isFull(&self->queue))
    {
        if (Utf8DecomposeIterator_getNextCharacter(self->it, character))
        {
            nds_bool ok = UnicharQueue_push(&self->queue, *character);
            if (!ok)
            {
                /* this is bad: the character has been read from input but storing failed */
                return NDS_FALSE;
            }

            /*
             * advance peek index
             * this should always succeed as a character has just been pushed there
             */
            return UnicharQueue_peek(&self->queue, character);
        }
    }

    return NDS_FALSE;
}

void LookaheadBuffer_resetPeekPointer(LookaheadBuffer* self)
{
    UnicharQueue_resetPeekIndex(&self->queue);
}

void LookaheadBuffer_skipChars(LookaheadBuffer* self, int count)
{
    unsigned len;
    unsigned toConsume;

    if (self->it == NULL || count <= 0)
        return;

    len = UnicharQueue_length(&self->queue);

    toConsume = count;
    if (toConsume > len)
        toConsume = len;

    UnicharQueue_consumeChars(&self->queue, toConsume);
    count -= toConsume;

    while (count > 0)
    {
        unichar dummy;
        if (!Utf8DecomposeIterator_getNextCharacter(self->it, &dummy))
            return;
    }
}
