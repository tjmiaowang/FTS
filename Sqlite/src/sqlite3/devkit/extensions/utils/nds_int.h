#ifndef NDS_INT_H
#define NDS_INT_H

/*
 * Ideally we would use stdint.h, but that is not available in MSVC.
 */

/*
 * Only define the types we actually need.
 *
 * This is valid on all the supported platforms.
 */
typedef unsigned char nds_uint8;
typedef unsigned int nds_uint32;

#endif /* NDS_INT_H */
