#include <stddef.h>
#include <assert.h>

#include "unichar_queue.h"

static int incrementIndex(int index, int increment_count)
{
    int next_index = index + increment_count;
    if (next_index >= UNICHAR_QUEUE_MAX_LENGTH)
        next_index = 0;

    return next_index;
}

static int nextIndex(int index)
{
    return incrementIndex(index, 1);
}

void UnicharQueue_init(UnicharQueue* self)
{
    /* buffer is empty */
    self->head = 0;
    self->tail = -1;
    self->peek = -1;

    assert(UnicharQueue_length(self) == 0);
}

nds_bool UnicharQueue_push(UnicharQueue* self, unichar character)
{
    if (self->head == self->tail)
        return NDS_FALSE; /* queue is full */

    assert(UnicharQueue_length(self) < UNICHAR_QUEUE_MAX_LENGTH);

    self->buffer[self->head] = character;

    /* if queue was marked as empty, set tail to head */
    if (self->tail < 0)
        self->tail = self->head;

    if (self->peek < 0)
        self->peek = self->head;

    /* advance head */
    self->head = nextIndex(self->head);

    return NDS_TRUE;
}

nds_bool UnicharQueue_pop(UnicharQueue* self, unichar * character)
{
    if (character == NULL)
        return NDS_FALSE;

    if (self->tail < 0)
        return NDS_FALSE; /* queue is empty */

    assert(UnicharQueue_length(self) > 0);

    *character = self->buffer[self->tail];

    if (self->peek == self->tail)
        self->peek = nextIndex(self->peek);

    self->tail = nextIndex(self->tail);

    if (self->tail == self->head)
        self->tail = -1; /* mark as empty */

    if (self->peek == self->head)
        self->peek = -1;

    return NDS_TRUE;
}

unsigned UnicharQueue_length(UnicharQueue const* self)
{
    if (self->tail < 0)
        return 0; /* queue is empty */

    /* head before tail */
    if (self->tail < self->head)
        return self->head - self->tail;

    /* tail before head -> wrapped around */
    return UNICHAR_QUEUE_MAX_LENGTH - (self->tail - self->head);
}

nds_bool UnicharQueue_isFull(UnicharQueue const* self)
{
    return self->head == self->tail;
}

void UnicharQueue_consumeChars(UnicharQueue* self, unsigned count)
{
    unsigned len = UnicharQueue_length(self);
    if (count > len)
        count = len;

    if (count > 0)
    {
        self->tail = incrementIndex(self->tail, count);

        if (self->tail == self->head)
            self->tail = -1;
    }
}

nds_bool UnicharQueue_peek(UnicharQueue* self, unichar* character)
{
    if (character == NULL)
        return NDS_FALSE;

    if (self->peek < 0)
        return NDS_FALSE;

    *character = self->buffer[self->peek];

    self->peek = nextIndex(self->peek);
    if (self->peek == self->head)
        self->peek = -1;

    return NDS_TRUE;
}

void UnicharQueue_resetPeekIndex(UnicharQueue* self)
{
    self->peek = self->tail;
}
