/*
** This version of SQLite is specially prepared for the
** Navigation Data Standard e.V.  Use by license only.
**
** This file contains code use to:
**
**   (1) Implement compression and decompression of database pages
**   (2) Implement encryption and decryption of database pages
**   (3) Determine which compression and encryption algorithms are to
**       be used for a particular database.
**
** The key component of this file is the nds_compression_algorithm_detector()
** function implemented near the bottom. All the other routines in this file
** merely support the nds_compression_algorithm_detector() function. If you
** are reading this file for the first time, and trying to understand what
** is happening, it is suggested that you begin at the bottom with the
** nds_compression_algorithm_detector() function and then work your way
** upward.
**
** Here are the compression functions implemented by this module:
**
**    zlib    This method uses the world-famous zLib compressor - the
**            same compression technology used in ZIP archives and in gzip.
**            Unlike the other compression methods above, whose entire
**            implementation is contained within this one file, the
**            compression and decompression routines are in an external
**            library. The code in this file merely invokes the external
**            library. This compression method is only included if this
**            file is compiled with the NDS_ENABLE_ZLIB macro defined.
**
**    lz4     This method uses the famous LZ4 compression. The code
**            in this file merely invokes the external library. This compression
**            method is only included if this file is compiled with the
**            NDS_ENABLE_LZ4 macro defined.
**
**    lz4hc   This method is high compression alternative of LZ4. It uses
**            the same decompression routine as the lz4 compression method.
**            The code in this file merely invokes the external library. This
**            compression method is only included if this file is compiled
**            with the NDS_ENABLE_LZ4 macro defined.
**
**    ndsc    This method uses NDS internal compression algorithm. The code
**            in this file merely invokes the external library. This compression
**            method is only included if this file is compiled with the
**            NDS_ENABLE_NDSC macro defined.
**
**    bsr     "Blank Space Removal" - Many SQLite pages contain large spans
**            of zero bytes. This compression method searches for the single
**            longest span of zeros within each page and removes it.
**
** The ZipVFS extension is not compelled to do compression on the database.
** It can also simply pass through the file content, resulting in an
** uncompressed database file that can be read and written by ordinary
** unextended versions of SQLite. Some people think of the no-compression
** option as a fifth form of compression. But are there no compression
** and decompression routines associated with the no-compression option.
**
** In addition to doing compression/decompression, the ZipVFS module is 
** also able to do encryption/decryption using Rijndael AES encryption
** algorithm. This encryption is only included if this file is compiled with
** the NDS_ENABLE_AES macro defined.
*/
#include "nds_sqlite3.h"
#include <string.h>
#include <assert.h>
#ifdef NDS_ENABLE_ZLIB
# include <zlib.h>
#endif
#ifdef NDS_ENABLE_LZ4
# include <lz4.h>
# include <lz4hc.h>
#endif
#ifdef NDS_ENABLE_NDSC
# include <packerNDSC.h>
#endif
#ifdef NDS_ENABLE_AES
# include "rijndael.h"
#endif

/*
** Forward declarations of structures
*/
typedef struct ZipvfsInst ZipvfsInst;
typedef struct ZipvfsAlgorithm ZipvfsAlgorithm;

/*
** Each open connection to a ZIPVFS database has an instance of the following
** structure which holds context information for that connection.  This
** structure is automatically allocated and initialized when the database
** connection is opened and automatically freed when the database connection
** is closed.
**
** The pCrypto field holds context information for the encryption/decryption
** logic.  For the demo encryption shown in this file, the pCrypto field
** is just a pointer to the password.  If this file is edited to support
** some other more robust encryption algorith, such as AES, the pCrypto
** field might store key schedules or other precomputed content needed by
** that algorithm.
**
** The pEncode and pDecode fields hold information needed by the compressor
** and decompressor functions.
**
** Callback routines for doing encryption/decryption and compression and
** uncompression are always passed a pointer to the complete ZipvfsInst
** structure.  Those routines should then access the particular fields of
** ZipvfsInst structure that are relevant to them.
**
** The setup and breakdown routines (xComprSetup(), xComprCleanup(),
** xDecmprSetup() and xDecmprCleanup()) are also passed a pointer to the
** complete ZipvfsInst object.  Once again, these routines should setup
** or breakdown only those fields of the ZipvfsInst object that are relevant.
**
** The pCtx pointer is a copy of the original context pointer that was 
** passed in as the 3rd parameter to zipvfs_create_vfs_v3().  For the
** NDSeV DevKit, this is currently always a NULL pointer.
*/
struct ZipvfsInst {
  void *pCtx;                     /* Context ptr to zipvfs_create_vfs_v3() */
  void *pCrypto;                  /* Info used by encryption/decryption */
  struct EncoderInst *pEncode;    /* Info used by compression */
  struct DecoderInst *pDecode;    /* Info used by decompression */
  const ZipvfsAlgorithm *pAlg;    /* Corresponding algorithm object */
  int iLevel;                     /* Compression level */
};

/*
** An instance of the following structure describes a single ZIPVFS
** compression and encryption algorithm.  There is an array of instances
** of this object further down in this file that contains descriptions of
** all supported algorithms.
**
** zName                This is the name of the algorithm.  When creating
**                      a new database, use the "zv=NAME" query parameter
**                      to select an algorithm, where NAME matches this field.
**                      The name will written into the database header and
**                      so when reopening the database, the same algorithm
**                      will be used.
**
** xBound(X,N)          This function returns the maximum size of the output
**                      buffer for xCompr() assuming that the input is
**                      N bytes in size.  X is a pointer to the ZipvfsInst
**                      object for the open connection.
**
** xComprSetup(X,F)     This function is called once when the database is
**                      opened.  Its job is to setup the X->pEncode field
**                      appropriately.  F is the name of the database file
**                      and is suitable for passing to sqlite3_uri_parameter().
**                      Return SQLITE_OK on success or another
**                      error code (ex: SQLITE_NOMEM) upon error.
**
** xCompr(X,O,N,I,M)    This function does compression on input buffer I
**                      of size M bytes, and writes the output into buffer O
**                      and the output buffer size into N.
**
** xComprCleanup(X)     This function is called once when the database is
**                      closing.  Its job is to cleanup the X->pEncode field.
**                      This routine undoes the work of xComprSetup().
**
** xDecmprSetup(X,F)    This function is called once when the database is
**                      opened.  It should setup the X->pDecode field as
**                      is appropriate.  F is the name of the database file,
**                      suitable for passing to sqlite3_uri_parameter().
**
** xDecmpr(X,O,N,I,M)   This routine does decompression.  The input in buffer
**                      I which is M bytes in size is decompressed into an
**                      output buffer O.  The number of bytes of decompressed
**                      content should be written into N.
**
** xDecmprCleanup(X)    This function is called once when the database is
**                      closed in order to cleanup the X->pDecode field.
**                      This undoes the work of xDecmprSetup().
**
** xCryptoSetup(X,F)    This function is invoked once when the database is
**                      first opened, for the purpose of setting up the
**                      X->pCrypto field.  The F argument is the name of the
**                      file that is being opened.  This routine can use
**                      sqlite3_uri_parameter(F, "password") to determine
**                      the value of the password query parameter, if desired.
**                     
** xEncrypt(X,O,I,M)    This routine does content encryption.  The input
**                      buffer I of size M bytes is encrypted and the results
**                      written into output buffer O.  Note that this routine
**                      must be invoked by the xCompr() method.  See the
**                      example implementations below for details.
**
** xDecrypt(X,0,I,M)    This routine does content decryption.  The input
**                      buffer I of size M bytes is decrypted into the output
**                      buffer O.  Note that this routine must be invoked
**                      by the xDecmpr() method.  See the examples below for
**                      a demonstration.
**                      
** xCryptoCleanup(X)    This routine is called when the database is closing
**                      in order to cleanup the X->pCrypto field.
*/
struct ZipvfsAlgorithm {
  const char zName[12];
  int (*xBound)(void*,int);
  int (*xComprSetup)(ZipvfsInst*,const char*);
  int (*xCompr)(void*,char*,int*,const char*,int);
  int (*xComprCleanup)(ZipvfsInst*);
  int (*xDecmprSetup)(ZipvfsInst*,const char*);
  int (*xDecmpr)(void*,char*,int*,const char*,int);
  int (*xDecmprCleanup)(ZipvfsInst*);
  int (*xCryptoSetup)(ZipvfsInst*, const char *zFile);
  int (*xEncrypt)(ZipvfsInst*,char*,const char*,int);
  int (*xDecrypt)(ZipvfsInst*,char*,const char*,int);
  int (*xCryptoCleanup)(ZipvfsInst*);
};

#ifdef NDS_ENABLE_AES
/*****************************************************************************
** Encryption routines.
**
** If the ZIPVFS module is started using a URI filename that contains
** a "password=" query parameter, then the password is used as 16-bytes key
** for Rijndael AES encryption algorithm.
**
** The aesEncryptionSetup() and aesEncryptionCleanup() routines are setup and
** breakdown routines for the encryption logic. The actual encryption
** and decryption of content is performed by aesEncrypt() and aesDecrypt()
** routines.
*/

#define AES_ENCRYPTION_KEY_BITS    128
#define AER_ENCRYPTION_NUM_BLOCKS  4
#define AER_ENCRYPTION_BLOCK_SIZE  (KEYLENGTH(AES_ENCRYPTION_KEY_BITS))

/* This structure is filled in aesEncryptionSetup() and passed to aesEncrypt()
** and aesDecrypt() routines and holds necessary input data for AES
** Rijndael encryption algorithm.
*/
struct aes_encryption_data
{
    unsigned long EncryptBuffer[RKLENGTH(AES_ENCRYPTION_KEY_BITS)];
    int           EncryptRounds;
    unsigned long DecryptBuffer[RKLENGTH(AES_ENCRYPTION_KEY_BITS)];
    int           DecryptRounds;
    char*         pTempBuffer;
    int           TempBufferSize;
};
#endif

/*
** This routine determines whether or not encryption
** should be used for the database file. This routine
** is called directly from ZIPVFS if ZIPVFS was registered using
** zipvfs_create_vfs_v2().  If ZIPVFS was registered using the newer
** zipvfs_create_vfs_v3() interface then ZIPVFS will never invoke this
** routine itself; the xAutoDetect callback supplied by the application
** should invoke this routine instead.  
*/
static int aesEncryptionSetup(
  ZipvfsInst *p,           /* The ZipvfsInst object for the chosen algorithm */
  const char *zFilename    /* Name of the file being opened */
){
#ifdef NDS_ENABLE_AES
  const char *zPasswd;
  struct aes_encryption_data* pEncryptData;

  zPasswd = sqlite3_uri_parameter(zFilename, "password");
  if( zPasswd!=0 && p->pAlg->xEncrypt!=0 && p->pAlg->xDecrypt!=0 ){
    /* If the database connection is being opened with a URI filename
    ** and there is a password= query parameter on the URI, then set
    ** the local context pointer to a suitable encryption data object.
    */
    unsigned char AesKey[KEYLENGTH(AES_ENCRYPTION_KEY_BITS)];
    int i;
    for (i = 0; i < sizeof(AesKey) / sizeof(AesKey[0]); i++)
      AesKey[i] = *zPasswd != 0 ? *zPasswd++ : 0;

    pEncryptData = (struct aes_encryption_data*)
                          sqlite3_malloc(sizeof(struct aes_encryption_data));

    pEncryptData->EncryptRounds = rijndaelSetupEncrypt(
                pEncryptData->EncryptBuffer, AesKey, AES_ENCRYPTION_KEY_BITS);
    pEncryptData->DecryptRounds = rijndaelSetupDecrypt(
                pEncryptData->DecryptBuffer, AesKey, AES_ENCRYPTION_KEY_BITS);
    pEncryptData->pTempBuffer = NULL;
    pEncryptData->TempBufferSize = 0;
    p->pCrypto = pEncryptData;
  }else{
    /* If not using URIs or if there is no password, then the local
    ** context pointer is NULL.
    */
    p->pCrypto = 0;
  }
#else
  p->pCrypto = 0;
#endif
  return SQLITE_OK;
}

/* This routine is called by ZIPVFS when a database connection is shutting
** down in order to deallocate resources previously allocated by
** aesEncryptionSetup().
*/
static int aesEncryptionCleanup(ZipvfsInst *p){
#ifdef NDS_ENABLE_AES
  if( p->pCrypto ){
    struct aes_encryption_data* pEncryptData =
                                (struct aes_encryption_data*) p->pCrypto;
    sqlite3_free(pEncryptData->pTempBuffer);
    sqlite3_free(pEncryptData);
    p->pCrypto = 0;
  }
#endif
  return SQLITE_OK;
}

#ifdef NDS_ENABLE_AES
/* This enum is used to distinguish AES descryption or encryption in helper
** routine aesEncryptDecrypt().
*/
enum aes_method
{
    AES_ENCRYPTION,
    AES_DECRYPTION
};

/* This is the helper routine for AES encryption and decryption.
*/
static void aesEncryptDecrypt(void* pCrypto, char* zOut, const char* zIn,
                              int nIn, enum aes_method eAesMethod){
  if ( pCrypto ){
    const unsigned char* zCurIn = (const unsigned char*) zIn;
    unsigned char* zCurOut = (unsigned char*) zOut;
    int nRestIn = nIn;
    struct aes_encryption_data* pEncryptData =
                                (struct aes_encryption_data*) pCrypto;
    const int nMaxEncryptionSize = AER_ENCRYPTION_NUM_BLOCKS *
                                   AER_ENCRYPTION_BLOCK_SIZE;
    const int nNumBlocks = (nIn >= nMaxEncryptionSize) ?
                                   AER_ENCRYPTION_NUM_BLOCKS :
                                   nIn / AER_ENCRYPTION_BLOCK_SIZE;
    int i = 0;
    for (; i < nNumBlocks; ++i){
      if (eAesMethod == AES_ENCRYPTION)
        rijndaelEncrypt(pEncryptData->EncryptBuffer,
                        pEncryptData->EncryptRounds, zCurIn, zCurOut);
      else
        rijndaelDecrypt(pEncryptData->DecryptBuffer,
                        pEncryptData->DecryptRounds, zCurIn, zCurOut);
      zCurIn += AER_ENCRYPTION_BLOCK_SIZE;
      zCurOut += AER_ENCRYPTION_BLOCK_SIZE;
      nRestIn -= AER_ENCRYPTION_BLOCK_SIZE;
    }
    if (zIn != zOut)
      memcpy(zCurOut, zCurIn, nRestIn);
  }
}
#endif

/* This is the routine that does the AES encryption.
**
** ZIPVFS does *not* call this routine directly.  Instead, the
** various compression and decompression routines must call this
** routine themselves.
**
** The first paramater p->pCrypto contain pointer to aes_encryption_data
** structure allocated by aesEncryptionSetup() routine or NULL if encryption
** is not required.
*/
static int aesEncryption(
  ZipvfsInst *p,       /* ZIPVFS algorithm context */
  char *zOut,          /* Write the results here */
  const char *zIn,     /* Input to be encrypted or decrypted */
  int nIn              /* Number of bytes to encrypto or decrypt */
){
#ifdef NDS_ENABLE_AES
  aesEncryptDecrypt(p->pCrypto, zOut, zIn, nIn, AES_ENCRYPTION);
#endif

  return SQLITE_OK;
}

/* This is the routine that does the AES decryption.
**
** ZIPVFS does *not* call this routine directly.  Instead, the
** various compression and decompression routines must call this
** routine themselves.
**
** The first paramater p->pCrypto contain pointer to aes_encryption_data
** structure allocated by aesEncryptionSetup() routine or NULL if encryption
** is not required.
*/
static int aesDecryption(
  ZipvfsInst *p,       /* ZIPVFS algorithm context */
  char *zOut,          /* Write the results here */
  const char *zIn,     /* Input to be encrypted or decrypted */
  int nIn              /* Number of bytes to encrypto or decrypt */
){
#ifdef NDS_ENABLE_AES
  aesEncryptDecrypt(p->pCrypto, zOut, zIn, nIn, AES_DECRYPTION);
#endif

  return SQLITE_OK;
}

/*
** This is a wrapper routine around xDecrypt().  It allocates space to do 
** the decryption so that the input, aIn[], is not altered.   A pointer to
** the decrypted text is returned.  NULL is returned if there is a memory
** allocation error.
*/
const char *aesDecryptWrapper(
  ZipvfsInst *p,          /* The open ZIPVFS connection */
  const char *aIn,        /* Buffer to be decrypted */
  const int nIn           /* Size of the input buffer */
){
#ifdef NDS_ENABLE_AES
  if( p->pCrypto ){
    struct aes_encryption_data* pEncryptData =
                                (struct aes_encryption_data*) p->pCrypto;
    if( pEncryptData->TempBufferSize<nIn ){
      int nNew = nIn;
      char *aNew = sqlite3_realloc(pEncryptData->pTempBuffer, nNew);
      if( aNew==0 ) return 0;
      pEncryptData->pTempBuffer = aNew;
      pEncryptData->TempBufferSize = nNew;
    }
    p->pAlg->xDecrypt(p, pEncryptData->pTempBuffer, aIn, nIn);
    aIn = pEncryptData->pTempBuffer;
  }
#endif

  return aIn;
}

/* End encryption logic
******************************************************************************/

#ifdef NDS_ENABLE_ZLIB

/*****************************************************************************
** ZLIB compression for ZipVFS.
**
** These routines implement compression using the external ZLIB library.
*/
static int zlibBound(void *pCtx, int nByte){
  return nByte + (nByte >> 12) + (nByte >> 14) + 11;
}

static int zlibCompress(
  void *pLocalCtx, 
  char *aDest, int *pnDest, 
  const char *aSrc, int nSrc
){
  uLongf n = *pnDest;             /* In/out buffer size for compress() */
  int rc;                         /* compress() return code */
  ZipvfsInst *p = (ZipvfsInst*)pLocalCtx;
  int iLevel = p->iLevel;
 
  if( iLevel<0 || iLevel>9 ) iLevel = Z_DEFAULT_COMPRESSION;
  rc = compress2((unsigned char*)aDest, &n, (unsigned char*)aSrc, nSrc, iLevel);
  if( p->pCrypto ){
    p->pAlg->xEncrypt(p, aDest, aDest, n);
  }
  *pnDest = n;
  return (rc==Z_OK ? SQLITE_OK : SQLITE_ERROR);
}

static int zlibUncompress(
  void *pLocalCtx, 
  char *aDest, int *pnDest, 
  const char *aSrc, int nSrc
){
  uLongf n = *pnDest;             /* In/out buffer size for uncompress() */
  int rc;                         /* uncompress() return code */
  ZipvfsInst *p = (ZipvfsInst*)pLocalCtx;
 
  aSrc = aesDecryptWrapper(p, aSrc, nSrc);
  if( aSrc==0 ) return SQLITE_NOMEM;
  rc = uncompress((unsigned char*)aDest, &n, (unsigned char*)aSrc, nSrc);
  *pnDest = n;
  return (rc==Z_OK ? SQLITE_OK : SQLITE_ERROR);
}
/* End ZLIB compression
******************************************************************************/

#endif /* NDS_ENABLE_ZLIB */

#ifdef NDS_ENABLE_LZ4
/*****************************************************************************
** LZ4 compression for ZipVFS.
**
** These routines implement compression using the external LZ4 library.
*/

static int lz4Bound(void *pCtx, int nByte){
  return LZ4_compressBound(nByte);
}

static void lz4Encrypt(
void *pLocalCtx,
char *aDest, int nDest
){
  ZipvfsInst *p = (ZipvfsInst*)pLocalCtx;
  if (p->pCrypto)
    p->pAlg->xEncrypt(p, aDest, aDest, nDest);
}

static int lz4Compress(
  void *pLocalCtx,
  char *aDest, int *pnDest,
  const char *aSrc, int nSrc
){
  int nDest = LZ4_compress_limitedOutput(aSrc, aDest, nSrc, *pnDest);
  if (nDest == 0)
    return SQLITE_ERROR;

  lz4Encrypt(pLocalCtx, aDest, nDest);
  *pnDest = nDest;

  return SQLITE_OK;
}

static int lz4CompressHc(
  void *pLocalCtx,
  char *aDest, int *pnDest,
  const char *aSrc, int nSrc
){
  int nDest = LZ4_compressHC_limitedOutput(aSrc, aDest, nSrc, *pnDest);
  if (nDest == 0)
    return SQLITE_ERROR;

  lz4Encrypt(pLocalCtx, aDest, nDest);
  *pnDest = nDest;

  return SQLITE_OK;
}

static int lz4Uncompress(
  void *pLocalCtx,
  char *aDest, int *pnDest,
  const char *aSrc, int nSrc
){
  int nDest;
  ZipvfsInst *p = (ZipvfsInst*)pLocalCtx;
  aSrc = aesDecryptWrapper(p, aSrc, nSrc);
  if (aSrc == 0)
    return SQLITE_NOMEM;

  nDest = LZ4_decompress_safe(aSrc, aDest, nSrc, *pnDest);
  if (nDest < 0)
    return SQLITE_ERROR;

  *pnDest = nDest;

  return SQLITE_OK;
}
/* End LZ4 compression
******************************************************************************/

#endif /* NDS_ENABLE_LZ4 */

#ifdef NDS_ENABLE_NDSC

/*****************************************************************************
** NDSC compression for ZipVFS.
**
** These routines implement compression using the external NDSC library.
*/

#define NDSC_MIN_COMPRESSION_LEVEL      0
#define NDSC_DEFAULT_COMPRESSION_LEVEL  4
#define NDSC_MAX_COMPRESSION_LEVEL      8

int ndscBound(void* arg, int n)
{
   return CalcNDSC(arg, n, 0);
}

int ndscCompress(
  void* arg,
  char* outBuff, int* outBuffSize,
  const char* inBuff,  int inBuffSize
  )
{
   ZipvfsInst *pInst = (ZipvfsInst*)arg;
   unsigned int outLen = *outBuffSize;
   int iLevel = pInst->iLevel;
   int result;
   if (iLevel < NDSC_MIN_COMPRESSION_LEVEL || iLevel > NDSC_MAX_COMPRESSION_LEVEL)
     iLevel = NDSC_DEFAULT_COMPRESSION_LEVEL;
   result = PackNDSC((unsigned char*)inBuff, inBuffSize,
                     (unsigned char*)outBuff, outLen,
                     &outLen, iLevel);
   *outBuffSize = outLen;
   if( pInst->pCrypto ){
     pInst->pAlg->xEncrypt(pInst, outBuff, outBuff, *outBuffSize);
   }
   return result == 0 ? SQLITE_OK : SQLITE_ERROR ;
}

int ndscUncompress(
  void* arg,
  char* outBuff, int* outBuffSize,
  const char* inBuff,  int inBuffSize
  )
{
   ZipvfsInst *p = (ZipvfsInst*)arg;
   int result;

   inBuff = aesDecryptWrapper(p, inBuff, inBuffSize);
   if( inBuff==0 ) return SQLITE_NOMEM;
   result = UnpackNDSC((unsigned char*)inBuff, inBuffSize,
                       (unsigned char*)outBuff, *outBuffSize);
   return result == 0 ? SQLITE_OK : SQLITE_ERROR ;
}
/* End NDSC compression
******************************************************************************/

#endif /* NDS_ENABLE_NDSC */

/******************************************************************************
** Blank-space removal compression routines for use with ZIPVFS
**
** Many SQLite database pages contain a large span of zero bytes.  This
** compression algorithm attempts to reduce the page size by simply not
** storing the single largest span of zeros within each page.
**
** The compressed format consists of a single 2-byte big-endian number X
** followed by N bytes of content.  To decompress, copy the first X
** bytes of content, followed by Pagesize-N zeros, followed by the final
** N-X bytes of content.
*/
static int bsrBound(void *NotUsed, int n){
  return n+2;
}
static int bsrCompress(
  void *pLocalCtx,
  char *aOut, int *pnOut,
  const char *aIn,  int nIn
){
  const char *p = aIn;          /* Loop pointer */
  const char *pEnd = &aIn[nIn]; /* Ptr to end of data */
  const char *pBestStart = p;   /* Ptr to first byte of longest run of zeros */
  const char *pN = pEnd;  /* End of data to search (reduces for longer runs) */
  int X;                  /* Length of the current run of zeros */
  int bestLen = 0;        /* Length of the longest run of zeros */
  ZipvfsInst *pInst = (ZipvfsInst*)pLocalCtx;

  /* Find the longest run of zeros */
  while( p<pN ){
    if( *p==0 ){
      const char *pS = p;  /* save start */
      p++;
      while( p<pEnd && *p==0 ) p++;
      X = p - pS;
      if( X>bestLen ){
        bestLen = X;
        pBestStart = pS;
        pN = &aIn[nIn - bestLen]; /* reduce search space based on longest run */
      }
    }
    p++;
  }

  /* Do the compression */
  X = pBestStart-aIn;          /* length of first data block */
  aOut[0] = (X>>8)&0xff;
  aOut[1] = X & 0xff;
  memcpy(&aOut[2], aIn, X);    /* copy first data block */
  memcpy(&aOut[2+X], &pBestStart[bestLen], nIn-X-bestLen);
  *pnOut = 2+nIn-bestLen;
  if( pInst->pCrypto ){
    pInst->pAlg->xEncrypt(pInst, aOut, aOut,*pnOut);
  }
  return SQLITE_OK;
}
static int bsrUncompress(
  void *pLocalCtx,
  char *aOut, int *pnOut,
  const char *aIn,  int nIn
){
  int X;                     /* Initial number of bytes to copy */
  int szPage;                /* Size of a page */
  int nTail;                 /* Byte of content to write to tail of page */
  const unsigned char *a;    /* Input unsigned */
  ZipvfsInst *p = (ZipvfsInst*)pLocalCtx;

  aIn = aesDecryptWrapper(p, aIn, nIn);
  if( aIn==0 ) return SQLITE_NOMEM;
  a = (unsigned char*)aIn;
  X = (a[0]<<8) + a[1];
  nIn -= 2;
  nTail = nIn - X;
  szPage = *pnOut;
  if( X>0 ){
    memcpy(aOut, &aIn[2], X);
  }
  memset(&aOut[X], 0, szPage-nIn);
  memcpy(&aOut[szPage-nTail], &aIn[2+X], nTail);
  return SQLITE_OK;
}
/* End BSR compression routines
******************************************************************************/

/*
** The following is the array of available compression and encryption 
** algorithms.  To add new compression or encryption algorithms, make
** new entries to this array.
*/
static const ZipvfsAlgorithm aZipvfs[] = {

#ifdef NDS_ENABLE_ZLIB
  /* ZLib */ {
  /* zName          */  "zlib",
  /* xBound         */  zlibBound,
  /* xComprSetup    */  0,
  /* xCompr         */  zlibCompress,
  /* xComprCleanup  */  0,
  /* xDecmprSetup   */  0,
  /* xDecmpr        */  zlibUncompress,
  /* xDecmprCleanup */  0,
  /* xCryptoSetup   */  aesEncryptionSetup,
  /* xEncrypt       */  aesEncryption,
  /* xDecrypt       */  aesDecryption,
  /* xCryptoCleanup */  aesEncryptionCleanup
  },
#endif /* NDS_ENABLE_ZLIB */

#ifdef NDS_ENABLE_LZ4
  /* LZ4 */ {
  /* zName          */  "lz4",
  /* xBound         */  lz4Bound,
  /* xComprSetup    */  0,
  /* xCompr         */  lz4Compress,
  /* xComprCleanup  */  0,
  /* xDecmprSetup   */  0,
  /* xDecmpr        */  lz4Uncompress,
  /* xDecmprCleanup */  0,
  /* xCryptoSetup   */  aesEncryptionSetup,
  /* xEncrypt       */  aesEncryption,
  /* xDecrypt       */  aesDecryption,
  /* xCryptoCleanup */  aesEncryptionCleanup
  },

  /* LZ4HC */ {
  /* zName          */  "lz4hc",
  /* xBound         */  lz4Bound,
  /* xComprSetup    */  0,
  /* xCompr         */  lz4CompressHc,
  /* xComprCleanup  */  0,
  /* xDecmprSetup   */  0,
  /* xDecmpr        */  lz4Uncompress,
  /* xDecmprCleanup */  0,
  /* xCryptoSetup   */  aesEncryptionSetup,
  /* xEncrypt       */  aesEncryption,
  /* xDecrypt       */  aesDecryption,
  /* xCryptoCleanup */  aesEncryptionCleanup
  },
#endif /* NDS_ENABLE_LZ4 */

#ifdef NDS_ENABLE_NDSC
  /* NDSC */ {
  /* zName          */  "ndsc",
  /* xBound         */  ndscBound,
  /* xComprSetup    */  0,
  /* xCompr         */  ndscCompress,
  /* xComprCleanup  */  0,
  /* xDecmprSetup   */  0,
  /* xDecmpr        */  ndscUncompress,
  /* xDecmprCleanup */  0,
  /* xCryptoSetup   */  aesEncryptionSetup,
  /* xEncrypt       */  aesEncryption,
  /* xDecrypt       */  aesDecryption,
  /* xCryptoCleanup */  aesEncryptionCleanup
  },
  
  /* NDSC with an alternative name */ {
  /* zName          */  "ndsc-mux",
  /* xBound         */  ndscBound,
  /* xComprSetup    */  0,
  /* xCompr         */  ndscCompress,
  /* xComprCleanup  */  0,
  /* xDecmprSetup   */  0,
  /* xDecmpr        */  ndscUncompress,
  /* xDecmprCleanup */  0,
  /* xCryptoSetup   */  aesEncryptionSetup,
  /* xEncrypt       */  aesEncryption,
  /* xDecrypt       */  aesDecryption,
  /* xCryptoCleanup */  aesEncryptionCleanup
  },
#endif /* NDS_ENABLE_NDSC */

  /* Blank-space removal */ {
  /* zName          */  "bsr",
  /* xBound         */  bsrBound,
  /* xComprSetup    */  0,
  /* xCompr         */  bsrCompress,
  /* xComprCleanup  */  0,
  /* xDecmprSetup   */  0,
  /* xDecmpr        */  bsrUncompress,
  /* xDecmprCleanup */  0,
  /* xCryptoSetup   */  aesEncryptionSetup,
  /* xEncrypt       */  aesEncryption,
  /* xDecrypt       */  aesDecryption,
  /* xCryptoCleanup */  aesEncryptionCleanup
  },

};

/*
** This routine is called when a ZIPVFS database connection is shutting
** down.  Invoke all of the cleanup procedures in the ZipvfsAlgorithm
** object.
*/
static int nds_compression_algorithm_close(void *pLocalCtx){
  ZipvfsInst *p = (ZipvfsInst*)pLocalCtx;
  const ZipvfsAlgorithm *pAlg = p->pAlg;
  if( pAlg->xComprCleanup ){
    (void)pAlg->xComprCleanup(p);
  }
  if( pAlg->xDecmprCleanup ){
    (void)pAlg->xDecmprCleanup(p);
  }
  if( pAlg->xCryptoCleanup ){
    (void)pAlg->xCryptoCleanup(p);
  }
  sqlite3_free(p);
  return SQLITE_OK;
}

/*
** The following routine is the crux of this whole module.
**
** This is the xAutoDetect callback for ZipVFS.  The job of this function
** is to figure out which compression algorithm to use for a database
** and fill out the pMethods object with pointers to the appropriate
** compression and decompression routines.
**
** ZIPVFS calls this routine exactly once for each database file that is
** opened.  The zHeader parameter is a copy of the text found in bytes
** 3 through 16 of the database header.  Or, if a new database is being
** created, zHeader is a NULL pointer.  zFile is the name of the database
** file.  If the database was opened as URI, then sqlite3_uri_parameter()
** can be used with the zFile parameter to extract query parameters from
** the URI.
*/
int nds_compression_algorithm_detector(
  void *pCtx,              /* Copy of pCtx from zipvfs_create_vfs_v3() */
  const char *zFile,       /* Name of file being opened */
  const char *zHeader,     /* Algorithm name in the database header */
  ZipvfsMethods *pMethods  /* OUT: Write new pCtx and function pointers here */
){
  /* If zHeader==0 that means we have a new database file.
  ** Look to the zv query parameter (if there is one) as a
  ** substitute for the database header.
  */
  if( zHeader==0 ){
    const char *zZv = sqlite3_uri_parameter(zFile, "zv");
    if( zZv ) zHeader = zZv;
  }

  /* Look for a compression algorithm that matches zHeader.
  */
  if( zHeader ){
    int i;
    for(i=0; i<(int)(sizeof(aZipvfs)/sizeof(aZipvfs[0])); i++){
      if( strcmp(aZipvfs[i].zName, zHeader)==0 ){
        ZipvfsInst *pInst = sqlite3_malloc( sizeof(*pInst) );
        int rc = SQLITE_OK;
        if( pInst==0 ) return SQLITE_NOMEM;
        memset(pInst, 0, sizeof(*pInst));
        pInst->pCtx = pCtx;
        pInst->pAlg = &aZipvfs[i];
        pInst->iLevel = (int)sqlite3_uri_int64(zFile, "level", -1);
        pMethods->zHdr = aZipvfs[i].zName;
        pMethods->xCompressBound = aZipvfs[i].xBound;
        pMethods->xCompress = aZipvfs[i].xCompr;
        pMethods->xUncompress = aZipvfs[i].xDecmpr;
        pMethods->xCompressClose = nds_compression_algorithm_close;
        pMethods->pCtx = pInst;
        if( aZipvfs[i].xCryptoSetup ){
          rc = aZipvfs[i].xCryptoSetup(pInst, zFile);
        }
        if( rc==SQLITE_OK && aZipvfs[i].xComprSetup ){
          rc = aZipvfs[i].xComprSetup(pInst, zFile);
        }
        if( rc==SQLITE_OK && aZipvfs[i].xDecmprSetup ){
          rc = aZipvfs[i].xDecmprSetup(pInst, zFile);
        }
        if( rc!=SQLITE_OK ){
          nds_compression_algorithm_close(pInst);
          memset(pMethods, 0, sizeof(*pMethods));
        }
        return rc;
      }
    }
  }

  /* If no compression algorithm is found whose name matches zHeader,
  ** then assume no compression is desired.  Clearing the pMethod object
  ** causes ZIPVFS to be a no-op so that it reads and writes ordinary
  ** uncompressed SQLite database files.
  */
  memset(pMethods, 0, sizeof(*pMethods));
  return SQLITE_OK;
}
