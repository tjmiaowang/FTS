
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "packerNDSC.h"

#ifndef CHAR_BIT
    #define CHAR_BIT 8
#endif

#define NSDC_HASH_LEN 4096  /* # of hash table entries (must be a power of 2) */


/* return maximum compressed length
 *
 */

unsigned int CalcNDSC( unsigned char *src_ptr, unsigned int src_len, int mode )
{
    return (src_len*9)/8+1; /* max. enlarging factor for NDSC is (9/8)+1 byte */
}


/* compress src_len bytes of src_ptr into dst_ptr
 * using NSDC_HASH_LEN entries in the hash table
 *
 * stores the length of the compressed data in dst_out
 *
 */

int PackNDSC( unsigned char *src_ptr, unsigned int src_len, unsigned char *dst_ptr, unsigned int dst_len, unsigned int *dst_out, int mode )
{
    unsigned char *src_ofs = src_ptr;
    unsigned char *src_end = src_ptr + src_len;
    unsigned char *cur_ofs, *lst_ofs, *pat_ofs, *end_ofs, *unc_ofs = NULL;
    unsigned char c;

    unsigned int hnum, cnt, gap, ext, hash, mcnt, mgap, mext;
    unsigned char *ctrl_ofs = dst_ptr;
    unsigned char ctrl_data = 0;
    unsigned int ctrl_cnt = 0;

    unsigned char *out_ofs = dst_ptr + sizeof(ctrl_data);
    unsigned char *dst_end = dst_ptr + dst_len;
    unsigned int i;

    unsigned char *(*hash_tbl)[];   /* hash table for quickly identifying matching patterns */

    if( (mode < 0) || (mode > 8) )
    {
        return( -1 );   /* unsupported compression mode */
    }

    hnum = 1 << mode;   /* # of hash pages per entry */

    if( hash_tbl = calloc(NSDC_HASH_LEN*hnum, sizeof(unsigned char *)) )
    {
        /* scan through the data stream */
        while( src_ofs < src_end )
        {
            /* check for overflow of dst_ptr */
            if( out_ofs >= dst_end )
            {
failure:
                free(hash_tbl);

                return( -1 );   /* output buffer too small */
            }

            cur_ofs = src_ofs;

            /* look for run-length */
            if( ((cur_ofs - src_ptr) & 1) != 0 )
            {
                src_ofs = cur_ofs;

                end_ofs = src_ofs+4096+3-1;

                if( src_end < end_ofs )
                {
                    end_ofs = src_end;
                }

                c = *(cur_ofs - 1);

                /* run-length scan core function */
                while( (src_ofs < end_ofs) && (*src_ofs == c) )
                {
                    src_ofs++;
                }

                /* decide whether it is beneficial to use run-length compression */
                if( (cnt = (unsigned int)(src_ofs - cur_ofs)) > 1 )
                {
                    /* examine extra char at the beginning of the run-length sequence */
                    cnt += ext = (unc_ofs != NULL) && ((cur_ofs - src_ptr) > 1) && (*(cur_ofs-2) == c) && (cnt < (4096+3-1));

                    /* test for minimum length */
                    if( cnt > 2 )
                    {
                        /* adjust starting position if necessary */
                        cur_ofs -= ext;

                        /* take care of uncompressable bytes */
                        if( unc_ofs != NULL )
                        {
                            while( unc_ofs < cur_ofs )
                            {
                                /* make room for the control bits */
                                if( ctrl_cnt++ == CHAR_BIT*sizeof(ctrl_data) )
                                {
                                    *ctrl_ofs = ctrl_data;
                                    ctrl_cnt = 1;
                                    ctrl_ofs = out_ofs;
                                    out_ofs += sizeof(ctrl_data);
                                }

                                if( (gap = (unsigned int)(cur_ofs - unc_ofs)) > 16 )
                                {
                                    if( gap > 4096+16-1 )
                                    {
                                        gap = 4096+16-1;
                                    }

                                    if( (out_ofs+2+gap) <= dst_end )
                                    {
                                        /* uncompressable */
                                        gap -= 16;
                                        *out_ofs++ = (0 << 4) + (gap >> 8);
                                        *out_ofs++ = gap;
                                        ctrl_data = (ctrl_data << 1) | 1;

                                        gap += 16;
                                        memcpy(out_ofs, unc_ofs, gap);
                                        unc_ofs += gap;
                                        out_ofs += gap;
                                    }
                                    else goto failure;
                                }
                                else
                                {
                                    if( out_ofs < dst_end )
                                    {
                                        /* copy uncompressable character */
                                        *out_ofs++ = *unc_ofs++;
                                        ctrl_data <<= 1;
                                    }
                                    else goto failure;
                                }
                            }

                            unc_ofs = NULL;
                        }

                        /* make room for the control bits */
                        if( ctrl_cnt++ == CHAR_BIT*sizeof(ctrl_data) )
                        {
                            *ctrl_ofs = ctrl_data;
                            ctrl_cnt = 1;
                            ctrl_ofs = out_ofs;
                            out_ofs += sizeof(ctrl_data);
                        }

                        cnt -= 3;

                        if( (out_ofs+2) <= dst_end )
                        {
                            /* run-length */
                            *out_ofs++ = (1 << 4) + (cnt >> 8);
                            *out_ofs++ = cnt;
                        }
                        else goto failure;

                        ctrl_data = (ctrl_data << 1) | 1;
                        continue;
                    }
                }
            }

            /* look for pattern */
            if( (src_end - cur_ofs) > 1 )
            {
                lst_ofs = cur_ofs;

                hash = ((lst_ofs[0] << 4) ^ lst_ofs[1]) & (NSDC_HASH_LEN - 1);

                if( ((cur_ofs - src_ptr) & 1) == 0 )
                {
                    for(i=0;i<hnum;i++)
                    {
                        pat_ofs = (*hash_tbl)[hash*hnum+i];
                        (*hash_tbl)[hash*hnum+i] = lst_ofs;
                        lst_ofs = pat_ofs;
                    }
                }
                else
                {
                    mcnt = 0;

                    for(i=0;i<hnum;i++)
                    {
                        pat_ofs = (*hash_tbl)[hash*hnum+i];
                        (*hash_tbl)[hash*hnum+i] = lst_ofs;

                        /* locate a possible pattern */
                        if( lst_ofs = pat_ofs )
                        {
                            /* compare characters if we are within the pattern scan range */
                            if( (cur_ofs - pat_ofs) < (4096+256+16-1) )
                            {
                                src_ofs = cur_ofs;

                                end_ofs = src_ofs+256+16-1;

                                if( src_end < end_ofs )
                                {
                                    end_ofs = src_end;
                                }

                                if( src_ofs+(cur_ofs - pat_ofs) < end_ofs )
                                {
                                    end_ofs = src_ofs+(cur_ofs - pat_ofs);
                                }

                                /* pattern scan core function */
                                while( (src_ofs < end_ofs) && (*src_ofs == *pat_ofs) )
                                {
                                    src_ofs++;
                                    pat_ofs++;
                                }

                                /* decide whether it is beneficial to use pattern compression */
                                if( (cnt = (unsigned int)(src_ofs - cur_ofs)) > 1 )
                                {
                                    if( (gap = (unsigned int)(cur_ofs - pat_ofs)) < 4096 )
                                    {
                                        /* examine extra char at the beginning of the pattern sequence */
                                        cnt += ext = (unc_ofs != NULL) && (gap > 0) && ((lst_ofs - src_ptr) > 0) && (*(cur_ofs-1) == *(lst_ofs-1)) && (cnt < (256+16-1));

                                        /* find matching pattern with maximum length */
                                        if( cnt > mcnt )
                                        {
                                            mcnt = cnt;
                                            mgap = gap - ext;
                                            mext = ext;
                                        }
                                    }
                                }
                            }
                            else
                            {
                                break;  /* the distance of the remaining patterns is too large */
                            }
                        }
                    }

                    /* test for minimum length */
                    if( mcnt > 2 )
                    {
                        /* ignore short patterns in uncompressable regions (favors high decompression speed over compressed file size) */
                        if( (unc_ofs == NULL) || (mcnt > 5) )
                        {
                            /* adjust starting position if necessary */
                            cur_ofs -= mext;

                            src_ofs = cur_ofs + mcnt;

                            /* take care of uncompressable bytes */
                            if( unc_ofs != NULL )
                            {
                                while( unc_ofs < cur_ofs )
                                {
                                    /* make room for the control bits */
                                    if( ctrl_cnt++ == CHAR_BIT*sizeof(ctrl_data) )
                                    {
                                        *ctrl_ofs = ctrl_data;
                                        ctrl_cnt = 1;
                                        ctrl_ofs = out_ofs;
                                        out_ofs += sizeof(ctrl_data);
                                    }

                                    if( (gap = (unsigned int)(cur_ofs - unc_ofs)) > 16 )
                                    {
                                        if( gap > 4096+16-1 )
                                        {
                                            gap = 4096+16-1;
                                        }

                                        if( (out_ofs+2+gap) <= dst_end )
                                        {
                                            /* uncompressable */
                                            gap -= 16;
                                            *out_ofs++ = (0 << 4) + (gap >> 8);
                                            *out_ofs++ = gap;
                                            ctrl_data = (ctrl_data << 1) | 1;

                                            gap += 16;
                                            memcpy(out_ofs, unc_ofs, gap);
                                            unc_ofs += gap;
                                            out_ofs += gap;
                                        }
                                        else goto failure;
                                    }
                                    else
                                    {
                                        if( out_ofs < dst_end )
                                        {
                                            /* copy uncompressable character */
                                            *out_ofs++ = *unc_ofs++;
                                            ctrl_data <<= 1;
                                        }
                                        else goto failure;
                                    }
                                }

                                unc_ofs = NULL;
                            }

                            /* make room for the control bits */
                            if( ctrl_cnt++ == CHAR_BIT*sizeof(ctrl_data) )
                            {
                                *ctrl_ofs = ctrl_data;
                                ctrl_cnt = 1;
                                ctrl_ofs = out_ofs;
                                out_ofs += sizeof(ctrl_data);
                            }

                            if( mcnt < 16 )
                            {
                                if( (out_ofs+2) <= dst_end )
                                {
                                    /* short pattern */
                                    *out_ofs++ = (mcnt << 4) + (mgap >> 8);
                                    *out_ofs++ = mgap;
                                }
                                else goto failure;
                            }
                            else
                            {
                                if( (out_ofs+3) <= dst_end )
                                {
                                    /* long pattern */
                                    *out_ofs++ = (2 << 4) + (mgap >> 8);
                                    *out_ofs++ = mgap;
                                    *out_ofs++ = mcnt - 16;
                                }
                                else goto failure;
                            }

                            ctrl_data = (ctrl_data << 1) | 1;
                            continue;
                        }
                    }
                }
            }

            if( unc_ofs == NULL )
            {
                /* start an uncompressable block */
                unc_ofs = cur_ofs;
            }

            /* uncompressable character */
            cur_ofs++;

            src_ofs = cur_ofs;
        }

        /* save last uncompressable bytes */
        if( unc_ofs != NULL )
        {
            while( unc_ofs < cur_ofs )
            {
                /* make room for the control bits */
                if( ctrl_cnt++ == CHAR_BIT*sizeof(ctrl_data) )
                {
                    *ctrl_ofs = ctrl_data;
                    ctrl_cnt = 1;
                    ctrl_ofs = out_ofs;
                    out_ofs += sizeof(ctrl_data);
                }

                if( (gap = (unsigned int)(cur_ofs - unc_ofs)) > 16 )
                {
                    if( gap > 4096+16-1 )
                    {
                        gap = 4096+16-1;
                    }

                    if( (out_ofs+2+gap) <= dst_end )
                    {
                        /* uncompressable */
                        gap -= 16;
                        *out_ofs++ = (0 << 4) + (gap >> 8);
                        *out_ofs++ = gap;
                        ctrl_data = (ctrl_data << 1) | 1;

                        gap += 16;
                        memcpy(out_ofs, unc_ofs, gap);
                        unc_ofs += gap;
                        out_ofs += gap;
                    }
                    else goto failure;
                }
                else
                {
                    if( out_ofs < dst_end )
                    {
                        /* copy uncompressable character */
                        *out_ofs++ = *unc_ofs++;
                        ctrl_data <<= 1;
                    }
                    else goto failure;
                }
            }

            unc_ofs = NULL;
        }

        if( ctrl_cnt != 0 )
        {
            /* save last control data */
            ctrl_data <<= (CHAR_BIT*sizeof(ctrl_data) - ctrl_cnt);
            *ctrl_ofs = ctrl_data;

            /* size of the compressed data */
            *dst_out = (unsigned int)(out_ofs - dst_ptr);
        }
        else
        {
            *dst_out = 0;
        }

        free(hash_tbl);

        return( 0 );    /* success */
    }

    return( -1 );   /* failure */
}

/* decompress src_len bytes of src_ptr into dst_ptr
 *
 */

int UnpackNDSC( unsigned char *src_ptr, unsigned int src_len, unsigned char *dst_ptr, unsigned int dst_len )
{
    unsigned char ctrl_data;
    unsigned int ctrl_mask = 0;
    unsigned char *src_ofs = src_ptr;
    unsigned char *src_end = src_ptr + src_len;
    unsigned char *dst_ofs = dst_ptr;
    unsigned char *dst_end = dst_ptr + dst_len;
    unsigned int cmd;
    unsigned int cnt;

    /* process the compressed data stream */
    while( src_ofs < src_end )
    {
        if( dst_ofs >= dst_end )
        {
failure:
            return( -1 );   /* decompression failed (data corrupt, output buffer too small, etc.) */
        }

        /* get new control data if needed */
        if( (ctrl_mask >>= 1) == 0 )
        {
            ctrl_data = *src_ofs++;
            ctrl_mask = 1 << (CHAR_BIT*sizeof(ctrl_data) - 1);
        }

        if( (ctrl_data & ctrl_mask) == 0 )
        {
            if( src_ofs < src_end )
            {
                /* copy a char if the control bit is zero */
                *dst_ofs++ = *src_ofs++;
            }
        }
        else
        {
            if( (src_ofs + 1) < src_end )
            {
                /* get uncompression information */
                cmd = (*src_ofs >> 4) & 0x0F;
                cnt = *src_ofs++ & 0x0F;
                cnt <<= 8;
                cnt += *src_ofs++;

                switch( cmd )
                {
                    case 0: /* uncompressable */
                        cnt += 16;
                        if( ((src_ofs + cnt) <= src_end) && ((dst_ofs + cnt) <= dst_end) )
                        {
                            memcpy(dst_ofs, src_ofs, cnt);
                        }
                        else goto failure;
                        src_ofs += cnt;
                        dst_ofs += cnt;
                        break;

                    case 1: /* run-length */
                        cnt += 3;
                        if( ((dst_ofs - 1) >= dst_ptr) && ((dst_ofs + cnt) <= dst_end) )
                        {
                            memset(dst_ofs, *(dst_ofs - 1), cnt);
                        }
                        else goto failure;
                        dst_ofs += cnt;
                        break;

                    case 2: /* long pattern */
                        if( src_ofs < src_end )
                        {
                            cmd = *src_ofs++;
                            cmd += 16;
                            if( ((dst_ofs - cnt - cmd) >= dst_ptr) && ((dst_ofs + cmd) <= dst_end) )
                            {
                                memcpy(dst_ofs, dst_ofs - cnt - cmd, cmd);
                            }
                            else goto failure;
                            dst_ofs += cmd;
                        }
                        break;

                    default:    /* short pattern */
                        if( ((dst_ofs - cnt - cmd) >= dst_ptr) && ((dst_ofs + cmd) <= dst_end) )
                        {
                            memcpy(dst_ofs, dst_ofs - cnt - cmd, cmd);
                        }
                        else goto failure;
                        dst_ofs += cmd;
                        break;
                }
            }
        }
    }

    /* test source buffer offset */
    if( src_ofs != src_end )
    {
        return( -1 );   /* corrupt file */
    }

    /* test destination buffer offset */
    if( dst_ofs != dst_end )
    {
        return( -1 );   /* corrupt file */
    }

    return( 0 );    /* success */
}

