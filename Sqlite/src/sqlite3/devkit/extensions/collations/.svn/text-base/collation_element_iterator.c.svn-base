#include <stddef.h>

#include "collation_element_iterator.h"
#include "../utils/utf8_utils.h"

static const collation_element UndefinedCE = UNDEFINED_CE;

static CollationTrieNode const* CollationElementIterator_findTrieNode(CollationTrieNode const* trie,
                                                                      unichar searched_char)
{
    int first = 0;
    int last = (int) (trie->childCount) - 1;
    while (first <= last)
    {
        int middle = first + (last - first) / 2;
        CollationTrieNode const* checked_node = &trie->children[middle];
        const unichar checked_char = checked_node->character;
        if (searched_char < checked_char)
            last = middle - 1;
        else if (searched_char > checked_char)
            first = middle + 1;
        else
            return checked_node;
    }

    return NULL;
}

static void CollationElementIterator_skipCombiningMarks(CollationElementIterator* self)
{
    unichar c;
    while (LookaheadBuffer_peekNextChar(self->lookaheadBuffer, &c) && UnicharIsCombiningMark(c))
    {
        LookaheadBuffer_skipChars(self->lookaheadBuffer, 1);
    }
}

static nds_bool CollationElementIterator_getNextCEArrayFromTrie(CollationElementIterator* self)
{
    CollationTrieNode const* node = NULL;
    CollationTrieNode const* lastValid = NULL;
    int lastValidDepth = 0;
    int currentDepth = 0;
    unichar c;

    if (self->collationTrie == NULL)
        return NDS_FALSE;

    if (self->lookaheadBuffer == NULL || !LookaheadBuffer_getNextChar(self->lookaheadBuffer, &c))
        return NDS_FALSE;

    node = CollationElementIterator_findTrieNode(self->collationTrie, c);
    while (node != NULL)
    {
        CollationTrieNode const* next;
        const nds_bool isNodeValid = (node->collationElements != NULL);

        if (isNodeValid)
        {
            lastValid = node;
            lastValidDepth = currentDepth;
        }

        if (!LookaheadBuffer_peekNextChar(self->lookaheadBuffer, &c))
            break;

        currentDepth++;
        next = CollationElementIterator_findTrieNode(node, c);

        if (next == NULL && self->parseStyle == CollationCombiningMarkSubset &&
                UnicharIsCombiningMark(c))
        {
            /* go on searching for subsequent combining marks, stay at current node */
            if (isNodeValid)
                lastValidDepth = currentDepth;
        }
        else
        {
            node = next;
        }
    }

    if (lastValid != NULL)
    {
        LookaheadBuffer_skipChars(self->lookaheadBuffer, lastValidDepth);
        self->currentCEArray = lastValid->collationElements;
        self->currentCEArrayLen = lastValid->collationElementCount;
    }
    else
    {
        /* return a pointer to undefined collation element */
        self->currentCEArray = &UndefinedCE;
        self->currentCEArrayLen = 1;

        if (self->parseStyle == CollationCombiningMarkSubset)
            CollationElementIterator_skipCombiningMarks(self);
    }
    LookaheadBuffer_resetPeekPointer(self->lookaheadBuffer);

    return NDS_TRUE;
}

static SimpleCollationEntry const* CollationElementIterator_findSimpleEntry(CollationElementIterator* self,
                                                                            unichar searched_char)
{
    int first = 0;
    int last = (int) (self->simpleEntryCount) - 1;
    while (first <= last)
    {
        int middle = first + (last - first) / 2;
        SimpleCollationEntry const* checked_entry = &self->simpleEntries[middle];
        const unichar checked_char = checked_entry->character;

        if (searched_char < checked_char)
            last = middle - 1;
        else if (searched_char > checked_char)
            first = middle + 1;
        else
            return checked_entry;
    }

    return NULL;
}

static nds_bool CollationElementIterator_getNextCEArrayFromSimpleEntries(CollationElementIterator* self)
{
    unichar c;
    SimpleCollationEntry const* entry;

    if (self->simpleEntries == NULL)
        return NDS_FALSE;

    if (self->lookaheadBuffer == NULL || !LookaheadBuffer_peekNextChar(self->lookaheadBuffer, &c))
        return NDS_FALSE;

    entry = CollationElementIterator_findSimpleEntry(self, c);
    if (entry == NULL)
    {
        /* undo the peek */
        LookaheadBuffer_resetPeekPointer(self->lookaheadBuffer);
        return NDS_FALSE;
    }

    /* consume the peeked character */
    LookaheadBuffer_skipChars(self->lookaheadBuffer, 1);

    if (self->parseStyle == CollationCombiningMarkSubset)
    {
        /* eat up all combining accents in the input */
        CollationElementIterator_skipCombiningMarks(self);

        /*
         * CollationElementIterator_skipCombiningMarks() can leave peeked characters
         * so reset the peek pointer here
         */
        LookaheadBuffer_resetPeekPointer(self->lookaheadBuffer);
    }

    /* set results */
    self->currentCEArray = &entry->collationElement;
    self->currentCEArrayLen = 1;

    return NDS_TRUE;
}

static nds_bool CollationElementIterator_getNextCEArray(CollationElementIterator* self)
{
    /* try simple elements first */
    if (CollationElementIterator_getNextCEArrayFromSimpleEntries(self))
        return NDS_TRUE;

    /* fall back to the full-featured trie */
    return CollationElementIterator_getNextCEArrayFromTrie(self);
}

void CollationElementIterator_init(CollationElementIterator* self, CollationData const* collationData,
                                   LookaheadBuffer* lookaheadBuffer)
{
    if (collationData != NULL)
    {
        self->parseStyle = collationData->parseStyle;
        self->collationTrie = collationData->trie;
        self->simpleEntries = collationData->simpleEntries;
        self->simpleEntryCount = collationData->simpleEntryCount;
    }
    else
    {
        self->parseStyle = CollationCombiningMarkExact;
        self->collationTrie = NULL;
        self->simpleEntries = NULL;
        self->simpleEntryCount = 0;
    }

    self->lookaheadBuffer = lookaheadBuffer;
    self->currentCEArray = NULL;
    self->currentCEArrayLen = 0;
}

nds_bool CollationElementIterator_getNext(CollationElementIterator* self, collation_element* collationElement)
{
    if (collationElement == NULL || self->collationTrie == NULL || self->lookaheadBuffer == NULL)
        return NDS_FALSE;

    if (self->currentCEArrayLen == 0)
    {
        if (!CollationElementIterator_getNextCEArray(self))
            return NDS_FALSE;
    }

    *collationElement = *self->currentCEArray;
    self->currentCEArray++;
    self->currentCEArrayLen--;
    return NDS_TRUE;
}
