#ifndef COLLATION_ELEMENT_ITERATOR_H
#define COLLATION_ELEMENT_ITERATOR_H

#include "../utils/nds_bool.h"
#include "../utils/unichar_utils.h"
#include "lookahead_buffer.h"
#include "collation_data.h"
#include "../utils/nds_extension_macros.h"

/**
 * A forward iterator that returns collation elements based on collation data and a lookahead buffer.
 */
typedef struct
{
    CollationParseStyle         parseStyle;
    CollationTrieNode const*    collationTrie;
    SimpleCollationEntry const* simpleEntries;
    unsigned short              simpleEntryCount;
    LookaheadBuffer*            lookaheadBuffer;
    collation_element const*    currentCEArray;
    unsigned                    currentCEArrayLen;
} CollationElementIterator;

NDS_EXTENSION_BEGIN_DECLS

/**
 * Initialize an instance of CollationElementIterator.
 *
 * \param[in] self            The instance of CollationElementIterator.
 * \param[in] collationData   Description of the collation.
 * \param[in] lookaheadBuffer Buffer that is used as a source of characters.
 */
void CollationElementIterator_init(CollationElementIterator* self, CollationData const* collationData,
                                   LookaheadBuffer* lookaheadBuffer);

/**
 * Get the next collation element.
 *
 * \param[in]  self             The instance of CollationElementIterator.
 * \param[out] collationElement Pointer to a variable that received the collation element.
 *
 * \return True iff a collation element has been retrieved.
 */
nds_bool CollationElementIterator_getNext(CollationElementIterator* self, collation_element* collationElement);

NDS_EXTENSION_END_DECLS

#endif /* COLLATION_ELEMENT_ITERATOR_H */
