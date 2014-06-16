#ifndef COLLATION_DATA_H
#define COLLATION_DATA_H

#include "../utils/nds_int.h"
#include "../utils/unichar_utils.h"

/**
 * Collation element that is used for input that doesn't map to anything else.
 */
#define UNDEFINED_CE ((collation_element) 0)

/**
 * An enum selecting how combining marks should be treated.
 */
typedef enum
{
    /**
     * Combining marks in input must exactly match that of the collation trie.
     *
     * This means that e.g. "C combining_caron" will not match "C" in the trie.
     * (For a match to be made, "C" would have to have a child "combining_caron".
     * Then that "combining_caron" node would be matched.)
     */
    CollationCombiningMarkExact,

    /**
     * Combining marks in input not present in the trie are ignored.
     *
     * This means that e.g. "C combining_caron" will match "C" in the trie. (If
     * the "C" trie node would have a "combining_caron" child then it would be
     * used. If not, the "C" node would be used.)
     */
    CollationCombiningMarkSubset
} CollationParseStyle;

/**
 * Type for collation elements.
 *
 * Input strings are converted to strings of collations elements and these
 * are then used for the actual comparison.
 */
typedef nds_uint32 collation_element;

/**
 * A node in the collation trie.
 *
 * \note The order of the elements needs to match the code of the python generator.
 */
typedef struct tagCollationTrieNode
{
    /*
     * The order of the elements is chosen so that there
     * is no needless padding between the fields and at the end.
     */

    /**
     * Pointer to collation elements this node should be translated into.
     *
     * \note collationElements can be NULL. In such case the node itself can
     * never be matched, but its children can. Consider e.g. the sorting rule
     * "abc" is assigned a collation element, but "ab" is not ("ab" should be
     * sorted as "a", "b").
     */
    collation_element const* collationElements;

    /**
     * Pointer to children of this node.
     *
     * \note This is non-NULL if inspection of further characters
     * is needed to determine the collation elements the input
     * maps to. Consider e.g. Czech "ch" which sorts as a single
     * character. Then the entry for "c" has an entry for "h" as
     * its child.
     */
    struct tagCollationTrieNode const* children;

    /**
     * Number of elements collationElements points to.
     */
    unsigned short collationElementCount;

    /**
     * Number of elements children points to.
     */
    unsigned short childCount;

    /**
     * Character this node describes.
     */
    unichar character;

} CollationTrieNode;

/**
 * Data for simple entry.
 * \note The order of the elements needs to match the code of the python generator.
 */
typedef struct
{
    unichar           character;
    collation_element collationElement;
} SimpleCollationEntry;

/**
 * Data describing a collation.
 * \note The order of the elements needs to match the code of the python generator.
 */
typedef struct tagNdsCollationData
{
    CollationParseStyle         parseStyle;
    CollationTrieNode const*    trie;

    SimpleCollationEntry const* simpleEntries;
    unsigned short              simpleEntryCount;
} CollationData;

#endif /* COLLATION_DATA_H */
