#ifndef UNICHAR_DECOMPOSITION_DATA_H
#define UNICHAR_DECOMPOSITION_DATA_H

#include "unichar.h"
#include "nds_extension_macros.h"

/**
 * Structure describing a single continuous range of data for unicode decomposition.
 */
typedef struct
{
    /**
     * First character included in this range.
     */
    unichar first;

    /**
     * Last character included in this range.
     */
    unichar last;

    /**
     * Pointer to an array of decomposed characters.
     *
     * The first entry contains the length of the decomposition,
     * followed by the actual decomposed characters.
     */
    unichar const* const* data;
} UnicodeDecompositionRange;

NDS_EXTENSION_BEGIN_DECLS

extern const UnicodeDecompositionRange UnicharDecompositionData[];
extern const unsigned                  UnicharDecompositionDataCount;

NDS_EXTENSION_END_DECLS

#endif /* UNICHAR_DECOMPOSITION_DATA_H */
