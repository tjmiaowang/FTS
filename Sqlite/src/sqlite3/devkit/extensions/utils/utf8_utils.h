#ifndef UTF8_UTILS_H
#define UTF8_UTILS_H

#include "nds_bool.h"
#include "unichar.h"
#include "nds_extension_macros.h"

NDS_EXTENSION_BEGIN_DECLS

/**
 * Retrieves next unicode character from an utf8-encoded string.
 *
 * pStr and strLen are updated so that DecodeNextUtf8CharToUnichar can be called again
 * to retrieve the following character. This can be used to iterate over all the
 * characters contained in the string.
 *
 * \param[in,out] pStr      Address of the pointer to the start of the utf-8 string. On return
 *                          the pointer is incremented to that it points to the next character.
 *
 * \param[in,out] strLen    Pointer to the length (in bytes) of the string *pStr points to.
 *                          The value is updated on return to reflect the number of bytes remaining.
 *
 * \param[out]    character Pointer to a variable that receives the character - if it can be decoded.
 *
 * \return Returns true iff a character was retrieved, false otherwise (i.e. when end of string
 * or an invalid UTF8 sequence have been reached).
 */
nds_bool DecodeNextUtf8CharToUnichar(char const** pStr, int* strLen, unichar* character);

/**
 * Returns the number of unicode characters contained in an utf8-encoded buffer.
 *
 * \param[in] str    Pointer to the beginning of the buffer.
 * \param[in] strLen Length (in bytes) of the buffer.
 *
 * \return Number of unicode characters that can be extracted from the buffer.
 *
 * \note This does not perform the full decoding. For an input that is not a valid UTF-8
 * string this can return a number different from what would be retrieved by subsequent calls to
 * DecodeNextUtf8CharToUnichar().
 */
unsigned GetUtf8StringCharCount(char const* str, int strLen);

/**
 * Writes a unicode character as an utf8-encoded string.
 *
 * pStr is updated so that WriteUnicharAsUtf8 can be called again to write
 * another character to the same string.
 *
 * \param[in] character The unicode character to write.
 * \param[in,out] pStr Pointer to the start of the buffer. On successful return
 *                     pStr is updated to point to the first byte after the end
 *                     of the encoded character.
 * \param[in] pEnd Pointer to one byte past the end of the buffer.
 *
 * \return True if the operation succeeded, false otherwise.
 */
nds_bool WriteUnicharAsUtf8(unichar character, char** pStr, const char* pEnd);

/**
 * Check if a byte is utf8 continuation byte.
 *
 * \return True iff the passed character is an utf8 continuation character.
 */
nds_bool IsUtf8ContinuationByte(char c);

NDS_EXTENSION_END_DECLS

#endif /* UTF8_UTILS_H */
