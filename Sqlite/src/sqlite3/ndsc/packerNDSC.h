#if !defined(__packerNDSC_h)
#define __packerNDSC_h


unsigned int CalcNDSC( unsigned char *src_ptr, unsigned int src_len, int mode );
int PackNDSC( unsigned char *src_ptr, unsigned int src_len, unsigned char *dst_ptr, unsigned int dst_len, unsigned int *dst_out, int mode );
int UnpackNDSC( unsigned char *src_ptr, unsigned int src_len, unsigned char *dst_ptr, unsigned int dst_len );

#endif

