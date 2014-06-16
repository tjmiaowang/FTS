#ifndef NDS_UNICODE61_H
#define NDS_UNICODE61_H

#include "nds_sqlite3.h"
#include "../utils/nds_extension_macros.h"

NDS_EXTENSION_BEGIN_DECLS

void sqlite3Fts3NdsUnicodeTokenizer(sqlite3_tokenizer_module const** ndsunicode61);

NDS_EXTENSION_END_DECLS

#endif /* NDS_UNICODE61_H */
