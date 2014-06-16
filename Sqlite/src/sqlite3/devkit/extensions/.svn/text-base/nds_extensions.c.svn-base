#include <stddef.h>

#include "nds_extensions.h"

#ifdef HAVE_NDS_COLLATIONS
    #include "collations/nds_collations_impl.h"
#endif

int nds_extensions_init(sqlite3* p_db)
{
    int r = SQLITE_OK;

    if (p_db == NULL)
        return SQLITE_ERROR;

#ifdef HAVE_NDS_COLLATIONS
    if (r == SQLITE_OK)
        r = RegisterNDSCollations(p_db);
#endif

    return r;
}
