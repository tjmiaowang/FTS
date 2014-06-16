#ifndef LOCALIZED_COMPARE_H
#define LOCALIZED_COMPARE_H

#include "collation_data.h"
#include "../utils/nds_extension_macros.h"

NDS_EXTENSION_BEGIN_DECLS

/**
 * Compare two utf8-encoded strings using given collation data.
 *
 * \param[in] cd      Collation data to use for the comparison.
 * \param[in] str1    Pointer to the first string to compare.
 * \param[in] str1Len Length (in bytes) of the first string.
 * \param[in] str2    Pointer to the second string to compare.
 * \param[in] str2Len Length (in bytes) of the second string.
 *
 * \return 0 if the strings are equal, a number lower than 0 if the first string is lower or
 * a number greater than 0 if the second string is lower.
 */
int LocalizedCompare(CollationData const* cd, char const* str1, int str1Len, char const* str2, int str2Len);

NDS_EXTENSION_END_DECLS

#endif /* LOCALIZED_COMPARE_H */
