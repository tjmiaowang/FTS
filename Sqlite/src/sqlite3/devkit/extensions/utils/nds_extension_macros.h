#ifndef NDS_EXTENSION_MACROS_H
#define NDS_EXTENSION_MACROS_H

#ifdef __cplusplus
    #define NDS_EXTENSION_BEGIN_DECLS extern "C" {
    #define NDS_EXTENSION_END_DECLS }
#else
    #define NDS_EXTENSION_BEGIN_DECLS
    #define NDS_EXTENSION_END_DECLS
#endif

#define ARRAY_SIZE(a) ( sizeof(a)/sizeof((a)[0]) )

#endif /* NDS_EXTENSION_MACROS_H */
