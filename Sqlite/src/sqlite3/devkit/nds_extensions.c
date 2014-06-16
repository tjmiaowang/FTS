/*
** This file implements a stub for the nds_extensions_init() function.
** The stub is a no-op.  But users of the NDS DevKit can replace the stub
** with a different function that does whatever application-specific
** initialization is required.
*/
#include "nds_sqlite3.h"

/*
** This routine is called as each new database connection is opened.
*/
int nds_extensions_init(sqlite3 *db){
  (void)db;  /* No-op */
  return SQLITE_OK;
}
