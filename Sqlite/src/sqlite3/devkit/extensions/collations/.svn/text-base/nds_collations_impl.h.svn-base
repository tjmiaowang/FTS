#ifndef NDS_COLLATIONS_IMPL_H
#define NDS_COLLATIONS_IMPL_H

#include "nds_sqlite3.h"
#include "../utils/nds_extension_macros.h"
#include "collation_data.h"

NDS_EXTENSION_BEGIN_DECLS

/**
 * Look up collation data by name.
 */
CollationData const* LookupCollationData(char const* collationName);

/**
 * Register all NDS collations.
 */
int RegisterNDSCollations(sqlite3* p_db);

NDS_EXTENSION_END_DECLS

#endif /* NDS_COLLATIONS_IMPL_H */
