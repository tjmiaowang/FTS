#include <stddef.h>
#include "test_unichar_queue.h"
#include "extensions_test.h"

#include "../collations/unichar_queue.h"

void TestUnicharQueue()
{
    UnicharQueue q;
    UnicharQueue_init(&q);

    unichar c;

    // pop from an empty queue
    EXPECT_EQ(false, UnicharQueue_pop(&q, &c));

    // store character and retrieve it
    static const unichar TEST_CHAR = 123;
    EXPECT_EQ(true, UnicharQueue_push(&q, TEST_CHAR));
    EXPECT_EQ(true, UnicharQueue_pop(&q, &c));
    EXPECT_EQ(c, TEST_CHAR);

    // queue must be empty now
    EXPECT_EQ(false, UnicharQueue_pop(&q, &c));

    // write and read UNICHAR_QUEUE_MAX_LENGTH characters
    for (unichar i = 0; i < UNICHAR_QUEUE_MAX_LENGTH; i++)
        EXPECT_EQ(true, UnicharQueue_push(&q, i));

    // no more space, next write must fail
    EXPECT_EQ(false, UnicharQueue_push(&q, 0));

    // read back what was written
    for (unichar i = 0; i < UNICHAR_QUEUE_MAX_LENGTH; i++)
    {
        EXPECT_EQ(true, UnicharQueue_pop(&q, &c));
        EXPECT_EQ(c, i);
    }

    // no more data - next read must fail
    EXPECT_EQ(false, UnicharQueue_pop(&q, &c));
}

void TestUnicharQueue_NullArgs()
{
    UnicharQueue q;
    UnicharQueue_init(&q);

    static const unichar TEST_CHAR = 123;
    EXPECT_EQ(true, UnicharQueue_push(&q, TEST_CHAR));
    EXPECT_EQ(false, UnicharQueue_pop(&q, NULL));
    EXPECT_EQ(false, UnicharQueue_peek(&q, NULL));
}
