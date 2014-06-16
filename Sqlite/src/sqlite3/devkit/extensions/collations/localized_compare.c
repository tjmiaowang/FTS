#include <stddef.h>

#include "../utils/nds_bool.h"
#include "localized_compare.h"
#include "utf8_decompose_iterator.h"
#include "lookahead_buffer.h"
#include "collation_element_iterator.h"

int LocalizedCompare(CollationData const* cd, char const* str1, int str1Len, char const* str2, int str2Len)
{
    Utf8DecomposeIterator decomposeIterator1, decomposeIterator2;
    LookaheadBuffer lookaheadBuffer1, lookaheadBuffer2;
    CollationElementIterator collationElementIterator1, collationElementIterator2;
    collation_element collationElement1, collationElement2;
    nds_bool string1Read, string2Read;

    if (cd == NULL)
        return 0;

    if (str1Len < 0)
        str1Len = 0;

    if (str2Len < 0)
        str2Len = 0;

    if (str1 == NULL)
        str1 = "";

    if (str2 == NULL)
        str2 = "";

    Utf8DecomposeIterator_init(&decomposeIterator1, str1, str1Len);
    Utf8DecomposeIterator_init(&decomposeIterator2, str2, str2Len);

    LookaheadBuffer_init(&lookaheadBuffer1, &decomposeIterator1);
    LookaheadBuffer_init(&lookaheadBuffer2, &decomposeIterator2);

    CollationElementIterator_init(&collationElementIterator1, cd, &lookaheadBuffer1);
    CollationElementIterator_init(&collationElementIterator2, cd, &lookaheadBuffer2);

    string1Read = CollationElementIterator_getNext(&collationElementIterator1, &collationElement1);
    string2Read = CollationElementIterator_getNext(&collationElementIterator2, &collationElement2);

    while (string1Read && string2Read)
    {
        if (collationElement1 != collationElement2)
            return collationElement1 < collationElement2 ? -1 : +1;

        string1Read = CollationElementIterator_getNext(&collationElementIterator1, &collationElement1);
        string2Read = CollationElementIterator_getNext(&collationElementIterator2, &collationElement2);
    }

    if (!string1Read && !string2Read)
        return 0;

    if (!string1Read)
        return -1;
    else /* !string2Read */
        return +1;
}
