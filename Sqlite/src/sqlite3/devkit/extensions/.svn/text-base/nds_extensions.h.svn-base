#ifndef NDS_EXTENSIONS_H
#define NDS_EXTENSIONS_H

#include "nds_sqlite3.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * The function registers all NDS extensions.
 *
 * \param p_db The pointer to database for which to register the extensions.
 *
 * \return Returns SQLITE_OK in case of success, otherwise returns SQLite error code.
 */
int nds_extensions_init(sqlite3* p_db);

#ifdef HAVE_NDS_COLLATIONS
    /* Collations API */

    #ifdef _WIN32
        #if defined(NDS_COLLATIONS_EXPORT)
            #define NDS_COLLATIONS_API __declspec(dllexport)
        #elif defined(NDS_COLLATIONS_IMPORT)
            #define NDS_COLLATIONS_API __declspec(dllimport)
        #else
            #define NDS_COLLATIONS_API
        #endif
    #else
        #ifdef __GNUC__
            #define NDS_COLLATIONS_API __attribute__((visibility("default")))
        #else
            #define NDS_COLLATIONS_API
        #endif
    #endif

    /**
     * An opaque structure containing the actual description of a collation.
     *
     * This is returned by nds_lookup_collation_data() and used in nds_localized_compare().
     */
    struct tagNdsCollationData;
    typedef struct tagNdsCollationData nds_collation;

    /**
     * Look up collation data by name.
     *
     * \note The returned pointer must not be freed. It's valid throughout the lifecycle of the application.
     *
     * \return A pointer to collation data (to be used with nds_localized_compare()) or NULL if not found.
     */
    NDS_COLLATIONS_API nds_collation const* nds_lookup_collation_data(char const* collation_name);

    /**
     * Compare two utf8-encoded strings using given collation data.
     *
     * \param[in] collation_data Collation data to use for the comparison returned by nds_lookup_collation_data().
     * \param[in] string1        Pointer to the first string to compare.
     * \param[in] string1_len    Length (in bytes) of the first string.
     * \param[in] string2        Pointer to the second string to compare.
     * \param[in] string2_len    Length (in bytes) of the second string.
     *
     * \return 0 if the strings are equal, a number lower than 0 if the first string is lower or
     * a number greater than 0 if the second string is lower.
     */
    NDS_COLLATIONS_API int nds_localized_compare(nds_collation const* collation_data, char const* string1,
                                                 int string1_len, char const* string2, int string2_len);

#endif /* HAVE_NDS_COLLATIONS */

#ifdef HAVE_NDS_TOKENIZER
    /* ndsunicode61 tokenizer */

    /**
     * Get a pointer to the ndsunicode61 tokenizer module.
     *
     * \note This function is not exported, it's expected to be only called from within nds sqlite.
     */
    void sqlite3Fts3NdsUnicodeTokenizer(sqlite3_tokenizer_module const**ppModule);
#endif /* HAVE_NDS_TOKENIZER */

#ifdef __cplusplus
}
#endif

#endif /* NDS_EXTENSIONS_H */
