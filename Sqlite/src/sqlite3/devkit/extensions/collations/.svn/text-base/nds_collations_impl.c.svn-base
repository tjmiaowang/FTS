#include <stddef.h>
#include <string.h>

#include "nds_collations_impl.h"

#include "collation_data.h"
#include "collation_data_de_de.h"
#include "collation_data_zh_cn.h"
#include "localized_compare.h"
#include "../utils/nds_extension_macros.h"
#include "../nds_extensions.h"

typedef struct
{
    char const*          name;
    CollationData const* p_data;
} CollationEntry;

static const CollationEntry COLLATIONS[] =
{
    { "de_DE_standard",  &CollationData_de_DE_standard },
    { "de_DE_phonebook", &CollationData_de_DE_phonebook },
    { "zh_CN_gb2312",    &CollationData_zh_CN_gb2312 },
};
static const unsigned COLLATIONS_COUNT = ARRAY_SIZE(COLLATIONS);

CollationData const* LookupCollationData(char const* collationName)
{
    unsigned i;
    for (i = 0; i < COLLATIONS_COUNT; i++)
    {
        if (strcmp(collationName, COLLATIONS[i].name) == 0)
        {
            return COLLATIONS[i].p_data;
        }
    }

    return NULL;
}

static int LocalizedCompareFunction(void* p_arg, int string1_len, void const* p_string1, int string2_len,
                                    void const* p_string2)
{
    CollationData const* p_data = (CollationData const*) p_arg;
    char const* string1 = (char const*) p_string1;
    char const* string2 = (char const*) p_string2;

    return LocalizedCompare(p_data, string1, string1_len, string2, string2_len);
}

int RegisterNDSCollations(sqlite3* p_db)
{
    unsigned i;

    if (p_db == NULL)
        return SQLITE_ERROR;

    for (i = 0; i < COLLATIONS_COUNT; i++)
    {
        int result = sqlite3_create_collation(p_db, COLLATIONS[i].name, SQLITE_UTF8,
                                              (void*) COLLATIONS[i].p_data, LocalizedCompareFunction);
        if (result != SQLITE_OK)
            return result;
    }

    return SQLITE_OK;
}
