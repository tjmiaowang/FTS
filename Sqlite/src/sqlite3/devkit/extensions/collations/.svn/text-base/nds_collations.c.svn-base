#include "../nds_extensions.h"
#include "nds_collations_impl.h"
#include "localized_compare.h"

nds_collation const* nds_lookup_collation_data(char const* collation_name)
{
    return LookupCollationData(collation_name);
}

int nds_localized_compare(nds_collation const* collation_data, char const* string1,
                          int string1_len, char const* string2, int string2_len)
{
    return LocalizedCompare(collation_data, string1, string1_len, string2, string2_len);
}
