#ifndef UNICHAR_UTILS_H
#define UNICHAR_UTILS_H

#include "../utils/nds_bool.h"
#include "unichar.h"
#include "nds_extension_macros.h"

NDS_EXTENSION_BEGIN_DECLS

/**
 * Test if a unicode character is a combining mark.
 *
 * Examples of combining marks include diaeresis (umlaut) or caron (hacek).
 */
nds_bool UnicharIsCombiningMark(unichar character);

/**
 * Decompose a unicode character.
 *
 * This performs a subset of unicode NFKD normalization.
 *
 * \param[in] character         Unicode character to be decomposed.
 * \param[out] decomposed       Address of a pointer that receives the decomposition.
 * \param[out] decompositionLen Address of a variable that receives the number of elements present
 *                              in the decomposition.
 *
 * \return True if a character can be decomposed. False if a character is not decomposable.
 */
nds_bool UnicharDecompose(unichar character, unichar const** decomposed, unsigned* decompositionLen);

NDS_EXTENSION_END_DECLS

#endif /* UNICHAR_UTILS_H */
