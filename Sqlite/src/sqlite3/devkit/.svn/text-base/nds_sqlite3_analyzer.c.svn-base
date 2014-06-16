/*
** This version of SQLite is specially prepared for the
** Navigation Data Standard e.V.  Use by license only.
*/
#define TCLSH 2
#include "nds_sqlite3.c"

/*
** 2010 July 12
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
******************************************************************************
**
** This file contains an implementation of the "dbstat" virtual table.
**
** The dbstat virtual table is used to extract low-level formatting
** information from an SQLite database in order to implement the
** "sqlite3_analyzer" utility.  See the ../tool/spaceanal.tcl script
** for an example implementation.
*/

#ifndef SQLITE_AMALGAMATION

#endif

#ifndef SQLITE_OMIT_VIRTUALTABLE

/*
** Page paths:
** 
**   The value of the 'path' column describes the path taken from the 
**   root-node of the b-tree structure to each page. The value of the 
**   root-node path is '/'.
**
**   The value of the path for the left-most child page of the root of
**   a b-tree is '/000/'. (Btrees store content ordered from left to right
**   so the pages to the left have smaller keys than the pages to the right.)
**   The next to left-most child of the root page is
**   '/001', and so on, each sibling page identified by a 3-digit hex 
**   value. The children of the 451st left-most sibling have paths such
**   as '/1c2/000/, '/1c2/001/' etc.
**
**   Overflow pages are specified by appending a '+' character and a 
**   six-digit hexadecimal value to the path to the cell they are linked
**   from. For example, the three overflow pages in a chain linked from 
**   the left-most cell of the 450th child of the root page are identified
**   by the paths:
**
**      '/1c2/000+000000'         // First page in overflow chain
**      '/1c2/000+000001'         // Second page in overflow chain
**      '/1c2/000+000002'         // Third page in overflow chain
**
**   If the paths are sorted using the BINARY collation sequence, then
**   the overflow pages associated with a cell will appear earlier in the
**   sort-order than its child page:
**
**      '/1c2/000/'               // Left-most child of 451st child of root
*/
#define VTAB_SCHEMA                                                         \
  "CREATE TABLE xx( "                                                       \
  "  name       STRING,           /* Name of table or index */"             \
  "  path       INTEGER,          /* Path to page from root */"             \
  "  pageno     INTEGER,          /* Page number */"                        \
  "  pagetype   STRING,           /* 'internal', 'leaf' or 'overflow' */"   \
  "  ncell      INTEGER,          /* Cells on page (0 for overflow) */"     \
  "  payload    INTEGER,          /* Bytes of payload on this page */"      \
  "  unused     INTEGER,          /* Bytes of unused space on this page */" \
  "  mx_payload INTEGER,          /* Largest payload size of all cells */"  \
  "  pgoffset   INTEGER,          /* Offset of page in file */"             \
  "  pgsize     INTEGER           /* Size of the page */"                   \
  ");"


typedef struct StatTable StatTable;
typedef struct StatCursor StatCursor;
typedef struct StatPage StatPage;
typedef struct StatCell StatCell;

struct StatCell {
  int nLocal;                     /* Bytes of local payload */
  u32 iChildPg;                   /* Child node (or 0 if this is a leaf) */
  int nOvfl;                      /* Entries in aOvfl[] */
  u32 *aOvfl;                     /* Array of overflow page numbers */
  int nLastOvfl;                  /* Bytes of payload on final overflow page */
  int iOvfl;                      /* Iterates through aOvfl[] */
};

struct StatPage {
  u32 iPgno;
  DbPage *pPg;
  int iCell;

  char *zPath;                    /* Path to this page */

  /* Variables populated by statDecodePage(): */
  u8 flags;                       /* Copy of flags byte */
  int nCell;                      /* Number of cells on page */
  int nUnused;                    /* Number of unused bytes on page */
  StatCell *aCell;                /* Array of parsed cells */
  u32 iRightChildPg;              /* Right-child page number (or 0) */
  int nMxPayload;                 /* Largest payload of any cell on this page */
};

struct StatCursor {
  sqlite3_vtab_cursor base;
  sqlite3_stmt *pStmt;            /* Iterates through set of root pages */
  int isEof;                      /* After pStmt has returned SQLITE_DONE */

  StatPage aPage[32];
  int iPage;                      /* Current entry in aPage[] */

  /* Values to return. */
  char *zName;                    /* Value of 'name' column */
  char *zPath;                    /* Value of 'path' column */
  u32 iPageno;                    /* Value of 'pageno' column */
  char *zPagetype;                /* Value of 'pagetype' column */
  int nCell;                      /* Value of 'ncell' column */
  int nPayload;                   /* Value of 'payload' column */
  int nUnused;                    /* Value of 'unused' column */
  int nMxPayload;                 /* Value of 'mx_payload' column */
  i64 iOffset;                    /* Value of 'pgOffset' column */
  int szPage;                     /* Value of 'pgSize' column */
};

struct StatTable {
  sqlite3_vtab base;
  sqlite3 *db;
};

#ifndef get2byte
# define get2byte(x)   ((x)[0]<<8 | (x)[1])
#endif

/*
** Connect to or create a statvfs virtual table.
*/
static int statConnect(
  sqlite3 *db,
  void *pAux,
  int argc, const char *const*argv,
  sqlite3_vtab **ppVtab,
  char **pzErr
){
  StatTable *pTab;

  pTab = (StatTable *)sqlite3_malloc(sizeof(StatTable));
  memset(pTab, 0, sizeof(StatTable));
  pTab->db = db;

  sqlite3_declare_vtab(db, VTAB_SCHEMA);
  *ppVtab = &pTab->base;
  return SQLITE_OK;
}

/*
** Disconnect from or destroy a statvfs virtual table.
*/
static int statDisconnect(sqlite3_vtab *pVtab){
  sqlite3_free(pVtab);
  return SQLITE_OK;
}

/*
** There is no "best-index". This virtual table always does a linear
** scan of the binary VFS log file.
*/
static int statBestIndex(sqlite3_vtab *tab, sqlite3_index_info *pIdxInfo){

  /* Records are always returned in ascending order of (name, path). 
  ** If this will satisfy the client, set the orderByConsumed flag so that 
  ** SQLite does not do an external sort.
  */
  if( ( pIdxInfo->nOrderBy==1
     && pIdxInfo->aOrderBy[0].iColumn==0
     && pIdxInfo->aOrderBy[0].desc==0
     ) ||
      ( pIdxInfo->nOrderBy==2
     && pIdxInfo->aOrderBy[0].iColumn==0
     && pIdxInfo->aOrderBy[0].desc==0
     && pIdxInfo->aOrderBy[1].iColumn==1
     && pIdxInfo->aOrderBy[1].desc==0
     )
  ){
    pIdxInfo->orderByConsumed = 1;
  }

  pIdxInfo->estimatedCost = 10.0;
  return SQLITE_OK;
}

/*
** Open a new statvfs cursor.
*/
static int statOpen(sqlite3_vtab *pVTab, sqlite3_vtab_cursor **ppCursor){
  StatTable *pTab = (StatTable *)pVTab;
  StatCursor *pCsr;
  int rc;

  pCsr = (StatCursor *)sqlite3_malloc(sizeof(StatCursor));
  memset(pCsr, 0, sizeof(StatCursor));
  pCsr->base.pVtab = pVTab;

  rc = sqlite3_prepare_v2(pTab->db, 
      "SELECT 'sqlite_master' AS name, 1 AS rootpage, 'table' AS type"
      "  UNION ALL  "
      "SELECT name, rootpage, type FROM sqlite_master WHERE rootpage!=0"
      "  ORDER BY name", -1,
      &pCsr->pStmt, 0
  );
  if( rc!=SQLITE_OK ){
    sqlite3_free(pCsr);
    return rc;
  }

  *ppCursor = (sqlite3_vtab_cursor *)pCsr;
  return SQLITE_OK;
}

static void statClearPage(StatPage *p){
  int i;
  for(i=0; i<p->nCell; i++){
    sqlite3_free(p->aCell[i].aOvfl);
  }
  sqlite3PagerUnref(p->pPg);
  sqlite3_free(p->aCell);
  sqlite3_free(p->zPath);
  memset(p, 0, sizeof(StatPage));
}

static void statResetCsr(StatCursor *pCsr){
  int i;
  sqlite3_reset(pCsr->pStmt);
  for(i=0; i<ArraySize(pCsr->aPage); i++){
    statClearPage(&pCsr->aPage[i]);
  }
  pCsr->iPage = 0;
  sqlite3_free(pCsr->zPath);
  pCsr->zPath = 0;
}

/*
** Close a statvfs cursor.
*/
static int statClose(sqlite3_vtab_cursor *pCursor){
  StatCursor *pCsr = (StatCursor *)pCursor;
  statResetCsr(pCsr);
  sqlite3_finalize(pCsr->pStmt);
  sqlite3_free(pCsr);
  return SQLITE_OK;
}

static void getLocalPayload(
  int nUsable,                    /* Usable bytes per page */
  u8 flags,                       /* Page flags */
  int nTotal,                     /* Total record (payload) size */
  int *pnLocal                    /* OUT: Bytes stored locally */
){
  int nLocal;
  int nMinLocal;
  int nMaxLocal;
 
  if( flags==0x0D ){              /* Table leaf node */
    nMinLocal = (nUsable - 12) * 32 / 255 - 23;
    nMaxLocal = nUsable - 35;
  }else{                          /* Index interior and leaf nodes */
    nMinLocal = (nUsable - 12) * 32 / 255 - 23;
    nMaxLocal = (nUsable - 12) * 64 / 255 - 23;
  }

  nLocal = nMinLocal + (nTotal - nMinLocal) % (nUsable - 4);
  if( nLocal>nMaxLocal ) nLocal = nMinLocal;
  *pnLocal = nLocal;
}

static int statDecodePage(Btree *pBt, StatPage *p){
  int nUnused;
  int iOff;
  int nHdr;
  int isLeaf;
  int szPage;

  u8 *aData = sqlite3PagerGetData(p->pPg);
  u8 *aHdr = &aData[p->iPgno==1 ? 100 : 0];

  p->flags = aHdr[0];
  p->nCell = get2byte(&aHdr[3]);
  p->nMxPayload = 0;

  isLeaf = (p->flags==0x0A || p->flags==0x0D);
  nHdr = 12 - isLeaf*4 + (p->iPgno==1)*100;

  nUnused = get2byte(&aHdr[5]) - nHdr - 2*p->nCell;
  nUnused += (int)aHdr[7];
  iOff = get2byte(&aHdr[1]);
  while( iOff ){
    nUnused += get2byte(&aData[iOff+2]);
    iOff = get2byte(&aData[iOff]);
  }
  p->nUnused = nUnused;
  p->iRightChildPg = isLeaf ? 0 : sqlite3Get4byte(&aHdr[8]);
  szPage = sqlite3BtreeGetPageSize(pBt);

  if( p->nCell ){
    int i;                        /* Used to iterate through cells */
    int nUsable = szPage - sqlite3BtreeGetReserve(pBt);

    p->aCell = sqlite3_malloc((p->nCell+1) * sizeof(StatCell));
    memset(p->aCell, 0, (p->nCell+1) * sizeof(StatCell));

    for(i=0; i<p->nCell; i++){
      StatCell *pCell = &p->aCell[i];

      iOff = get2byte(&aData[nHdr+i*2]);
      if( !isLeaf ){
        pCell->iChildPg = sqlite3Get4byte(&aData[iOff]);
        iOff += 4;
      }
      if( p->flags==0x05 ){
        /* A table interior node. nPayload==0. */
      }else{
        u32 nPayload;             /* Bytes of payload total (local+overflow) */
        int nLocal;               /* Bytes of payload stored locally */
        iOff += getVarint32(&aData[iOff], nPayload);
        if( p->flags==0x0D ){
          u64 dummy;
          iOff += sqlite3GetVarint(&aData[iOff], &dummy);
        }
        if( nPayload>(u32)p->nMxPayload ) p->nMxPayload = nPayload;
        getLocalPayload(nUsable, p->flags, nPayload, &nLocal);
        pCell->nLocal = nLocal;
        assert( nLocal>=0 );
        assert( nPayload>=(u32)nLocal );
        assert( nLocal<=(nUsable-35) );
        if( nPayload>(u32)nLocal ){
          int j;
          int nOvfl = ((nPayload - nLocal) + nUsable-4 - 1) / (nUsable - 4);
          pCell->nLastOvfl = (nPayload-nLocal) - (nOvfl-1) * (nUsable-4);
          pCell->nOvfl = nOvfl;
          pCell->aOvfl = sqlite3_malloc(sizeof(u32)*nOvfl);
          pCell->aOvfl[0] = sqlite3Get4byte(&aData[iOff+nLocal]);
          for(j=1; j<nOvfl; j++){
            int rc;
            u32 iPrev = pCell->aOvfl[j-1];
            DbPage *pPg = 0;
            rc = sqlite3PagerGet(sqlite3BtreePager(pBt), iPrev, &pPg);
            if( rc!=SQLITE_OK ){
              assert( pPg==0 );
              return rc;
            } 
            pCell->aOvfl[j] = sqlite3Get4byte(sqlite3PagerGetData(pPg));
            sqlite3PagerUnref(pPg);
          }
        }
      }
    }
  }

  return SQLITE_OK;
}

/*
** Populate the pCsr->iOffset and pCsr->szPage member variables. Based on
** the current value of pCsr->iPageno.
*/
static void statSizeAndOffset(StatCursor *pCsr){
  StatTable *pTab = (StatTable *)((sqlite3_vtab_cursor *)pCsr)->pVtab;
  Btree *pBt = pTab->db->aDb[0].pBt;
  Pager *pPager = sqlite3BtreePager(pBt);
  sqlite3_file *fd;
  sqlite3_int64 x[2];

  /* The default page size and offset */
  pCsr->szPage = sqlite3BtreeGetPageSize(pBt);
  pCsr->iOffset = (i64)pCsr->szPage * (pCsr->iPageno - 1);

  /* If connected to a ZIPVFS backend, override the page size and
  ** offset with actual values obtained from ZIPVFS.
  */
  fd = sqlite3PagerFile(pPager);
  x[0] = pCsr->iPageno;
  if( sqlite3OsFileControl(fd, 230440, &x)==SQLITE_OK ){
    pCsr->iOffset = x[0];
    pCsr->szPage = (int)x[1];
  }
}

/*
** Move a statvfs cursor to the next entry in the file.
*/
static int statNext(sqlite3_vtab_cursor *pCursor){
  int rc;
  int nPayload;
  StatCursor *pCsr = (StatCursor *)pCursor;
  StatTable *pTab = (StatTable *)pCursor->pVtab;
  Btree *pBt = pTab->db->aDb[0].pBt;
  Pager *pPager = sqlite3BtreePager(pBt);

  sqlite3_free(pCsr->zPath);
  pCsr->zPath = 0;

statNextRestart:
  if( pCsr->aPage[0].pPg==0 ){
    rc = sqlite3_step(pCsr->pStmt);
    if( rc==SQLITE_ROW ){
      int nPage;
      u32 iRoot = (u32)sqlite3_column_int64(pCsr->pStmt, 1);
      sqlite3PagerPagecount(pPager, &nPage);
      if( nPage==0 ){
        pCsr->isEof = 1;
        return sqlite3_reset(pCsr->pStmt);
      }
      rc = sqlite3PagerGet(pPager, iRoot, &pCsr->aPage[0].pPg);
      pCsr->aPage[0].iPgno = iRoot;
      pCsr->aPage[0].iCell = 0;
      pCsr->aPage[0].zPath = sqlite3_mprintf("/");
      pCsr->iPage = 0;
    }else{
      pCsr->isEof = 1;
      return sqlite3_reset(pCsr->pStmt);
    }
  }else{

    /* Page p itself has already been visited. */
    StatPage *p = &pCsr->aPage[pCsr->iPage];

    while( p->iCell<p->nCell ){
      StatCell *pCell = &p->aCell[p->iCell];
      if( pCell->iOvfl<pCell->nOvfl ){
        int nUsable = sqlite3BtreeGetPageSize(pBt)-sqlite3BtreeGetReserve(pBt);
        pCsr->zName = (char *)sqlite3_column_text(pCsr->pStmt, 0);
        pCsr->iPageno = pCell->aOvfl[pCell->iOvfl];
        pCsr->zPagetype = "overflow";
        pCsr->nCell = 0;
        pCsr->nMxPayload = 0;
        pCsr->zPath = sqlite3_mprintf(
            "%s%.3x+%.6x", p->zPath, p->iCell, pCell->iOvfl
        );
        if( pCell->iOvfl<pCell->nOvfl-1 ){
          pCsr->nUnused = 0;
          pCsr->nPayload = nUsable - 4;
        }else{
          pCsr->nPayload = pCell->nLastOvfl;
          pCsr->nUnused = nUsable - 4 - pCsr->nPayload;
        }
        pCell->iOvfl++;
        statSizeAndOffset(pCsr);
        return SQLITE_OK;
      }
      if( p->iRightChildPg ) break;
      p->iCell++;
    }

    if( !p->iRightChildPg || p->iCell>p->nCell ){
      statClearPage(p);
      if( pCsr->iPage==0 ) return statNext(pCursor);
      pCsr->iPage--;
      goto statNextRestart; /* Tail recursion */
    }
    pCsr->iPage++;
    assert( p==&pCsr->aPage[pCsr->iPage-1] );

    if( p->iCell==p->nCell ){
      p[1].iPgno = p->iRightChildPg;
    }else{
      p[1].iPgno = p->aCell[p->iCell].iChildPg;
    }
    rc = sqlite3PagerGet(pPager, p[1].iPgno, &p[1].pPg);
    p[1].iCell = 0;
    p[1].zPath = sqlite3_mprintf("%s%.3x/", p->zPath, p->iCell);
    p->iCell++;
  }


  /* Populate the StatCursor fields with the values to be returned
  ** by the xColumn() and xRowid() methods.
  */
  if( rc==SQLITE_OK ){
    int i;
    StatPage *p = &pCsr->aPage[pCsr->iPage];
    pCsr->zName = (char *)sqlite3_column_text(pCsr->pStmt, 0);
    pCsr->iPageno = p->iPgno;

    statDecodePage(pBt, p);
    statSizeAndOffset(pCsr);

    switch( p->flags ){
      case 0x05:             /* table internal */
      case 0x02:             /* index internal */
        pCsr->zPagetype = "internal";
        break;
      case 0x0D:             /* table leaf */
      case 0x0A:             /* index leaf */
        pCsr->zPagetype = "leaf";
        break;
      default:
        pCsr->zPagetype = "corrupted";
        break;
    }
    pCsr->nCell = p->nCell;
    pCsr->nUnused = p->nUnused;
    pCsr->nMxPayload = p->nMxPayload;
    pCsr->zPath = sqlite3_mprintf("%s", p->zPath);
    nPayload = 0;
    for(i=0; i<p->nCell; i++){
      nPayload += p->aCell[i].nLocal;
    }
    pCsr->nPayload = nPayload;
  }

  return rc;
}

static int statEof(sqlite3_vtab_cursor *pCursor){
  StatCursor *pCsr = (StatCursor *)pCursor;
  return pCsr->isEof;
}

static int statFilter(
  sqlite3_vtab_cursor *pCursor, 
  int idxNum, const char *idxStr,
  int argc, sqlite3_value **argv
){
  StatCursor *pCsr = (StatCursor *)pCursor;

  statResetCsr(pCsr);
  return statNext(pCursor);
}

static int statColumn(
  sqlite3_vtab_cursor *pCursor, 
  sqlite3_context *ctx, 
  int i
){
  StatCursor *pCsr = (StatCursor *)pCursor;
  switch( i ){
    case 0:            /* name */
      sqlite3_result_text(ctx, pCsr->zName, -1, SQLITE_STATIC);
      break;
    case 1:            /* path */
      sqlite3_result_text(ctx, pCsr->zPath, -1, SQLITE_TRANSIENT);
      break;
    case 2:            /* pageno */
      sqlite3_result_int64(ctx, pCsr->iPageno);
      break;
    case 3:            /* pagetype */
      sqlite3_result_text(ctx, pCsr->zPagetype, -1, SQLITE_STATIC);
      break;
    case 4:            /* ncell */
      sqlite3_result_int(ctx, pCsr->nCell);
      break;
    case 5:            /* payload */
      sqlite3_result_int(ctx, pCsr->nPayload);
      break;
    case 6:            /* unused */
      sqlite3_result_int(ctx, pCsr->nUnused);
      break;
    case 7:            /* mx_payload */
      sqlite3_result_int(ctx, pCsr->nMxPayload);
      break;
    case 8:            /* pgoffset */
      sqlite3_result_int64(ctx, pCsr->iOffset);
      break;
    case 9:            /* pgsize */
      sqlite3_result_int(ctx, pCsr->szPage);
      break;
  }
  return SQLITE_OK;
}

static int statRowid(sqlite3_vtab_cursor *pCursor, sqlite_int64 *pRowid){
  StatCursor *pCsr = (StatCursor *)pCursor;
  *pRowid = pCsr->iPageno;
  return SQLITE_OK;
}

int sqlite3_dbstat_register(sqlite3 *db){
  static sqlite3_module dbstat_module = {
    0,                            /* iVersion */
    statConnect,                  /* xCreate */
    statConnect,                  /* xConnect */
    statBestIndex,                /* xBestIndex */
    statDisconnect,               /* xDisconnect */
    statDisconnect,               /* xDestroy */
    statOpen,                     /* xOpen - open a cursor */
    statClose,                    /* xClose - close a cursor */
    statFilter,                   /* xFilter - configure scan constraints */
    statNext,                     /* xNext - advance a cursor */
    statEof,                      /* xEof - check for end of scan */
    statColumn,                   /* xColumn - read data */
    statRowid,                    /* xRowid - read data */
    0,                            /* xUpdate */
    0,                            /* xBegin */
    0,                            /* xSync */
    0,                            /* xCommit */
    0,                            /* xRollback */
    0,                            /* xFindMethod */
    0,                            /* xRename */
  };
  sqlite3_create_module(db, "dbstat", &dbstat_module, 0);
  return SQLITE_OK;
}

#endif

#if defined(SQLITE_TEST) || TCLSH==2
#include <tcl.h>

static int test_dbstat(
  void *clientData,
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){
#ifdef SQLITE_OMIT_VIRTUALTABLE
  Tcl_AppendResult(interp, "dbstat not available because of "
                           "SQLITE_OMIT_VIRTUALTABLE", (void*)0);
  return TCL_ERROR;
#else
  struct SqliteDb { sqlite3 *db; };
  char *zDb;
  Tcl_CmdInfo cmdInfo;

  if( objc!=2 ){
    Tcl_WrongNumArgs(interp, 1, objv, "DB");
    return TCL_ERROR;
  }

  zDb = Tcl_GetString(objv[1]);
  if( Tcl_GetCommandInfo(interp, zDb, &cmdInfo) ){
    sqlite3* db = ((struct SqliteDb*)cmdInfo.objClientData)->db;
    sqlite3_dbstat_register(db);
  }
  return TCL_OK;
#endif
}

int SqlitetestStat_Init(Tcl_Interp *interp){
  Tcl_CreateObjCommand(interp, "register_dbstat_vtab", test_dbstat, 0, 0);
  return TCL_OK;
}
#endif /* if defined(SQLITE_TEST) || TCLSH==2 */
/*
** 2001 September 15
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************
** A TCL Interface to SQLite.  Append this file to sqlite3.c and
** compile the whole thing to build a TCL-enabled version of SQLite.
**
** Compile-time options:
**
**  -DTCLSH=1             Add a "main()" routine that works as a tclsh.
**
**  -DSQLITE_TCLMD5       When used in conjuction with -DTCLSH=1, add
**                        four new commands to the TCL interpreter for
**                        generating MD5 checksums:  md5, md5file,
**                        md5-10x8, and md5file-10x8.
**
**  -DSQLITE_TEST         When used in conjuction with -DTCLSH=1, add
**                        hundreds of new commands used for testing
**                        SQLite.  This option implies -DSQLITE_TCLMD5.
*/

#include <errno.h>

/*
** Some additional include files are needed if this file is not
** appended to the amalgamation.
*/
#ifndef SQLITE_AMALGAMATION

# include <stdlib.h>
# include <string.h>
# include <assert.h>
  typedef unsigned char u8;
#endif
#include <ctype.h>

/* Used to get the current process ID */
#if !defined(_WIN32)
# include <unistd.h>
# define GETPID getpid
#elif !defined(_WIN32_WCE)
# ifndef SQLITE_AMALGAMATION
#  define WIN32_LEAN_AND_MEAN
#  include <windows.h>
# endif
# define GETPID (int)GetCurrentProcessId
#endif

/*
 * Windows needs to know which symbols to export.  Unix does not.
 * BUILD_sqlite should be undefined for Unix.
 */
#ifdef BUILD_sqlite
#undef TCL_STORAGE_CLASS
#define TCL_STORAGE_CLASS DLLEXPORT
#endif /* BUILD_sqlite */

#define NUM_PREPARED_STMTS 10
#define MAX_PREPARED_STMTS 100

/* Forward declaration */
typedef struct SqliteDb SqliteDb;

/*
** New SQL functions can be created as TCL scripts.  Each such function
** is described by an instance of the following structure.
*/
typedef struct SqlFunc SqlFunc;
struct SqlFunc {
  Tcl_Interp *interp;   /* The TCL interpret to execute the function */
  Tcl_Obj *pScript;     /* The Tcl_Obj representation of the script */
  SqliteDb *pDb;        /* Database connection that owns this function */
  int useEvalObjv;      /* True if it is safe to use Tcl_EvalObjv */
  char *zName;          /* Name of this function */
  SqlFunc *pNext;       /* Next function on the list of them all */
};

/*
** New collation sequences function can be created as TCL scripts.  Each such
** function is described by an instance of the following structure.
*/
typedef struct SqlCollate SqlCollate;
struct SqlCollate {
  Tcl_Interp *interp;   /* The TCL interpret to execute the function */
  char *zScript;        /* The script to be run */
  SqlCollate *pNext;    /* Next function on the list of them all */
};

/*
** Prepared statements are cached for faster execution.  Each prepared
** statement is described by an instance of the following structure.
*/
typedef struct SqlPreparedStmt SqlPreparedStmt;
struct SqlPreparedStmt {
  SqlPreparedStmt *pNext;  /* Next in linked list */
  SqlPreparedStmt *pPrev;  /* Previous on the list */
  sqlite3_stmt *pStmt;     /* The prepared statement */
  int nSql;                /* chars in zSql[] */
  const char *zSql;        /* Text of the SQL statement */
  int nParm;               /* Size of apParm array */
  Tcl_Obj **apParm;        /* Array of referenced object pointers */
};

typedef struct IncrblobChannel IncrblobChannel;

/*
** There is one instance of this structure for each SQLite database
** that has been opened by the SQLite TCL interface.
**
** If this module is built with SQLITE_TEST defined (to create the SQLite
** testfixture executable), then it may be configured to use either
** sqlite3_prepare_v2() or sqlite3_prepare() to prepare SQL statements.
** If SqliteDb.bLegacyPrepare is true, sqlite3_prepare() is used.
*/
struct SqliteDb {
  sqlite3 *db;               /* The "real" database structure. MUST BE FIRST */
  Tcl_Interp *interp;        /* The interpreter used for this database */
  char *zBusy;               /* The busy callback routine */
  char *zCommit;             /* The commit hook callback routine */
  char *zTrace;              /* The trace callback routine */
  char *zProfile;            /* The profile callback routine */
  char *zProgress;           /* The progress callback routine */
  char *zAuth;               /* The authorization callback routine */
  int disableAuth;           /* Disable the authorizer if it exists */
  char *zNull;               /* Text to substitute for an SQL NULL value */
  SqlFunc *pFunc;            /* List of SQL functions */
  Tcl_Obj *pUpdateHook;      /* Update hook script (if any) */
  Tcl_Obj *pRollbackHook;    /* Rollback hook script (if any) */
  Tcl_Obj *pWalHook;         /* WAL hook script (if any) */
  Tcl_Obj *pUnlockNotify;    /* Unlock notify script (if any) */
  SqlCollate *pCollate;      /* List of SQL collation functions */
  int rc;                    /* Return code of most recent sqlite3_exec() */
  Tcl_Obj *pCollateNeeded;   /* Collation needed script */
  SqlPreparedStmt *stmtList; /* List of prepared statements*/
  SqlPreparedStmt *stmtLast; /* Last statement in the list */
  int maxStmt;               /* The next maximum number of stmtList */
  int nStmt;                 /* Number of statements in stmtList */
  IncrblobChannel *pIncrblob;/* Linked list of open incrblob channels */
  int nStep, nSort, nIndex;  /* Statistics for most recent operation */
  int nTransaction;          /* Number of nested [transaction] methods */
#ifdef SQLITE_TEST
  int bLegacyPrepare;        /* True to use sqlite3_prepare() */
#endif
};

struct IncrblobChannel {
  sqlite3_blob *pBlob;      /* sqlite3 blob handle */
  SqliteDb *pDb;            /* Associated database connection */
  int iSeek;                /* Current seek offset */
  Tcl_Channel channel;      /* Channel identifier */
  IncrblobChannel *pNext;   /* Linked list of all open incrblob channels */
  IncrblobChannel *pPrev;   /* Linked list of all open incrblob channels */
};

/*
** Compute a string length that is limited to what can be stored in
** lower 30 bits of a 32-bit signed integer.
*/
static int strlen30(const char *z){
  const char *z2 = z;
  while( *z2 ){ z2++; }
  return 0x3fffffff & (int)(z2 - z);
}


#ifndef SQLITE_OMIT_INCRBLOB
/*
** Close all incrblob channels opened using database connection pDb.
** This is called when shutting down the database connection.
*/
static void closeIncrblobChannels(SqliteDb *pDb){
  IncrblobChannel *p;
  IncrblobChannel *pNext;

  for(p=pDb->pIncrblob; p; p=pNext){
    pNext = p->pNext;

    /* Note: Calling unregister here call Tcl_Close on the incrblob channel, 
    ** which deletes the IncrblobChannel structure at *p. So do not
    ** call Tcl_Free() here.
    */
    Tcl_UnregisterChannel(pDb->interp, p->channel);
  }
}

/*
** Close an incremental blob channel.
*/
static int incrblobClose(ClientData instanceData, Tcl_Interp *interp){
  IncrblobChannel *p = (IncrblobChannel *)instanceData;
  int rc = sqlite3_blob_close(p->pBlob);
  sqlite3 *db = p->pDb->db;

  /* Remove the channel from the SqliteDb.pIncrblob list. */
  if( p->pNext ){
    p->pNext->pPrev = p->pPrev;
  }
  if( p->pPrev ){
    p->pPrev->pNext = p->pNext;
  }
  if( p->pDb->pIncrblob==p ){
    p->pDb->pIncrblob = p->pNext;
  }

  /* Free the IncrblobChannel structure */
  Tcl_Free((char *)p);

  if( rc!=SQLITE_OK ){
    Tcl_SetResult(interp, (char *)sqlite3_errmsg(db), TCL_VOLATILE);
    return TCL_ERROR;
  }
  return TCL_OK;
}

/*
** Read data from an incremental blob channel.
*/
static int incrblobInput(
  ClientData instanceData, 
  char *buf, 
  int bufSize,
  int *errorCodePtr
){
  IncrblobChannel *p = (IncrblobChannel *)instanceData;
  int nRead = bufSize;         /* Number of bytes to read */
  int nBlob;                   /* Total size of the blob */
  int rc;                      /* sqlite error code */

  nBlob = sqlite3_blob_bytes(p->pBlob);
  if( (p->iSeek+nRead)>nBlob ){
    nRead = nBlob-p->iSeek;
  }
  if( nRead<=0 ){
    return 0;
  }

  rc = sqlite3_blob_read(p->pBlob, (void *)buf, nRead, p->iSeek);
  if( rc!=SQLITE_OK ){
    *errorCodePtr = rc;
    return -1;
  }

  p->iSeek += nRead;
  return nRead;
}

/*
** Write data to an incremental blob channel.
*/
static int incrblobOutput(
  ClientData instanceData, 
  CONST char *buf, 
  int toWrite,
  int *errorCodePtr
){
  IncrblobChannel *p = (IncrblobChannel *)instanceData;
  int nWrite = toWrite;        /* Number of bytes to write */
  int nBlob;                   /* Total size of the blob */
  int rc;                      /* sqlite error code */

  nBlob = sqlite3_blob_bytes(p->pBlob);
  if( (p->iSeek+nWrite)>nBlob ){
    *errorCodePtr = EINVAL;
    return -1;
  }
  if( nWrite<=0 ){
    return 0;
  }

  rc = sqlite3_blob_write(p->pBlob, (void *)buf, nWrite, p->iSeek);
  if( rc!=SQLITE_OK ){
    *errorCodePtr = EIO;
    return -1;
  }

  p->iSeek += nWrite;
  return nWrite;
}

/*
** Seek an incremental blob channel.
*/
static int incrblobSeek(
  ClientData instanceData, 
  long offset,
  int seekMode,
  int *errorCodePtr
){
  IncrblobChannel *p = (IncrblobChannel *)instanceData;

  switch( seekMode ){
    case SEEK_SET:
      p->iSeek = offset;
      break;
    case SEEK_CUR:
      p->iSeek += offset;
      break;
    case SEEK_END:
      p->iSeek = sqlite3_blob_bytes(p->pBlob) + offset;
      break;

    default: assert(!"Bad seekMode");
  }

  return p->iSeek;
}


static void incrblobWatch(ClientData instanceData, int mode){ 
  /* NO-OP */ 
}
static int incrblobHandle(ClientData instanceData, int dir, ClientData *hPtr){
  return TCL_ERROR;
}

static Tcl_ChannelType IncrblobChannelType = {
  "incrblob",                        /* typeName                             */
  TCL_CHANNEL_VERSION_2,             /* version                              */
  incrblobClose,                     /* closeProc                            */
  incrblobInput,                     /* inputProc                            */
  incrblobOutput,                    /* outputProc                           */
  incrblobSeek,                      /* seekProc                             */
  0,                                 /* setOptionProc                        */
  0,                                 /* getOptionProc                        */
  incrblobWatch,                     /* watchProc (this is a no-op)          */
  incrblobHandle,                    /* getHandleProc (always returns error) */
  0,                                 /* close2Proc                           */
  0,                                 /* blockModeProc                        */
  0,                                 /* flushProc                            */
  0,                                 /* handlerProc                          */
  0,                                 /* wideSeekProc                         */
};

/*
** Create a new incrblob channel.
*/
static int createIncrblobChannel(
  Tcl_Interp *interp, 
  SqliteDb *pDb, 
  const char *zDb,
  const char *zTable, 
  const char *zColumn, 
  sqlite_int64 iRow,
  int isReadonly
){
  IncrblobChannel *p;
  sqlite3 *db = pDb->db;
  sqlite3_blob *pBlob;
  int rc;
  int flags = TCL_READABLE|(isReadonly ? 0 : TCL_WRITABLE);

  /* This variable is used to name the channels: "incrblob_[incr count]" */
  static int count = 0;
  char zChannel[64];

  rc = sqlite3_blob_open(db, zDb, zTable, zColumn, iRow, !isReadonly, &pBlob);
  if( rc!=SQLITE_OK ){
    Tcl_SetResult(interp, (char *)sqlite3_errmsg(pDb->db), TCL_VOLATILE);
    return TCL_ERROR;
  }

  p = (IncrblobChannel *)Tcl_Alloc(sizeof(IncrblobChannel));
  p->iSeek = 0;
  p->pBlob = pBlob;

  sqlite3_snprintf(sizeof(zChannel), zChannel, "incrblob_%d", ++count);
  p->channel = Tcl_CreateChannel(&IncrblobChannelType, zChannel, p, flags);
  Tcl_RegisterChannel(interp, p->channel);

  /* Link the new channel into the SqliteDb.pIncrblob list. */
  p->pNext = pDb->pIncrblob;
  p->pPrev = 0;
  if( p->pNext ){
    p->pNext->pPrev = p;
  }
  pDb->pIncrblob = p;
  p->pDb = pDb;

  Tcl_SetResult(interp, (char *)Tcl_GetChannelName(p->channel), TCL_VOLATILE);
  return TCL_OK;
}
#else  /* else clause for "#ifndef SQLITE_OMIT_INCRBLOB" */
  #define closeIncrblobChannels(pDb)
#endif

/*
** Look at the script prefix in pCmd.  We will be executing this script
** after first appending one or more arguments.  This routine analyzes
** the script to see if it is safe to use Tcl_EvalObjv() on the script
** rather than the more general Tcl_EvalEx().  Tcl_EvalObjv() is much
** faster.
**
** Scripts that are safe to use with Tcl_EvalObjv() consists of a
** command name followed by zero or more arguments with no [...] or $
** or {...} or ; to be seen anywhere.  Most callback scripts consist
** of just a single procedure name and they meet this requirement.
*/
static int safeToUseEvalObjv(Tcl_Interp *interp, Tcl_Obj *pCmd){
  /* We could try to do something with Tcl_Parse().  But we will instead
  ** just do a search for forbidden characters.  If any of the forbidden
  ** characters appear in pCmd, we will report the string as unsafe.
  */
  const char *z;
  int n;
  z = Tcl_GetStringFromObj(pCmd, &n);
  while( n-- > 0 ){
    int c = *(z++);
    if( c=='$' || c=='[' || c==';' ) return 0;
  }
  return 1;
}

/*
** Find an SqlFunc structure with the given name.  Or create a new
** one if an existing one cannot be found.  Return a pointer to the
** structure.
*/
static SqlFunc *findSqlFunc(SqliteDb *pDb, const char *zName){
  SqlFunc *p, *pNew;
  int nName = strlen30(zName);
  pNew = (SqlFunc*)Tcl_Alloc( sizeof(*pNew) + nName + 1 );
  pNew->zName = (char*)&pNew[1];
  memcpy(pNew->zName, zName, nName+1);
  for(p=pDb->pFunc; p; p=p->pNext){ 
    if( sqlite3_stricmp(p->zName, pNew->zName)==0 ){
      Tcl_Free((char*)pNew);
      return p;
    }
  }
  pNew->interp = pDb->interp;
  pNew->pDb = pDb;
  pNew->pScript = 0;
  pNew->pNext = pDb->pFunc;
  pDb->pFunc = pNew;
  return pNew;
}

/*
** Free a single SqlPreparedStmt object.
*/
static void dbFreeStmt(SqlPreparedStmt *pStmt){
#ifdef SQLITE_TEST
  if( sqlite3_sql(pStmt->pStmt)==0 ){
    Tcl_Free((char *)pStmt->zSql);
  }
#endif
  sqlite3_finalize(pStmt->pStmt);
  Tcl_Free((char *)pStmt);
}

/*
** Finalize and free a list of prepared statements
*/
static void flushStmtCache(SqliteDb *pDb){
  SqlPreparedStmt *pPreStmt;
  SqlPreparedStmt *pNext;

  for(pPreStmt = pDb->stmtList; pPreStmt; pPreStmt=pNext){
    pNext = pPreStmt->pNext;
    dbFreeStmt(pPreStmt);
  }
  pDb->nStmt = 0;
  pDb->stmtLast = 0;
  pDb->stmtList = 0;
}

/*
** TCL calls this procedure when an sqlite3 database command is
** deleted.
*/
static void DbDeleteCmd(void *db){
  SqliteDb *pDb = (SqliteDb*)db;
  flushStmtCache(pDb);
  closeIncrblobChannels(pDb);
  sqlite3_close(pDb->db);
  while( pDb->pFunc ){
    SqlFunc *pFunc = pDb->pFunc;
    pDb->pFunc = pFunc->pNext;
    assert( pFunc->pDb==pDb );
    Tcl_DecrRefCount(pFunc->pScript);
    Tcl_Free((char*)pFunc);
  }
  while( pDb->pCollate ){
    SqlCollate *pCollate = pDb->pCollate;
    pDb->pCollate = pCollate->pNext;
    Tcl_Free((char*)pCollate);
  }
  if( pDb->zBusy ){
    Tcl_Free(pDb->zBusy);
  }
  if( pDb->zTrace ){
    Tcl_Free(pDb->zTrace);
  }
  if( pDb->zProfile ){
    Tcl_Free(pDb->zProfile);
  }
  if( pDb->zAuth ){
    Tcl_Free(pDb->zAuth);
  }
  if( pDb->zNull ){
    Tcl_Free(pDb->zNull);
  }
  if( pDb->pUpdateHook ){
    Tcl_DecrRefCount(pDb->pUpdateHook);
  }
  if( pDb->pRollbackHook ){
    Tcl_DecrRefCount(pDb->pRollbackHook);
  }
  if( pDb->pWalHook ){
    Tcl_DecrRefCount(pDb->pWalHook);
  }
  if( pDb->pCollateNeeded ){
    Tcl_DecrRefCount(pDb->pCollateNeeded);
  }
  Tcl_Free((char*)pDb);
}

/*
** This routine is called when a database file is locked while trying
** to execute SQL.
*/
static int DbBusyHandler(void *cd, int nTries){
  SqliteDb *pDb = (SqliteDb*)cd;
  int rc;
  char zVal[30];

  sqlite3_snprintf(sizeof(zVal), zVal, "%d", nTries);
  rc = Tcl_VarEval(pDb->interp, pDb->zBusy, " ", zVal, (char*)0);
  if( rc!=TCL_OK || atoi(Tcl_GetStringResult(pDb->interp)) ){
    return 0;
  }
  return 1;
}

#ifndef SQLITE_OMIT_PROGRESS_CALLBACK
/*
** This routine is invoked as the 'progress callback' for the database.
*/
static int DbProgressHandler(void *cd){
  SqliteDb *pDb = (SqliteDb*)cd;
  int rc;

  assert( pDb->zProgress );
  rc = Tcl_Eval(pDb->interp, pDb->zProgress);
  if( rc!=TCL_OK || atoi(Tcl_GetStringResult(pDb->interp)) ){
    return 1;
  }
  return 0;
}
#endif

#ifndef SQLITE_OMIT_TRACE
/*
** This routine is called by the SQLite trace handler whenever a new
** block of SQL is executed.  The TCL script in pDb->zTrace is executed.
*/
static void DbTraceHandler(void *cd, const char *zSql){
  SqliteDb *pDb = (SqliteDb*)cd;
  Tcl_DString str;

  Tcl_DStringInit(&str);
  Tcl_DStringAppend(&str, pDb->zTrace, -1);
  Tcl_DStringAppendElement(&str, zSql);
  Tcl_Eval(pDb->interp, Tcl_DStringValue(&str));
  Tcl_DStringFree(&str);
  Tcl_ResetResult(pDb->interp);
}
#endif

#ifndef SQLITE_OMIT_TRACE
/*
** This routine is called by the SQLite profile handler after a statement
** SQL has executed.  The TCL script in pDb->zProfile is evaluated.
*/
static void DbProfileHandler(void *cd, const char *zSql, sqlite_uint64 tm){
  SqliteDb *pDb = (SqliteDb*)cd;
  Tcl_DString str;
  char zTm[100];

  sqlite3_snprintf(sizeof(zTm)-1, zTm, "%lld", tm);
  Tcl_DStringInit(&str);
  Tcl_DStringAppend(&str, pDb->zProfile, -1);
  Tcl_DStringAppendElement(&str, zSql);
  Tcl_DStringAppendElement(&str, zTm);
  Tcl_Eval(pDb->interp, Tcl_DStringValue(&str));
  Tcl_DStringFree(&str);
  Tcl_ResetResult(pDb->interp);
}
#endif

/*
** This routine is called when a transaction is committed.  The
** TCL script in pDb->zCommit is executed.  If it returns non-zero or
** if it throws an exception, the transaction is rolled back instead
** of being committed.
*/
static int DbCommitHandler(void *cd){
  SqliteDb *pDb = (SqliteDb*)cd;
  int rc;

  rc = Tcl_Eval(pDb->interp, pDb->zCommit);
  if( rc!=TCL_OK || atoi(Tcl_GetStringResult(pDb->interp)) ){
    return 1;
  }
  return 0;
}

static void DbRollbackHandler(void *clientData){
  SqliteDb *pDb = (SqliteDb*)clientData;
  assert(pDb->pRollbackHook);
  if( TCL_OK!=Tcl_EvalObjEx(pDb->interp, pDb->pRollbackHook, 0) ){
    Tcl_BackgroundError(pDb->interp);
  }
}

/*
** This procedure handles wal_hook callbacks.
*/
static int DbWalHandler(
  void *clientData, 
  sqlite3 *db, 
  const char *zDb, 
  int nEntry
){
  int ret = SQLITE_OK;
  Tcl_Obj *p;
  SqliteDb *pDb = (SqliteDb*)clientData;
  Tcl_Interp *interp = pDb->interp;
  assert(pDb->pWalHook);

  p = Tcl_DuplicateObj(pDb->pWalHook);
  Tcl_IncrRefCount(p);
  Tcl_ListObjAppendElement(interp, p, Tcl_NewStringObj(zDb, -1));
  Tcl_ListObjAppendElement(interp, p, Tcl_NewIntObj(nEntry));
  if( TCL_OK!=Tcl_EvalObjEx(interp, p, 0) 
   || TCL_OK!=Tcl_GetIntFromObj(interp, Tcl_GetObjResult(interp), &ret)
  ){
    Tcl_BackgroundError(interp);
  }
  Tcl_DecrRefCount(p);

  return ret;
}

#if defined(SQLITE_TEST) && defined(SQLITE_ENABLE_UNLOCK_NOTIFY)
static void setTestUnlockNotifyVars(Tcl_Interp *interp, int iArg, int nArg){
  char zBuf[64];
  sprintf(zBuf, "%d", iArg);
  Tcl_SetVar(interp, "sqlite_unlock_notify_arg", zBuf, TCL_GLOBAL_ONLY);
  sprintf(zBuf, "%d", nArg);
  Tcl_SetVar(interp, "sqlite_unlock_notify_argcount", zBuf, TCL_GLOBAL_ONLY);
}
#else
# define setTestUnlockNotifyVars(x,y,z)
#endif

#ifdef SQLITE_ENABLE_UNLOCK_NOTIFY
static void DbUnlockNotify(void **apArg, int nArg){
  int i;
  for(i=0; i<nArg; i++){
    const int flags = (TCL_EVAL_GLOBAL|TCL_EVAL_DIRECT);
    SqliteDb *pDb = (SqliteDb *)apArg[i];
    setTestUnlockNotifyVars(pDb->interp, i, nArg);
    assert( pDb->pUnlockNotify);
    Tcl_EvalObjEx(pDb->interp, pDb->pUnlockNotify, flags);
    Tcl_DecrRefCount(pDb->pUnlockNotify);
    pDb->pUnlockNotify = 0;
  }
}
#endif

static void DbUpdateHandler(
  void *p, 
  int op,
  const char *zDb, 
  const char *zTbl, 
  sqlite_int64 rowid
){
  SqliteDb *pDb = (SqliteDb *)p;
  Tcl_Obj *pCmd;

  assert( pDb->pUpdateHook );
  assert( op==SQLITE_INSERT || op==SQLITE_UPDATE || op==SQLITE_DELETE );

  pCmd = Tcl_DuplicateObj(pDb->pUpdateHook);
  Tcl_IncrRefCount(pCmd);
  Tcl_ListObjAppendElement(0, pCmd, Tcl_NewStringObj(
    ( (op==SQLITE_INSERT)?"INSERT":(op==SQLITE_UPDATE)?"UPDATE":"DELETE"), -1));
  Tcl_ListObjAppendElement(0, pCmd, Tcl_NewStringObj(zDb, -1));
  Tcl_ListObjAppendElement(0, pCmd, Tcl_NewStringObj(zTbl, -1));
  Tcl_ListObjAppendElement(0, pCmd, Tcl_NewWideIntObj(rowid));
  Tcl_EvalObjEx(pDb->interp, pCmd, TCL_EVAL_DIRECT);
  Tcl_DecrRefCount(pCmd);
}

static void tclCollateNeeded(
  void *pCtx,
  sqlite3 *db,
  int enc,
  const char *zName
){
  SqliteDb *pDb = (SqliteDb *)pCtx;
  Tcl_Obj *pScript = Tcl_DuplicateObj(pDb->pCollateNeeded);
  Tcl_IncrRefCount(pScript);
  Tcl_ListObjAppendElement(0, pScript, Tcl_NewStringObj(zName, -1));
  Tcl_EvalObjEx(pDb->interp, pScript, 0);
  Tcl_DecrRefCount(pScript);
}

/*
** This routine is called to evaluate an SQL collation function implemented
** using TCL script.
*/
static int tclSqlCollate(
  void *pCtx,
  int nA,
  const void *zA,
  int nB,
  const void *zB
){
  SqlCollate *p = (SqlCollate *)pCtx;
  Tcl_Obj *pCmd;

  pCmd = Tcl_NewStringObj(p->zScript, -1);
  Tcl_IncrRefCount(pCmd);
  Tcl_ListObjAppendElement(p->interp, pCmd, Tcl_NewStringObj(zA, nA));
  Tcl_ListObjAppendElement(p->interp, pCmd, Tcl_NewStringObj(zB, nB));
  Tcl_EvalObjEx(p->interp, pCmd, TCL_EVAL_DIRECT);
  Tcl_DecrRefCount(pCmd);
  return (atoi(Tcl_GetStringResult(p->interp)));
}

/*
** This routine is called to evaluate an SQL function implemented
** using TCL script.
*/
static void tclSqlFunc(sqlite3_context *context, int argc, sqlite3_value**argv){
  SqlFunc *p = sqlite3_user_data(context);
  Tcl_Obj *pCmd;
  int i;
  int rc;

  if( argc==0 ){
    /* If there are no arguments to the function, call Tcl_EvalObjEx on the
    ** script object directly.  This allows the TCL compiler to generate
    ** bytecode for the command on the first invocation and thus make
    ** subsequent invocations much faster. */
    pCmd = p->pScript;
    Tcl_IncrRefCount(pCmd);
    rc = Tcl_EvalObjEx(p->interp, pCmd, 0);
    Tcl_DecrRefCount(pCmd);
  }else{
    /* If there are arguments to the function, make a shallow copy of the
    ** script object, lappend the arguments, then evaluate the copy.
    **
    ** By "shallow" copy, we mean a only the outer list Tcl_Obj is duplicated.
    ** The new Tcl_Obj contains pointers to the original list elements. 
    ** That way, when Tcl_EvalObjv() is run and shimmers the first element
    ** of the list to tclCmdNameType, that alternate representation will
    ** be preserved and reused on the next invocation.
    */
    Tcl_Obj **aArg;
    int nArg;
    if( Tcl_ListObjGetElements(p->interp, p->pScript, &nArg, &aArg) ){
      sqlite3_result_error(context, Tcl_GetStringResult(p->interp), -1); 
      return;
    }     
    pCmd = Tcl_NewListObj(nArg, aArg);
    Tcl_IncrRefCount(pCmd);
    for(i=0; i<argc; i++){
      sqlite3_value *pIn = argv[i];
      Tcl_Obj *pVal;
            
      /* Set pVal to contain the i'th column of this row. */
      switch( sqlite3_value_type(pIn) ){
        case SQLITE_BLOB: {
          int bytes = sqlite3_value_bytes(pIn);
          pVal = Tcl_NewByteArrayObj(sqlite3_value_blob(pIn), bytes);
          break;
        }
        case SQLITE_INTEGER: {
          sqlite_int64 v = sqlite3_value_int64(pIn);
          if( v>=-2147483647 && v<=2147483647 ){
            pVal = Tcl_NewIntObj((int)v);
          }else{
            pVal = Tcl_NewWideIntObj(v);
          }
          break;
        }
        case SQLITE_FLOAT: {
          double r = sqlite3_value_double(pIn);
          pVal = Tcl_NewDoubleObj(r);
          break;
        }
        case SQLITE_NULL: {
          pVal = Tcl_NewStringObj(p->pDb->zNull, -1);
          break;
        }
        default: {
          int bytes = sqlite3_value_bytes(pIn);
          pVal = Tcl_NewStringObj((char *)sqlite3_value_text(pIn), bytes);
          break;
        }
      }
      rc = Tcl_ListObjAppendElement(p->interp, pCmd, pVal);
      if( rc ){
        Tcl_DecrRefCount(pCmd);
        sqlite3_result_error(context, Tcl_GetStringResult(p->interp), -1); 
        return;
      }
    }
    if( !p->useEvalObjv ){
      /* Tcl_EvalObjEx() will automatically call Tcl_EvalObjv() if pCmd
      ** is a list without a string representation.  To prevent this from
      ** happening, make sure pCmd has a valid string representation */
      Tcl_GetString(pCmd);
    }
    rc = Tcl_EvalObjEx(p->interp, pCmd, TCL_EVAL_DIRECT);
    Tcl_DecrRefCount(pCmd);
  }

  if( rc && rc!=TCL_RETURN ){
    sqlite3_result_error(context, Tcl_GetStringResult(p->interp), -1); 
  }else{
    Tcl_Obj *pVar = Tcl_GetObjResult(p->interp);
    int n;
    u8 *data;
    const char *zType = (pVar->typePtr ? pVar->typePtr->name : "");
    char c = zType[0];
    if( c=='b' && strcmp(zType,"bytearray")==0 && pVar->bytes==0 ){
      /* Only return a BLOB type if the Tcl variable is a bytearray and
      ** has no string representation. */
      data = Tcl_GetByteArrayFromObj(pVar, &n);
      sqlite3_result_blob(context, data, n, SQLITE_TRANSIENT);
    }else if( c=='b' && strcmp(zType,"boolean")==0 ){
      Tcl_GetIntFromObj(0, pVar, &n);
      sqlite3_result_int(context, n);
    }else if( c=='d' && strcmp(zType,"double")==0 ){
      double r;
      Tcl_GetDoubleFromObj(0, pVar, &r);
      sqlite3_result_double(context, r);
    }else if( (c=='w' && strcmp(zType,"wideInt")==0) ||
          (c=='i' && strcmp(zType,"int")==0) ){
      Tcl_WideInt v;
      Tcl_GetWideIntFromObj(0, pVar, &v);
      sqlite3_result_int64(context, v);
    }else{
      data = (unsigned char *)Tcl_GetStringFromObj(pVar, &n);
      sqlite3_result_text(context, (char *)data, n, SQLITE_TRANSIENT);
    }
  }
}

#ifndef SQLITE_OMIT_AUTHORIZATION
/*
** This is the authentication function.  It appends the authentication
** type code and the two arguments to zCmd[] then invokes the result
** on the interpreter.  The reply is examined to determine if the
** authentication fails or succeeds.
*/
static int auth_callback(
  void *pArg,
  int code,
  const char *zArg1,
  const char *zArg2,
  const char *zArg3,
  const char *zArg4
){
  const char *zCode;
  Tcl_DString str;
  int rc;
  const char *zReply;
  SqliteDb *pDb = (SqliteDb*)pArg;
  if( pDb->disableAuth ) return SQLITE_OK;

  switch( code ){
    case SQLITE_COPY              : zCode="SQLITE_COPY"; break;
    case SQLITE_CREATE_INDEX      : zCode="SQLITE_CREATE_INDEX"; break;
    case SQLITE_CREATE_TABLE      : zCode="SQLITE_CREATE_TABLE"; break;
    case SQLITE_CREATE_TEMP_INDEX : zCode="SQLITE_CREATE_TEMP_INDEX"; break;
    case SQLITE_CREATE_TEMP_TABLE : zCode="SQLITE_CREATE_TEMP_TABLE"; break;
    case SQLITE_CREATE_TEMP_TRIGGER: zCode="SQLITE_CREATE_TEMP_TRIGGER"; break;
    case SQLITE_CREATE_TEMP_VIEW  : zCode="SQLITE_CREATE_TEMP_VIEW"; break;
    case SQLITE_CREATE_TRIGGER    : zCode="SQLITE_CREATE_TRIGGER"; break;
    case SQLITE_CREATE_VIEW       : zCode="SQLITE_CREATE_VIEW"; break;
    case SQLITE_DELETE            : zCode="SQLITE_DELETE"; break;
    case SQLITE_DROP_INDEX        : zCode="SQLITE_DROP_INDEX"; break;
    case SQLITE_DROP_TABLE        : zCode="SQLITE_DROP_TABLE"; break;
    case SQLITE_DROP_TEMP_INDEX   : zCode="SQLITE_DROP_TEMP_INDEX"; break;
    case SQLITE_DROP_TEMP_TABLE   : zCode="SQLITE_DROP_TEMP_TABLE"; break;
    case SQLITE_DROP_TEMP_TRIGGER : zCode="SQLITE_DROP_TEMP_TRIGGER"; break;
    case SQLITE_DROP_TEMP_VIEW    : zCode="SQLITE_DROP_TEMP_VIEW"; break;
    case SQLITE_DROP_TRIGGER      : zCode="SQLITE_DROP_TRIGGER"; break;
    case SQLITE_DROP_VIEW         : zCode="SQLITE_DROP_VIEW"; break;
    case SQLITE_INSERT            : zCode="SQLITE_INSERT"; break;
    case SQLITE_PRAGMA            : zCode="SQLITE_PRAGMA"; break;
    case SQLITE_READ              : zCode="SQLITE_READ"; break;
    case SQLITE_SELECT            : zCode="SQLITE_SELECT"; break;
    case SQLITE_TRANSACTION       : zCode="SQLITE_TRANSACTION"; break;
    case SQLITE_UPDATE            : zCode="SQLITE_UPDATE"; break;
    case SQLITE_ATTACH            : zCode="SQLITE_ATTACH"; break;
    case SQLITE_DETACH            : zCode="SQLITE_DETACH"; break;
    case SQLITE_ALTER_TABLE       : zCode="SQLITE_ALTER_TABLE"; break;
    case SQLITE_REINDEX           : zCode="SQLITE_REINDEX"; break;
    case SQLITE_ANALYZE           : zCode="SQLITE_ANALYZE"; break;
    case SQLITE_CREATE_VTABLE     : zCode="SQLITE_CREATE_VTABLE"; break;
    case SQLITE_DROP_VTABLE       : zCode="SQLITE_DROP_VTABLE"; break;
    case SQLITE_FUNCTION          : zCode="SQLITE_FUNCTION"; break;
    case SQLITE_SAVEPOINT         : zCode="SQLITE_SAVEPOINT"; break;
    case SQLITE_RECURSIVE         : zCode="SQLITE_RECURSIVE"; break;
    default                       : zCode="????"; break;
  }
  Tcl_DStringInit(&str);
  Tcl_DStringAppend(&str, pDb->zAuth, -1);
  Tcl_DStringAppendElement(&str, zCode);
  Tcl_DStringAppendElement(&str, zArg1 ? zArg1 : "");
  Tcl_DStringAppendElement(&str, zArg2 ? zArg2 : "");
  Tcl_DStringAppendElement(&str, zArg3 ? zArg3 : "");
  Tcl_DStringAppendElement(&str, zArg4 ? zArg4 : "");
  rc = Tcl_GlobalEval(pDb->interp, Tcl_DStringValue(&str));
  Tcl_DStringFree(&str);
  zReply = rc==TCL_OK ? Tcl_GetStringResult(pDb->interp) : "SQLITE_DENY";
  if( strcmp(zReply,"SQLITE_OK")==0 ){
    rc = SQLITE_OK;
  }else if( strcmp(zReply,"SQLITE_DENY")==0 ){
    rc = SQLITE_DENY;
  }else if( strcmp(zReply,"SQLITE_IGNORE")==0 ){
    rc = SQLITE_IGNORE;
  }else{
    rc = 999;
  }
  return rc;
}
#endif /* SQLITE_OMIT_AUTHORIZATION */

/*
** This routine reads a line of text from FILE in, stores
** the text in memory obtained from malloc() and returns a pointer
** to the text.  NULL is returned at end of file, or if malloc()
** fails.
**
** The interface is like "readline" but no command-line editing
** is done.
**
** copied from shell.c from '.import' command
*/
static char *local_getline(char *zPrompt, FILE *in){
  char *zLine;
  int nLine;
  int n;

  nLine = 100;
  zLine = malloc( nLine );
  if( zLine==0 ) return 0;
  n = 0;
  while( 1 ){
    if( n+100>nLine ){
      nLine = nLine*2 + 100;
      zLine = realloc(zLine, nLine);
      if( zLine==0 ) return 0;
    }
    if( fgets(&zLine[n], nLine - n, in)==0 ){
      if( n==0 ){
        free(zLine);
        return 0;
      }
      zLine[n] = 0;
      break;
    }
    while( zLine[n] ){ n++; }
    if( n>0 && zLine[n-1]=='\n' ){
      n--;
      zLine[n] = 0;
      break;
    }
  }
  zLine = realloc( zLine, n+1 );
  return zLine;
}


/*
** This function is part of the implementation of the command:
**
**   $db transaction [-deferred|-immediate|-exclusive] SCRIPT
**
** It is invoked after evaluating the script SCRIPT to commit or rollback
** the transaction or savepoint opened by the [transaction] command.
*/
static int DbTransPostCmd(
  ClientData data[],                   /* data[0] is the Sqlite3Db* for $db */
  Tcl_Interp *interp,                  /* Tcl interpreter */
  int result                           /* Result of evaluating SCRIPT */
){
  static const char *const azEnd[] = {
    "RELEASE _tcl_transaction",        /* rc==TCL_ERROR, nTransaction!=0 */
    "COMMIT",                          /* rc!=TCL_ERROR, nTransaction==0 */
    "ROLLBACK TO _tcl_transaction ; RELEASE _tcl_transaction",
    "ROLLBACK"                         /* rc==TCL_ERROR, nTransaction==0 */
  };
  SqliteDb *pDb = (SqliteDb*)data[0];
  int rc = result;
  const char *zEnd;

  pDb->nTransaction--;
  zEnd = azEnd[(rc==TCL_ERROR)*2 + (pDb->nTransaction==0)];

  pDb->disableAuth++;
  if( sqlite3_exec(pDb->db, zEnd, 0, 0, 0) ){
      /* This is a tricky scenario to handle. The most likely cause of an
      ** error is that the exec() above was an attempt to commit the 
      ** top-level transaction that returned SQLITE_BUSY. Or, less likely,
      ** that an IO-error has occurred. In either case, throw a Tcl exception
      ** and try to rollback the transaction.
      **
      ** But it could also be that the user executed one or more BEGIN, 
      ** COMMIT, SAVEPOINT, RELEASE or ROLLBACK commands that are confusing
      ** this method's logic. Not clear how this would be best handled.
      */
    if( rc!=TCL_ERROR ){
      Tcl_AppendResult(interp, sqlite3_errmsg(pDb->db), (char*)0);
      rc = TCL_ERROR;
    }
    sqlite3_exec(pDb->db, "ROLLBACK", 0, 0, 0);
  }
  pDb->disableAuth--;

  return rc;
}

/*
** Unless SQLITE_TEST is defined, this function is a simple wrapper around
** sqlite3_prepare_v2(). If SQLITE_TEST is defined, then it uses either
** sqlite3_prepare_v2() or legacy interface sqlite3_prepare(), depending
** on whether or not the [db_use_legacy_prepare] command has been used to 
** configure the connection.
*/
static int dbPrepare(
  SqliteDb *pDb,                  /* Database object */
  const char *zSql,               /* SQL to compile */
  sqlite3_stmt **ppStmt,          /* OUT: Prepared statement */
  const char **pzOut              /* OUT: Pointer to next SQL statement */
){
#ifdef SQLITE_TEST
  if( pDb->bLegacyPrepare ){
    return sqlite3_prepare(pDb->db, zSql, -1, ppStmt, pzOut);
  }
#endif
  return sqlite3_prepare_v2(pDb->db, zSql, -1, ppStmt, pzOut);
}

/*
** Search the cache for a prepared-statement object that implements the
** first SQL statement in the buffer pointed to by parameter zIn. If
** no such prepared-statement can be found, allocate and prepare a new
** one. In either case, bind the current values of the relevant Tcl
** variables to any $var, :var or @var variables in the statement. Before
** returning, set *ppPreStmt to point to the prepared-statement object.
**
** Output parameter *pzOut is set to point to the next SQL statement in
** buffer zIn, or to the '\0' byte at the end of zIn if there is no
** next statement.
**
** If successful, TCL_OK is returned. Otherwise, TCL_ERROR is returned
** and an error message loaded into interpreter pDb->interp.
*/
static int dbPrepareAndBind(
  SqliteDb *pDb,                  /* Database object */
  char const *zIn,                /* SQL to compile */
  char const **pzOut,             /* OUT: Pointer to next SQL statement */
  SqlPreparedStmt **ppPreStmt     /* OUT: Object used to cache statement */
){
  const char *zSql = zIn;         /* Pointer to first SQL statement in zIn */
  sqlite3_stmt *pStmt;            /* Prepared statement object */
  SqlPreparedStmt *pPreStmt;      /* Pointer to cached statement */
  int nSql;                       /* Length of zSql in bytes */
  int nVar;                       /* Number of variables in statement */
  int iParm = 0;                  /* Next free entry in apParm */
  char c;
  int i;
  Tcl_Interp *interp = pDb->interp;

  *ppPreStmt = 0;

  /* Trim spaces from the start of zSql and calculate the remaining length. */
  while( (c = zSql[0])==' ' || c=='\t' || c=='\r' || c=='\n' ){ zSql++; }
  nSql = strlen30(zSql);

  for(pPreStmt = pDb->stmtList; pPreStmt; pPreStmt=pPreStmt->pNext){
    int n = pPreStmt->nSql;
    if( nSql>=n 
        && memcmp(pPreStmt->zSql, zSql, n)==0
        && (zSql[n]==0 || zSql[n-1]==';')
    ){
      pStmt = pPreStmt->pStmt;
      *pzOut = &zSql[pPreStmt->nSql];

      /* When a prepared statement is found, unlink it from the
      ** cache list.  It will later be added back to the beginning
      ** of the cache list in order to implement LRU replacement.
      */
      if( pPreStmt->pPrev ){
        pPreStmt->pPrev->pNext = pPreStmt->pNext;
      }else{
        pDb->stmtList = pPreStmt->pNext;
      }
      if( pPreStmt->pNext ){
        pPreStmt->pNext->pPrev = pPreStmt->pPrev;
      }else{
        pDb->stmtLast = pPreStmt->pPrev;
      }
      pDb->nStmt--;
      nVar = sqlite3_bind_parameter_count(pStmt);
      break;
    }
  }
  
  /* If no prepared statement was found. Compile the SQL text. Also allocate
  ** a new SqlPreparedStmt structure.  */
  if( pPreStmt==0 ){
    int nByte;

    if( SQLITE_OK!=dbPrepare(pDb, zSql, &pStmt, pzOut) ){
      Tcl_SetObjResult(interp, Tcl_NewStringObj(sqlite3_errmsg(pDb->db), -1));
      return TCL_ERROR;
    }
    if( pStmt==0 ){
      if( SQLITE_OK!=sqlite3_errcode(pDb->db) ){
        /* A compile-time error in the statement. */
        Tcl_SetObjResult(interp, Tcl_NewStringObj(sqlite3_errmsg(pDb->db), -1));
        return TCL_ERROR;
      }else{
        /* The statement was a no-op.  Continue to the next statement
        ** in the SQL string.
        */
        return TCL_OK;
      }
    }

    assert( pPreStmt==0 );
    nVar = sqlite3_bind_parameter_count(pStmt);
    nByte = sizeof(SqlPreparedStmt) + nVar*sizeof(Tcl_Obj *);
    pPreStmt = (SqlPreparedStmt*)Tcl_Alloc(nByte);
    memset(pPreStmt, 0, nByte);

    pPreStmt->pStmt = pStmt;
    pPreStmt->nSql = (int)(*pzOut - zSql);
    pPreStmt->zSql = sqlite3_sql(pStmt);
    pPreStmt->apParm = (Tcl_Obj **)&pPreStmt[1];
#ifdef SQLITE_TEST
    if( pPreStmt->zSql==0 ){
      char *zCopy = Tcl_Alloc(pPreStmt->nSql + 1);
      memcpy(zCopy, zSql, pPreStmt->nSql);
      zCopy[pPreStmt->nSql] = '\0';
      pPreStmt->zSql = zCopy;
    }
#endif
  }
  assert( pPreStmt );
  assert( strlen30(pPreStmt->zSql)==pPreStmt->nSql );
  assert( 0==memcmp(pPreStmt->zSql, zSql, pPreStmt->nSql) );

  /* Bind values to parameters that begin with $ or : */  
  for(i=1; i<=nVar; i++){
    const char *zVar = sqlite3_bind_parameter_name(pStmt, i);
    if( zVar!=0 && (zVar[0]=='$' || zVar[0]==':' || zVar[0]=='@') ){
      Tcl_Obj *pVar = Tcl_GetVar2Ex(interp, &zVar[1], 0, 0);
      if( pVar ){
        int n;
        u8 *data;
        const char *zType = (pVar->typePtr ? pVar->typePtr->name : "");
        char c = zType[0];
        if( zVar[0]=='@' ||
           (c=='b' && strcmp(zType,"bytearray")==0 && pVar->bytes==0) ){
          /* Load a BLOB type if the Tcl variable is a bytearray and
          ** it has no string representation or the host
          ** parameter name begins with "@". */
          data = Tcl_GetByteArrayFromObj(pVar, &n);
          sqlite3_bind_blob(pStmt, i, data, n, SQLITE_STATIC);
          Tcl_IncrRefCount(pVar);
          pPreStmt->apParm[iParm++] = pVar;
        }else if( c=='b' && strcmp(zType,"boolean")==0 ){
          Tcl_GetIntFromObj(interp, pVar, &n);
          sqlite3_bind_int(pStmt, i, n);
        }else if( c=='d' && strcmp(zType,"double")==0 ){
          double r;
          Tcl_GetDoubleFromObj(interp, pVar, &r);
          sqlite3_bind_double(pStmt, i, r);
        }else if( (c=='w' && strcmp(zType,"wideInt")==0) ||
              (c=='i' && strcmp(zType,"int")==0) ){
          Tcl_WideInt v;
          Tcl_GetWideIntFromObj(interp, pVar, &v);
          sqlite3_bind_int64(pStmt, i, v);
        }else{
          data = (unsigned char *)Tcl_GetStringFromObj(pVar, &n);
          sqlite3_bind_text(pStmt, i, (char *)data, n, SQLITE_STATIC);
          Tcl_IncrRefCount(pVar);
          pPreStmt->apParm[iParm++] = pVar;
        }
      }else{
        sqlite3_bind_null(pStmt, i);
      }
    }
  }
  pPreStmt->nParm = iParm;
  *ppPreStmt = pPreStmt;

  return TCL_OK;
}

/*
** Release a statement reference obtained by calling dbPrepareAndBind().
** There should be exactly one call to this function for each call to
** dbPrepareAndBind().
**
** If the discard parameter is non-zero, then the statement is deleted
** immediately. Otherwise it is added to the LRU list and may be returned
** by a subsequent call to dbPrepareAndBind().
*/
static void dbReleaseStmt(
  SqliteDb *pDb,                  /* Database handle */
  SqlPreparedStmt *pPreStmt,      /* Prepared statement handle to release */
  int discard                     /* True to delete (not cache) the pPreStmt */
){
  int i;

  /* Free the bound string and blob parameters */
  for(i=0; i<pPreStmt->nParm; i++){
    Tcl_DecrRefCount(pPreStmt->apParm[i]);
  }
  pPreStmt->nParm = 0;

  if( pDb->maxStmt<=0 || discard ){
    /* If the cache is turned off, deallocated the statement */
    dbFreeStmt(pPreStmt);
  }else{
    /* Add the prepared statement to the beginning of the cache list. */
    pPreStmt->pNext = pDb->stmtList;
    pPreStmt->pPrev = 0;
    if( pDb->stmtList ){
     pDb->stmtList->pPrev = pPreStmt;
    }
    pDb->stmtList = pPreStmt;
    if( pDb->stmtLast==0 ){
      assert( pDb->nStmt==0 );
      pDb->stmtLast = pPreStmt;
    }else{
      assert( pDb->nStmt>0 );
    }
    pDb->nStmt++;
   
    /* If we have too many statement in cache, remove the surplus from 
    ** the end of the cache list.  */
    while( pDb->nStmt>pDb->maxStmt ){
      SqlPreparedStmt *pLast = pDb->stmtLast;
      pDb->stmtLast = pLast->pPrev;
      pDb->stmtLast->pNext = 0;
      pDb->nStmt--;
      dbFreeStmt(pLast);
    }
  }
}

/*
** Structure used with dbEvalXXX() functions:
**
**   dbEvalInit()
**   dbEvalStep()
**   dbEvalFinalize()
**   dbEvalRowInfo()
**   dbEvalColumnValue()
*/
typedef struct DbEvalContext DbEvalContext;
struct DbEvalContext {
  SqliteDb *pDb;                  /* Database handle */
  Tcl_Obj *pSql;                  /* Object holding string zSql */
  const char *zSql;               /* Remaining SQL to execute */
  SqlPreparedStmt *pPreStmt;      /* Current statement */
  int nCol;                       /* Number of columns returned by pStmt */
  Tcl_Obj *pArray;                /* Name of array variable */
  Tcl_Obj **apColName;            /* Array of column names */
};

/*
** Release any cache of column names currently held as part of
** the DbEvalContext structure passed as the first argument.
*/
static void dbReleaseColumnNames(DbEvalContext *p){
  if( p->apColName ){
    int i;
    for(i=0; i<p->nCol; i++){
      Tcl_DecrRefCount(p->apColName[i]);
    }
    Tcl_Free((char *)p->apColName);
    p->apColName = 0;
  }
  p->nCol = 0;
}

/*
** Initialize a DbEvalContext structure.
**
** If pArray is not NULL, then it contains the name of a Tcl array
** variable. The "*" member of this array is set to a list containing
** the names of the columns returned by the statement as part of each
** call to dbEvalStep(), in order from left to right. e.g. if the names 
** of the returned columns are a, b and c, it does the equivalent of the 
** tcl command:
**
**     set ${pArray}(*) {a b c}
*/
static void dbEvalInit(
  DbEvalContext *p,               /* Pointer to structure to initialize */
  SqliteDb *pDb,                  /* Database handle */
  Tcl_Obj *pSql,                  /* Object containing SQL script */
  Tcl_Obj *pArray                 /* Name of Tcl array to set (*) element of */
){
  memset(p, 0, sizeof(DbEvalContext));
  p->pDb = pDb;
  p->zSql = Tcl_GetString(pSql);
  p->pSql = pSql;
  Tcl_IncrRefCount(pSql);
  if( pArray ){
    p->pArray = pArray;
    Tcl_IncrRefCount(pArray);
  }
}

/*
** Obtain information about the row that the DbEvalContext passed as the
** first argument currently points to.
*/
static void dbEvalRowInfo(
  DbEvalContext *p,               /* Evaluation context */
  int *pnCol,                     /* OUT: Number of column names */
  Tcl_Obj ***papColName           /* OUT: Array of column names */
){
  /* Compute column names */
  if( 0==p->apColName ){
    sqlite3_stmt *pStmt = p->pPreStmt->pStmt;
    int i;                        /* Iterator variable */
    int nCol;                     /* Number of columns returned by pStmt */
    Tcl_Obj **apColName = 0;      /* Array of column names */

    p->nCol = nCol = sqlite3_column_count(pStmt);
    if( nCol>0 && (papColName || p->pArray) ){
      apColName = (Tcl_Obj**)Tcl_Alloc( sizeof(Tcl_Obj*)*nCol );
      for(i=0; i<nCol; i++){
        apColName[i] = Tcl_NewStringObj(sqlite3_column_name(pStmt,i), -1);
        Tcl_IncrRefCount(apColName[i]);
      }
      p->apColName = apColName;
    }

    /* If results are being stored in an array variable, then create
    ** the array(*) entry for that array
    */
    if( p->pArray ){
      Tcl_Interp *interp = p->pDb->interp;
      Tcl_Obj *pColList = Tcl_NewObj();
      Tcl_Obj *pStar = Tcl_NewStringObj("*", -1);

      for(i=0; i<nCol; i++){
        Tcl_ListObjAppendElement(interp, pColList, apColName[i]);
      }
      Tcl_IncrRefCount(pStar);
      Tcl_ObjSetVar2(interp, p->pArray, pStar, pColList, 0);
      Tcl_DecrRefCount(pStar);
    }
  }

  if( papColName ){
    *papColName = p->apColName;
  }
  if( pnCol ){
    *pnCol = p->nCol;
  }
}

/*
** Return one of TCL_OK, TCL_BREAK or TCL_ERROR. If TCL_ERROR is
** returned, then an error message is stored in the interpreter before
** returning.
**
** A return value of TCL_OK means there is a row of data available. The
** data may be accessed using dbEvalRowInfo() and dbEvalColumnValue(). This
** is analogous to a return of SQLITE_ROW from sqlite3_step(). If TCL_BREAK
** is returned, then the SQL script has finished executing and there are
** no further rows available. This is similar to SQLITE_DONE.
*/
static int dbEvalStep(DbEvalContext *p){
  const char *zPrevSql = 0;       /* Previous value of p->zSql */

  while( p->zSql[0] || p->pPreStmt ){
    int rc;
    if( p->pPreStmt==0 ){
      zPrevSql = (p->zSql==zPrevSql ? 0 : p->zSql);
      rc = dbPrepareAndBind(p->pDb, p->zSql, &p->zSql, &p->pPreStmt);
      if( rc!=TCL_OK ) return rc;
    }else{
      int rcs;
      SqliteDb *pDb = p->pDb;
      SqlPreparedStmt *pPreStmt = p->pPreStmt;
      sqlite3_stmt *pStmt = pPreStmt->pStmt;

      rcs = sqlite3_step(pStmt);
      if( rcs==SQLITE_ROW ){
        return TCL_OK;
      }
      if( p->pArray ){
        dbEvalRowInfo(p, 0, 0);
      }
      rcs = sqlite3_reset(pStmt);

      pDb->nStep = sqlite3_stmt_status(pStmt,SQLITE_STMTSTATUS_FULLSCAN_STEP,1);
      pDb->nSort = sqlite3_stmt_status(pStmt,SQLITE_STMTSTATUS_SORT,1);
      pDb->nIndex = sqlite3_stmt_status(pStmt,SQLITE_STMTSTATUS_AUTOINDEX,1);
      dbReleaseColumnNames(p);
      p->pPreStmt = 0;

      if( rcs!=SQLITE_OK ){
        /* If a run-time error occurs, report the error and stop reading
        ** the SQL.  */
        dbReleaseStmt(pDb, pPreStmt, 1);
#if SQLITE_TEST
        if( p->pDb->bLegacyPrepare && rcs==SQLITE_SCHEMA && zPrevSql ){
          /* If the runtime error was an SQLITE_SCHEMA, and the database
          ** handle is configured to use the legacy sqlite3_prepare() 
          ** interface, retry prepare()/step() on the same SQL statement.
          ** This only happens once. If there is a second SQLITE_SCHEMA
          ** error, the error will be returned to the caller. */
          p->zSql = zPrevSql;
          continue;
        }
#endif
        Tcl_SetObjResult(pDb->interp,
                         Tcl_NewStringObj(sqlite3_errmsg(pDb->db), -1));
        return TCL_ERROR;
      }else{
        dbReleaseStmt(pDb, pPreStmt, 0);
      }
    }
  }

  /* Finished */
  return TCL_BREAK;
}

/*
** Free all resources currently held by the DbEvalContext structure passed
** as the first argument. There should be exactly one call to this function
** for each call to dbEvalInit().
*/
static void dbEvalFinalize(DbEvalContext *p){
  if( p->pPreStmt ){
    sqlite3_reset(p->pPreStmt->pStmt);
    dbReleaseStmt(p->pDb, p->pPreStmt, 0);
    p->pPreStmt = 0;
  }
  if( p->pArray ){
    Tcl_DecrRefCount(p->pArray);
    p->pArray = 0;
  }
  Tcl_DecrRefCount(p->pSql);
  dbReleaseColumnNames(p);
}

/*
** Return a pointer to a Tcl_Obj structure with ref-count 0 that contains
** the value for the iCol'th column of the row currently pointed to by
** the DbEvalContext structure passed as the first argument.
*/
static Tcl_Obj *dbEvalColumnValue(DbEvalContext *p, int iCol){
  sqlite3_stmt *pStmt = p->pPreStmt->pStmt;
  switch( sqlite3_column_type(pStmt, iCol) ){
    case SQLITE_BLOB: {
      int bytes = sqlite3_column_bytes(pStmt, iCol);
      const char *zBlob = sqlite3_column_blob(pStmt, iCol);
      if( !zBlob ) bytes = 0;
      return Tcl_NewByteArrayObj((u8*)zBlob, bytes);
    }
    case SQLITE_INTEGER: {
      sqlite_int64 v = sqlite3_column_int64(pStmt, iCol);
      if( v>=-2147483647 && v<=2147483647 ){
        return Tcl_NewIntObj((int)v);
      }else{
        return Tcl_NewWideIntObj(v);
      }
    }
    case SQLITE_FLOAT: {
      return Tcl_NewDoubleObj(sqlite3_column_double(pStmt, iCol));
    }
    case SQLITE_NULL: {
      return Tcl_NewStringObj(p->pDb->zNull, -1);
    }
  }

  return Tcl_NewStringObj((char*)sqlite3_column_text(pStmt, iCol), -1);
}

/*
** If using Tcl version 8.6 or greater, use the NR functions to avoid
** recursive evalution of scripts by the [db eval] and [db trans]
** commands. Even if the headers used while compiling the extension
** are 8.6 or newer, the code still tests the Tcl version at runtime.
** This allows stubs-enabled builds to be used with older Tcl libraries.
*/
#if TCL_MAJOR_VERSION>8 || (TCL_MAJOR_VERSION==8 && TCL_MINOR_VERSION>=6)
# define SQLITE_TCL_NRE 1
static int DbUseNre(void){
  int major, minor;
  Tcl_GetVersion(&major, &minor, 0, 0);
  return( (major==8 && minor>=6) || major>8 );
}
#else
/* 
** Compiling using headers earlier than 8.6. In this case NR cannot be
** used, so DbUseNre() to always return zero. Add #defines for the other
** Tcl_NRxxx() functions to prevent them from causing compilation errors,
** even though the only invocations of them are within conditional blocks 
** of the form:
**
**   if( DbUseNre() ) { ... }
*/
# define SQLITE_TCL_NRE 0
# define DbUseNre() 0
# define Tcl_NRAddCallback(a,b,c,d,e,f) (void)0
# define Tcl_NREvalObj(a,b,c) 0
# define Tcl_NRCreateCommand(a,b,c,d,e,f) (void)0
#endif

/*
** This function is part of the implementation of the command:
**
**   $db eval SQL ?ARRAYNAME? SCRIPT
*/
static int DbEvalNextCmd(
  ClientData data[],                   /* data[0] is the (DbEvalContext*) */
  Tcl_Interp *interp,                  /* Tcl interpreter */
  int result                           /* Result so far */
){
  int rc = result;                     /* Return code */

  /* The first element of the data[] array is a pointer to a DbEvalContext
  ** structure allocated using Tcl_Alloc(). The second element of data[]
  ** is a pointer to a Tcl_Obj containing the script to run for each row
  ** returned by the queries encapsulated in data[0]. */
  DbEvalContext *p = (DbEvalContext *)data[0];
  Tcl_Obj *pScript = (Tcl_Obj *)data[1];
  Tcl_Obj *pArray = p->pArray;

  while( (rc==TCL_OK || rc==TCL_CONTINUE) && TCL_OK==(rc = dbEvalStep(p)) ){
    int i;
    int nCol;
    Tcl_Obj **apColName;
    dbEvalRowInfo(p, &nCol, &apColName);
    for(i=0; i<nCol; i++){
      Tcl_Obj *pVal = dbEvalColumnValue(p, i);
      if( pArray==0 ){
        Tcl_ObjSetVar2(interp, apColName[i], 0, pVal, 0);
      }else{
        Tcl_ObjSetVar2(interp, pArray, apColName[i], pVal, 0);
      }
    }

    /* The required interpreter variables are now populated with the data 
    ** from the current row. If using NRE, schedule callbacks to evaluate
    ** script pScript, then to invoke this function again to fetch the next
    ** row (or clean up if there is no next row or the script throws an
    ** exception). After scheduling the callbacks, return control to the 
    ** caller.
    **
    ** If not using NRE, evaluate pScript directly and continue with the
    ** next iteration of this while(...) loop.  */
    if( DbUseNre() ){
      Tcl_NRAddCallback(interp, DbEvalNextCmd, (void*)p, (void*)pScript, 0, 0);
      return Tcl_NREvalObj(interp, pScript, 0);
    }else{
      rc = Tcl_EvalObjEx(interp, pScript, 0);
    }
  }

  Tcl_DecrRefCount(pScript);
  dbEvalFinalize(p);
  Tcl_Free((char *)p);

  if( rc==TCL_OK || rc==TCL_BREAK ){
    Tcl_ResetResult(interp);
    rc = TCL_OK;
  }
  return rc;
}

/*
** The "sqlite" command below creates a new Tcl command for each
** connection it opens to an SQLite database.  This routine is invoked
** whenever one of those connection-specific commands is executed
** in Tcl.  For example, if you run Tcl code like this:
**
**       sqlite3 db1  "my_database"
**       db1 close
**
** The first command opens a connection to the "my_database" database
** and calls that connection "db1".  The second command causes this
** subroutine to be invoked.
*/
static int DbObjCmd(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
  SqliteDb *pDb = (SqliteDb*)cd;
  int choice;
  int rc = TCL_OK;
  static const char *DB_strs[] = {
    "authorizer",         "backup",            "busy",
    "cache",              "changes",           "close",
    "collate",            "collation_needed",  "commit_hook",
    "complete",           "copy",              "enable_load_extension",
    "errorcode",          "eval",              "exists",
    "function",           "incrblob",          "interrupt",
    "last_insert_rowid",  "nullvalue",         "onecolumn",
    "profile",            "progress",          "rekey",
    "restore",            "rollback_hook",     "status",
    "timeout",            "total_changes",     "trace",
    "transaction",        "unlock_notify",     "update_hook",
    "version",            "wal_hook",          0
  };
  enum DB_enum {
    DB_AUTHORIZER,        DB_BACKUP,           DB_BUSY,
    DB_CACHE,             DB_CHANGES,          DB_CLOSE,
    DB_COLLATE,           DB_COLLATION_NEEDED, DB_COMMIT_HOOK,
    DB_COMPLETE,          DB_COPY,             DB_ENABLE_LOAD_EXTENSION,
    DB_ERRORCODE,         DB_EVAL,             DB_EXISTS,
    DB_FUNCTION,          DB_INCRBLOB,         DB_INTERRUPT,
    DB_LAST_INSERT_ROWID, DB_NULLVALUE,        DB_ONECOLUMN,
    DB_PROFILE,           DB_PROGRESS,         DB_REKEY,
    DB_RESTORE,           DB_ROLLBACK_HOOK,    DB_STATUS,
    DB_TIMEOUT,           DB_TOTAL_CHANGES,    DB_TRACE,
    DB_TRANSACTION,       DB_UNLOCK_NOTIFY,    DB_UPDATE_HOOK,
    DB_VERSION,           DB_WAL_HOOK
  };
  /* don't leave trailing commas on DB_enum, it confuses the AIX xlc compiler */

  if( objc<2 ){
    Tcl_WrongNumArgs(interp, 1, objv, "SUBCOMMAND ...");
    return TCL_ERROR;
  }
  if( Tcl_GetIndexFromObj(interp, objv[1], DB_strs, "option", 0, &choice) ){
    return TCL_ERROR;
  }

  switch( (enum DB_enum)choice ){

  /*    $db authorizer ?CALLBACK?
  **
  ** Invoke the given callback to authorize each SQL operation as it is
  ** compiled.  5 arguments are appended to the callback before it is
  ** invoked:
  **
  **   (1) The authorization type (ex: SQLITE_CREATE_TABLE, SQLITE_INSERT, ...)
  **   (2) First descriptive name (depends on authorization type)
  **   (3) Second descriptive name
  **   (4) Name of the database (ex: "main", "temp")
  **   (5) Name of trigger that is doing the access
  **
  ** The callback should return on of the following strings: SQLITE_OK,
  ** SQLITE_IGNORE, or SQLITE_DENY.  Any other return value is an error.
  **
  ** If this method is invoked with no arguments, the current authorization
  ** callback string is returned.
  */
  case DB_AUTHORIZER: {
#ifdef SQLITE_OMIT_AUTHORIZATION
    Tcl_AppendResult(interp, "authorization not available in this build",
                     (char*)0);
    return TCL_ERROR;
#else
    if( objc>3 ){
      Tcl_WrongNumArgs(interp, 2, objv, "?CALLBACK?");
      return TCL_ERROR;
    }else if( objc==2 ){
      if( pDb->zAuth ){
        Tcl_AppendResult(interp, pDb->zAuth, (char*)0);
      }
    }else{
      char *zAuth;
      int len;
      if( pDb->zAuth ){
        Tcl_Free(pDb->zAuth);
      }
      zAuth = Tcl_GetStringFromObj(objv[2], &len);
      if( zAuth && len>0 ){
        pDb->zAuth = Tcl_Alloc( len + 1 );
        memcpy(pDb->zAuth, zAuth, len+1);
      }else{
        pDb->zAuth = 0;
      }
      if( pDb->zAuth ){
        pDb->interp = interp;
        sqlite3_set_authorizer(pDb->db, auth_callback, pDb);
      }else{
        sqlite3_set_authorizer(pDb->db, 0, 0);
      }
    }
#endif
    break;
  }

  /*    $db backup ?DATABASE? FILENAME
  **
  ** Open or create a database file named FILENAME.  Transfer the
  ** content of local database DATABASE (default: "main") into the
  ** FILENAME database.
  */
  case DB_BACKUP: {
    const char *zDestFile;
    const char *zSrcDb;
    sqlite3 *pDest;
    sqlite3_backup *pBackup;

    if( objc==3 ){
      zSrcDb = "main";
      zDestFile = Tcl_GetString(objv[2]);
    }else if( objc==4 ){
      zSrcDb = Tcl_GetString(objv[2]);
      zDestFile = Tcl_GetString(objv[3]);
    }else{
      Tcl_WrongNumArgs(interp, 2, objv, "?DATABASE? FILENAME");
      return TCL_ERROR;
    }
    rc = sqlite3_open(zDestFile, &pDest);
    if( rc!=SQLITE_OK ){
      Tcl_AppendResult(interp, "cannot open target database: ",
           sqlite3_errmsg(pDest), (char*)0);
      sqlite3_close(pDest);
      return TCL_ERROR;
    }
    pBackup = sqlite3_backup_init(pDest, "main", pDb->db, zSrcDb);
    if( pBackup==0 ){
      Tcl_AppendResult(interp, "backup failed: ",
           sqlite3_errmsg(pDest), (char*)0);
      sqlite3_close(pDest);
      return TCL_ERROR;
    }
    while(  (rc = sqlite3_backup_step(pBackup,100))==SQLITE_OK ){}
    sqlite3_backup_finish(pBackup);
    if( rc==SQLITE_DONE ){
      rc = TCL_OK;
    }else{
      Tcl_AppendResult(interp, "backup failed: ",
           sqlite3_errmsg(pDest), (char*)0);
      rc = TCL_ERROR;
    }
    sqlite3_close(pDest);
    break;
  }

  /*    $db busy ?CALLBACK?
  **
  ** Invoke the given callback if an SQL statement attempts to open
  ** a locked database file.
  */
  case DB_BUSY: {
    if( objc>3 ){
      Tcl_WrongNumArgs(interp, 2, objv, "CALLBACK");
      return TCL_ERROR;
    }else if( objc==2 ){
      if( pDb->zBusy ){
        Tcl_AppendResult(interp, pDb->zBusy, (char*)0);
      }
    }else{
      char *zBusy;
      int len;
      if( pDb->zBusy ){
        Tcl_Free(pDb->zBusy);
      }
      zBusy = Tcl_GetStringFromObj(objv[2], &len);
      if( zBusy && len>0 ){
        pDb->zBusy = Tcl_Alloc( len + 1 );
        memcpy(pDb->zBusy, zBusy, len+1);
      }else{
        pDb->zBusy = 0;
      }
      if( pDb->zBusy ){
        pDb->interp = interp;
        sqlite3_busy_handler(pDb->db, DbBusyHandler, pDb);
      }else{
        sqlite3_busy_handler(pDb->db, 0, 0);
      }
    }
    break;
  }

  /*     $db cache flush
  **     $db cache size n
  **
  ** Flush the prepared statement cache, or set the maximum number of
  ** cached statements.
  */
  case DB_CACHE: {
    char *subCmd;
    int n;

    if( objc<=2 ){
      Tcl_WrongNumArgs(interp, 1, objv, "cache option ?arg?");
      return TCL_ERROR;
    }
    subCmd = Tcl_GetStringFromObj( objv[2], 0 );
    if( *subCmd=='f' && strcmp(subCmd,"flush")==0 ){
      if( objc!=3 ){
        Tcl_WrongNumArgs(interp, 2, objv, "flush");
        return TCL_ERROR;
      }else{
        flushStmtCache( pDb );
      }
    }else if( *subCmd=='s' && strcmp(subCmd,"size")==0 ){
      if( objc!=4 ){
        Tcl_WrongNumArgs(interp, 2, objv, "size n");
        return TCL_ERROR;
      }else{
        if( TCL_ERROR==Tcl_GetIntFromObj(interp, objv[3], &n) ){
          Tcl_AppendResult( interp, "cannot convert \"", 
               Tcl_GetStringFromObj(objv[3],0), "\" to integer", (char*)0);
          return TCL_ERROR;
        }else{
          if( n<0 ){
            flushStmtCache( pDb );
            n = 0;
          }else if( n>MAX_PREPARED_STMTS ){
            n = MAX_PREPARED_STMTS;
          }
          pDb->maxStmt = n;
        }
      }
    }else{
      Tcl_AppendResult( interp, "bad option \"", 
          Tcl_GetStringFromObj(objv[2],0), "\": must be flush or size",
          (char*)0);
      return TCL_ERROR;
    }
    break;
  }

  /*     $db changes
  **
  ** Return the number of rows that were modified, inserted, or deleted by
  ** the most recent INSERT, UPDATE or DELETE statement, not including 
  ** any changes made by trigger programs.
  */
  case DB_CHANGES: {
    Tcl_Obj *pResult;
    if( objc!=2 ){
      Tcl_WrongNumArgs(interp, 2, objv, "");
      return TCL_ERROR;
    }
    pResult = Tcl_GetObjResult(interp);
    Tcl_SetIntObj(pResult, sqlite3_changes(pDb->db));
    break;
  }

  /*    $db close
  **
  ** Shutdown the database
  */
  case DB_CLOSE: {
    Tcl_DeleteCommand(interp, Tcl_GetStringFromObj(objv[0], 0));
    break;
  }

  /*
  **     $db collate NAME SCRIPT
  **
  ** Create a new SQL collation function called NAME.  Whenever
  ** that function is called, invoke SCRIPT to evaluate the function.
  */
  case DB_COLLATE: {
    SqlCollate *pCollate;
    char *zName;
    char *zScript;
    int nScript;
    if( objc!=4 ){
      Tcl_WrongNumArgs(interp, 2, objv, "NAME SCRIPT");
      return TCL_ERROR;
    }
    zName = Tcl_GetStringFromObj(objv[2], 0);
    zScript = Tcl_GetStringFromObj(objv[3], &nScript);
    pCollate = (SqlCollate*)Tcl_Alloc( sizeof(*pCollate) + nScript + 1 );
    if( pCollate==0 ) return TCL_ERROR;
    pCollate->interp = interp;
    pCollate->pNext = pDb->pCollate;
    pCollate->zScript = (char*)&pCollate[1];
    pDb->pCollate = pCollate;
    memcpy(pCollate->zScript, zScript, nScript+1);
    if( sqlite3_create_collation(pDb->db, zName, SQLITE_UTF8, 
        pCollate, tclSqlCollate) ){
      Tcl_SetResult(interp, (char *)sqlite3_errmsg(pDb->db), TCL_VOLATILE);
      return TCL_ERROR;
    }
    break;
  }

  /*
  **     $db collation_needed SCRIPT
  **
  ** Create a new SQL collation function called NAME.  Whenever
  ** that function is called, invoke SCRIPT to evaluate the function.
  */
  case DB_COLLATION_NEEDED: {
    if( objc!=3 ){
      Tcl_WrongNumArgs(interp, 2, objv, "SCRIPT");
      return TCL_ERROR;
    }
    if( pDb->pCollateNeeded ){
      Tcl_DecrRefCount(pDb->pCollateNeeded);
    }
    pDb->pCollateNeeded = Tcl_DuplicateObj(objv[2]);
    Tcl_IncrRefCount(pDb->pCollateNeeded);
    sqlite3_collation_needed(pDb->db, pDb, tclCollateNeeded);
    break;
  }

  /*    $db commit_hook ?CALLBACK?
  **
  ** Invoke the given callback just before committing every SQL transaction.
  ** If the callback throws an exception or returns non-zero, then the
  ** transaction is aborted.  If CALLBACK is an empty string, the callback
  ** is disabled.
  */
  case DB_COMMIT_HOOK: {
    if( objc>3 ){
      Tcl_WrongNumArgs(interp, 2, objv, "?CALLBACK?");
      return TCL_ERROR;
    }else if( objc==2 ){
      if( pDb->zCommit ){
        Tcl_AppendResult(interp, pDb->zCommit, (char*)0);
      }
    }else{
      const char *zCommit;
      int len;
      if( pDb->zCommit ){
        Tcl_Free(pDb->zCommit);
      }
      zCommit = Tcl_GetStringFromObj(objv[2], &len);
      if( zCommit && len>0 ){
        pDb->zCommit = Tcl_Alloc( len + 1 );
        memcpy(pDb->zCommit, zCommit, len+1);
      }else{
        pDb->zCommit = 0;
      }
      if( pDb->zCommit ){
        pDb->interp = interp;
        sqlite3_commit_hook(pDb->db, DbCommitHandler, pDb);
      }else{
        sqlite3_commit_hook(pDb->db, 0, 0);
      }
    }
    break;
  }

  /*    $db complete SQL
  **
  ** Return TRUE if SQL is a complete SQL statement.  Return FALSE if
  ** additional lines of input are needed.  This is similar to the
  ** built-in "info complete" command of Tcl.
  */
  case DB_COMPLETE: {
#ifndef SQLITE_OMIT_COMPLETE
    Tcl_Obj *pResult;
    int isComplete;
    if( objc!=3 ){
      Tcl_WrongNumArgs(interp, 2, objv, "SQL");
      return TCL_ERROR;
    }
    isComplete = sqlite3_complete( Tcl_GetStringFromObj(objv[2], 0) );
    pResult = Tcl_GetObjResult(interp);
    Tcl_SetBooleanObj(pResult, isComplete);
#endif
    break;
  }

  /*    $db copy conflict-algorithm table filename ?SEPARATOR? ?NULLINDICATOR?
  **
  ** Copy data into table from filename, optionally using SEPARATOR
  ** as column separators.  If a column contains a null string, or the
  ** value of NULLINDICATOR, a NULL is inserted for the column.
  ** conflict-algorithm is one of the sqlite conflict algorithms:
  **    rollback, abort, fail, ignore, replace
  ** On success, return the number of lines processed, not necessarily same
  ** as 'db changes' due to conflict-algorithm selected.
  **
  ** This code is basically an implementation/enhancement of
  ** the sqlite3 shell.c ".import" command.
  **
  ** This command usage is equivalent to the sqlite2.x COPY statement,
  ** which imports file data into a table using the PostgreSQL COPY file format:
  **   $db copy $conflit_algo $table_name $filename \t \\N
  */
  case DB_COPY: {
    char *zTable;               /* Insert data into this table */
    char *zFile;                /* The file from which to extract data */
    char *zConflict;            /* The conflict algorithm to use */
    sqlite3_stmt *pStmt;        /* A statement */
    int nCol;                   /* Number of columns in the table */
    int nByte;                  /* Number of bytes in an SQL string */
    int i, j;                   /* Loop counters */
    int nSep;                   /* Number of bytes in zSep[] */
    int nNull;                  /* Number of bytes in zNull[] */
    char *zSql;                 /* An SQL statement */
    char *zLine;                /* A single line of input from the file */
    char **azCol;               /* zLine[] broken up into columns */
    const char *zCommit;        /* How to commit changes */
    FILE *in;                   /* The input file */
    int lineno = 0;             /* Line number of input file */
    char zLineNum[80];          /* Line number print buffer */
    Tcl_Obj *pResult;           /* interp result */

    const char *zSep;
    const char *zNull;
    if( objc<5 || objc>7 ){
      Tcl_WrongNumArgs(interp, 2, objv, 
         "CONFLICT-ALGORITHM TABLE FILENAME ?SEPARATOR? ?NULLINDICATOR?");
      return TCL_ERROR;
    }
    if( objc>=6 ){
      zSep = Tcl_GetStringFromObj(objv[5], 0);
    }else{
      zSep = "\t";
    }
    if( objc>=7 ){
      zNull = Tcl_GetStringFromObj(objv[6], 0);
    }else{
      zNull = "";
    }
    zConflict = Tcl_GetStringFromObj(objv[2], 0);
    zTable = Tcl_GetStringFromObj(objv[3], 0);
    zFile = Tcl_GetStringFromObj(objv[4], 0);
    nSep = strlen30(zSep);
    nNull = strlen30(zNull);
    if( nSep==0 ){
      Tcl_AppendResult(interp,"Error: non-null separator required for copy",
                       (char*)0);
      return TCL_ERROR;
    }
    if(strcmp(zConflict, "rollback") != 0 &&
       strcmp(zConflict, "abort"   ) != 0 &&
       strcmp(zConflict, "fail"    ) != 0 &&
       strcmp(zConflict, "ignore"  ) != 0 &&
       strcmp(zConflict, "replace" ) != 0 ) {
      Tcl_AppendResult(interp, "Error: \"", zConflict, 
            "\", conflict-algorithm must be one of: rollback, "
            "abort, fail, ignore, or replace", (char*)0);
      return TCL_ERROR;
    }
    zSql = sqlite3_mprintf("SELECT * FROM '%q'", zTable);
    if( zSql==0 ){
      Tcl_AppendResult(interp, "Error: no such table: ", zTable, (char*)0);
      return TCL_ERROR;
    }
    nByte = strlen30(zSql);
    rc = sqlite3_prepare(pDb->db, zSql, -1, &pStmt, 0);
    sqlite3_free(zSql);
    if( rc ){
      Tcl_AppendResult(interp, "Error: ", sqlite3_errmsg(pDb->db), (char*)0);
      nCol = 0;
    }else{
      nCol = sqlite3_column_count(pStmt);
    }
    sqlite3_finalize(pStmt);
    if( nCol==0 ) {
      return TCL_ERROR;
    }
    zSql = malloc( nByte + 50 + nCol*2 );
    if( zSql==0 ) {
      Tcl_AppendResult(interp, "Error: can't malloc()", (char*)0);
      return TCL_ERROR;
    }
    sqlite3_snprintf(nByte+50, zSql, "INSERT OR %q INTO '%q' VALUES(?",
         zConflict, zTable);
    j = strlen30(zSql);
    for(i=1; i<nCol; i++){
      zSql[j++] = ',';
      zSql[j++] = '?';
    }
    zSql[j++] = ')';
    zSql[j] = 0;
    rc = sqlite3_prepare(pDb->db, zSql, -1, &pStmt, 0);
    free(zSql);
    if( rc ){
      Tcl_AppendResult(interp, "Error: ", sqlite3_errmsg(pDb->db), (char*)0);
      sqlite3_finalize(pStmt);
      return TCL_ERROR;
    }
    in = fopen(zFile, "rb");
    if( in==0 ){
      Tcl_AppendResult(interp, "Error: cannot open file: ", zFile, NULL);
      sqlite3_finalize(pStmt);
      return TCL_ERROR;
    }
    azCol = malloc( sizeof(azCol[0])*(nCol+1) );
    if( azCol==0 ) {
      Tcl_AppendResult(interp, "Error: can't malloc()", (char*)0);
      fclose(in);
      return TCL_ERROR;
    }
    (void)sqlite3_exec(pDb->db, "BEGIN", 0, 0, 0);
    zCommit = "COMMIT";
    while( (zLine = local_getline(0, in))!=0 ){
      char *z;
      lineno++;
      azCol[0] = zLine;
      for(i=0, z=zLine; *z; z++){
        if( *z==zSep[0] && strncmp(z, zSep, nSep)==0 ){
          *z = 0;
          i++;
          if( i<nCol ){
            azCol[i] = &z[nSep];
            z += nSep-1;
          }
        }
      }
      if( i+1!=nCol ){
        char *zErr;
        int nErr = strlen30(zFile) + 200;
        zErr = malloc(nErr);
        if( zErr ){
          sqlite3_snprintf(nErr, zErr,
             "Error: %s line %d: expected %d columns of data but found %d",
             zFile, lineno, nCol, i+1);
          Tcl_AppendResult(interp, zErr, (char*)0);
          free(zErr);
        }
        zCommit = "ROLLBACK";
        break;
      }
      for(i=0; i<nCol; i++){
        /* check for null data, if so, bind as null */
        if( (nNull>0 && strcmp(azCol[i], zNull)==0)
          || strlen30(azCol[i])==0 
        ){
          sqlite3_bind_null(pStmt, i+1);
        }else{
          sqlite3_bind_text(pStmt, i+1, azCol[i], -1, SQLITE_STATIC);
        }
      }
      sqlite3_step(pStmt);
      rc = sqlite3_reset(pStmt);
      free(zLine);
      if( rc!=SQLITE_OK ){
        Tcl_AppendResult(interp,"Error: ", sqlite3_errmsg(pDb->db), (char*)0);
        zCommit = "ROLLBACK";
        break;
      }
    }
    free(azCol);
    fclose(in);
    sqlite3_finalize(pStmt);
    (void)sqlite3_exec(pDb->db, zCommit, 0, 0, 0);

    if( zCommit[0] == 'C' ){
      /* success, set result as number of lines processed */
      pResult = Tcl_GetObjResult(interp);
      Tcl_SetIntObj(pResult, lineno);
      rc = TCL_OK;
    }else{
      /* failure, append lineno where failed */
      sqlite3_snprintf(sizeof(zLineNum), zLineNum,"%d",lineno);
      Tcl_AppendResult(interp,", failed while processing line: ",zLineNum,
                       (char*)0);
      rc = TCL_ERROR;
    }
    break;
  }

  /*
  **    $db enable_load_extension BOOLEAN
  **
  ** Turn the extension loading feature on or off.  It if off by
  ** default.
  */
  case DB_ENABLE_LOAD_EXTENSION: {
#ifndef SQLITE_OMIT_LOAD_EXTENSION
    int onoff;
    if( objc!=3 ){
      Tcl_WrongNumArgs(interp, 2, objv, "BOOLEAN");
      return TCL_ERROR;
    }
    if( Tcl_GetBooleanFromObj(interp, objv[2], &onoff) ){
      return TCL_ERROR;
    }
    sqlite3_enable_load_extension(pDb->db, onoff);
    break;
#else
    Tcl_AppendResult(interp, "extension loading is turned off at compile-time",
                     (char*)0);
    return TCL_ERROR;
#endif
  }

  /*
  **    $db errorcode
  **
  ** Return the numeric error code that was returned by the most recent
  ** call to sqlite3_exec().
  */
  case DB_ERRORCODE: {
    Tcl_SetObjResult(interp, Tcl_NewIntObj(sqlite3_errcode(pDb->db)));
    break;
  }

  /*
  **    $db exists $sql
  **    $db onecolumn $sql
  **
  ** The onecolumn method is the equivalent of:
  **     lindex [$db eval $sql] 0
  */
  case DB_EXISTS: 
  case DB_ONECOLUMN: {
    DbEvalContext sEval;
    if( objc!=3 ){
      Tcl_WrongNumArgs(interp, 2, objv, "SQL");
      return TCL_ERROR;
    }

    dbEvalInit(&sEval, pDb, objv[2], 0);
    rc = dbEvalStep(&sEval);
    if( choice==DB_ONECOLUMN ){
      if( rc==TCL_OK ){
        Tcl_SetObjResult(interp, dbEvalColumnValue(&sEval, 0));
      }else if( rc==TCL_BREAK ){
        Tcl_ResetResult(interp);
      }
    }else if( rc==TCL_BREAK || rc==TCL_OK ){
      Tcl_SetObjResult(interp, Tcl_NewBooleanObj(rc==TCL_OK));
    }
    dbEvalFinalize(&sEval);

    if( rc==TCL_BREAK ){
      rc = TCL_OK;
    }
    break;
  }
   
  /*
  **    $db eval $sql ?array? ?{  ...code... }?
  **
  ** The SQL statement in $sql is evaluated.  For each row, the values are
  ** placed in elements of the array named "array" and ...code... is executed.
  ** If "array" and "code" are omitted, then no callback is every invoked.
  ** If "array" is an empty string, then the values are placed in variables
  ** that have the same name as the fields extracted by the query.
  */
  case DB_EVAL: {
    if( objc<3 || objc>5 ){
      Tcl_WrongNumArgs(interp, 2, objv, "SQL ?ARRAY-NAME? ?SCRIPT?");
      return TCL_ERROR;
    }

    if( objc==3 ){
      DbEvalContext sEval;
      Tcl_Obj *pRet = Tcl_NewObj();
      Tcl_IncrRefCount(pRet);
      dbEvalInit(&sEval, pDb, objv[2], 0);
      while( TCL_OK==(rc = dbEvalStep(&sEval)) ){
        int i;
        int nCol;
        dbEvalRowInfo(&sEval, &nCol, 0);
        for(i=0; i<nCol; i++){
          Tcl_ListObjAppendElement(interp, pRet, dbEvalColumnValue(&sEval, i));
        }
      }
      dbEvalFinalize(&sEval);
      if( rc==TCL_BREAK ){
        Tcl_SetObjResult(interp, pRet);
        rc = TCL_OK;
      }
      Tcl_DecrRefCount(pRet);
    }else{
      ClientData cd[2];
      DbEvalContext *p;
      Tcl_Obj *pArray = 0;
      Tcl_Obj *pScript;

      if( objc==5 && *(char *)Tcl_GetString(objv[3]) ){
        pArray = objv[3];
      }
      pScript = objv[objc-1];
      Tcl_IncrRefCount(pScript);
      
      p = (DbEvalContext *)Tcl_Alloc(sizeof(DbEvalContext));
      dbEvalInit(p, pDb, objv[2], pArray);

      cd[0] = (void *)p;
      cd[1] = (void *)pScript;
      rc = DbEvalNextCmd(cd, interp, TCL_OK);
    }
    break;
  }

  /*
  **     $db function NAME [-argcount N] SCRIPT
  **
  ** Create a new SQL function called NAME.  Whenever that function is
  ** called, invoke SCRIPT to evaluate the function.
  */
  case DB_FUNCTION: {
    SqlFunc *pFunc;
    Tcl_Obj *pScript;
    char *zName;
    int nArg = -1;
    if( objc==6 ){
      const char *z = Tcl_GetString(objv[3]);
      int n = strlen30(z);
      if( n>2 && strncmp(z, "-argcount",n)==0 ){
        if( Tcl_GetIntFromObj(interp, objv[4], &nArg) ) return TCL_ERROR;
        if( nArg<0 ){
          Tcl_AppendResult(interp, "number of arguments must be non-negative",
                           (char*)0);
          return TCL_ERROR;
        }
      }
      pScript = objv[5];
    }else if( objc!=4 ){
      Tcl_WrongNumArgs(interp, 2, objv, "NAME [-argcount N] SCRIPT");
      return TCL_ERROR;
    }else{
      pScript = objv[3];
    }
    zName = Tcl_GetStringFromObj(objv[2], 0);
    pFunc = findSqlFunc(pDb, zName);
    if( pFunc==0 ) return TCL_ERROR;
    if( pFunc->pScript ){
      Tcl_DecrRefCount(pFunc->pScript);
    }
    pFunc->pScript = pScript;
    Tcl_IncrRefCount(pScript);
    pFunc->useEvalObjv = safeToUseEvalObjv(interp, pScript);
    rc = sqlite3_create_function(pDb->db, zName, nArg, SQLITE_UTF8,
        pFunc, tclSqlFunc, 0, 0);
    if( rc!=SQLITE_OK ){
      rc = TCL_ERROR;
      Tcl_SetResult(interp, (char *)sqlite3_errmsg(pDb->db), TCL_VOLATILE);
    }
    break;
  }

  /*
  **     $db incrblob ?-readonly? ?DB? TABLE COLUMN ROWID
  */
  case DB_INCRBLOB: {
#ifdef SQLITE_OMIT_INCRBLOB
    Tcl_AppendResult(interp, "incrblob not available in this build", (char*)0);
    return TCL_ERROR;
#else
    int isReadonly = 0;
    const char *zDb = "main";
    const char *zTable;
    const char *zColumn;
    Tcl_WideInt iRow;

    /* Check for the -readonly option */
    if( objc>3 && strcmp(Tcl_GetString(objv[2]), "-readonly")==0 ){
      isReadonly = 1;
    }

    if( objc!=(5+isReadonly) && objc!=(6+isReadonly) ){
      Tcl_WrongNumArgs(interp, 2, objv, "?-readonly? ?DB? TABLE COLUMN ROWID");
      return TCL_ERROR;
    }

    if( objc==(6+isReadonly) ){
      zDb = Tcl_GetString(objv[2]);
    }
    zTable = Tcl_GetString(objv[objc-3]);
    zColumn = Tcl_GetString(objv[objc-2]);
    rc = Tcl_GetWideIntFromObj(interp, objv[objc-1], &iRow);

    if( rc==TCL_OK ){
      rc = createIncrblobChannel(
          interp, pDb, zDb, zTable, zColumn, iRow, isReadonly
      );
    }
#endif
    break;
  }

  /*
  **     $db interrupt
  **
  ** Interrupt the execution of the inner-most SQL interpreter.  This
  ** causes the SQL statement to return an error of SQLITE_INTERRUPT.
  */
  case DB_INTERRUPT: {
    sqlite3_interrupt(pDb->db);
    break;
  }

  /*
  **     $db nullvalue ?STRING?
  **
  ** Change text used when a NULL comes back from the database. If ?STRING?
  ** is not present, then the current string used for NULL is returned.
  ** If STRING is present, then STRING is returned.
  **
  */
  case DB_NULLVALUE: {
    if( objc!=2 && objc!=3 ){
      Tcl_WrongNumArgs(interp, 2, objv, "NULLVALUE");
      return TCL_ERROR;
    }
    if( objc==3 ){
      int len;
      char *zNull = Tcl_GetStringFromObj(objv[2], &len);
      if( pDb->zNull ){
        Tcl_Free(pDb->zNull);
      }
      if( zNull && len>0 ){
        pDb->zNull = Tcl_Alloc( len + 1 );
        memcpy(pDb->zNull, zNull, len);
        pDb->zNull[len] = '\0';
      }else{
        pDb->zNull = 0;
      }
    }
    Tcl_SetObjResult(interp, Tcl_NewStringObj(pDb->zNull, -1));
    break;
  }

  /*
  **     $db last_insert_rowid 
  **
  ** Return an integer which is the ROWID for the most recent insert.
  */
  case DB_LAST_INSERT_ROWID: {
    Tcl_Obj *pResult;
    Tcl_WideInt rowid;
    if( objc!=2 ){
      Tcl_WrongNumArgs(interp, 2, objv, "");
      return TCL_ERROR;
    }
    rowid = sqlite3_last_insert_rowid(pDb->db);
    pResult = Tcl_GetObjResult(interp);
    Tcl_SetWideIntObj(pResult, rowid);
    break;
  }

  /*
  ** The DB_ONECOLUMN method is implemented together with DB_EXISTS.
  */

  /*    $db progress ?N CALLBACK?
  ** 
  ** Invoke the given callback every N virtual machine opcodes while executing
  ** queries.
  */
  case DB_PROGRESS: {
    if( objc==2 ){
      if( pDb->zProgress ){
        Tcl_AppendResult(interp, pDb->zProgress, (char*)0);
      }
    }else if( objc==4 ){
      char *zProgress;
      int len;
      int N;
      if( TCL_OK!=Tcl_GetIntFromObj(interp, objv[2], &N) ){
        return TCL_ERROR;
      };
      if( pDb->zProgress ){
        Tcl_Free(pDb->zProgress);
      }
      zProgress = Tcl_GetStringFromObj(objv[3], &len);
      if( zProgress && len>0 ){
        pDb->zProgress = Tcl_Alloc( len + 1 );
        memcpy(pDb->zProgress, zProgress, len+1);
      }else{
        pDb->zProgress = 0;
      }
#ifndef SQLITE_OMIT_PROGRESS_CALLBACK
      if( pDb->zProgress ){
        pDb->interp = interp;
        sqlite3_progress_handler(pDb->db, N, DbProgressHandler, pDb);
      }else{
        sqlite3_progress_handler(pDb->db, 0, 0, 0);
      }
#endif
    }else{
      Tcl_WrongNumArgs(interp, 2, objv, "N CALLBACK");
      return TCL_ERROR;
    }
    break;
  }

  /*    $db profile ?CALLBACK?
  **
  ** Make arrangements to invoke the CALLBACK routine after each SQL statement
  ** that has run.  The text of the SQL and the amount of elapse time are
  ** appended to CALLBACK before the script is run.
  */
  case DB_PROFILE: {
    if( objc>3 ){
      Tcl_WrongNumArgs(interp, 2, objv, "?CALLBACK?");
      return TCL_ERROR;
    }else if( objc==2 ){
      if( pDb->zProfile ){
        Tcl_AppendResult(interp, pDb->zProfile, (char*)0);
      }
    }else{
      char *zProfile;
      int len;
      if( pDb->zProfile ){
        Tcl_Free(pDb->zProfile);
      }
      zProfile = Tcl_GetStringFromObj(objv[2], &len);
      if( zProfile && len>0 ){
        pDb->zProfile = Tcl_Alloc( len + 1 );
        memcpy(pDb->zProfile, zProfile, len+1);
      }else{
        pDb->zProfile = 0;
      }
#if !defined(SQLITE_OMIT_TRACE) && !defined(SQLITE_OMIT_FLOATING_POINT)
      if( pDb->zProfile ){
        pDb->interp = interp;
        sqlite3_profile(pDb->db, DbProfileHandler, pDb);
      }else{
        sqlite3_profile(pDb->db, 0, 0);
      }
#endif
    }
    break;
  }

  /*
  **     $db rekey KEY
  **
  ** Change the encryption key on the currently open database.
  */
  case DB_REKEY: {
#ifdef SQLITE_HAS_CODEC
    int nKey;
    void *pKey;
#endif
    if( objc!=3 ){
      Tcl_WrongNumArgs(interp, 2, objv, "KEY");
      return TCL_ERROR;
    }
#ifdef SQLITE_HAS_CODEC
    pKey = Tcl_GetByteArrayFromObj(objv[2], &nKey);
    rc = sqlite3_rekey(pDb->db, pKey, nKey);
    if( rc ){
      Tcl_AppendResult(interp, sqlite3_errstr(rc), (char*)0);
      rc = TCL_ERROR;
    }
#endif
    break;
  }

  /*    $db restore ?DATABASE? FILENAME
  **
  ** Open a database file named FILENAME.  Transfer the content 
  ** of FILENAME into the local database DATABASE (default: "main").
  */
  case DB_RESTORE: {
    const char *zSrcFile;
    const char *zDestDb;
    sqlite3 *pSrc;
    sqlite3_backup *pBackup;
    int nTimeout = 0;

    if( objc==3 ){
      zDestDb = "main";
      zSrcFile = Tcl_GetString(objv[2]);
    }else if( objc==4 ){
      zDestDb = Tcl_GetString(objv[2]);
      zSrcFile = Tcl_GetString(objv[3]);
    }else{
      Tcl_WrongNumArgs(interp, 2, objv, "?DATABASE? FILENAME");
      return TCL_ERROR;
    }
    rc = sqlite3_open_v2(zSrcFile, &pSrc, SQLITE_OPEN_READONLY, 0);
    if( rc!=SQLITE_OK ){
      Tcl_AppendResult(interp, "cannot open source database: ",
           sqlite3_errmsg(pSrc), (char*)0);
      sqlite3_close(pSrc);
      return TCL_ERROR;
    }
    pBackup = sqlite3_backup_init(pDb->db, zDestDb, pSrc, "main");
    if( pBackup==0 ){
      Tcl_AppendResult(interp, "restore failed: ",
           sqlite3_errmsg(pDb->db), (char*)0);
      sqlite3_close(pSrc);
      return TCL_ERROR;
    }
    while( (rc = sqlite3_backup_step(pBackup,100))==SQLITE_OK
              || rc==SQLITE_BUSY ){
      if( rc==SQLITE_BUSY ){
        if( nTimeout++ >= 3 ) break;
        sqlite3_sleep(100);
      }
    }
    sqlite3_backup_finish(pBackup);
    if( rc==SQLITE_DONE ){
      rc = TCL_OK;
    }else if( rc==SQLITE_BUSY || rc==SQLITE_LOCKED ){
      Tcl_AppendResult(interp, "restore failed: source database busy",
                       (char*)0);
      rc = TCL_ERROR;
    }else{
      Tcl_AppendResult(interp, "restore failed: ",
           sqlite3_errmsg(pDb->db), (char*)0);
      rc = TCL_ERROR;
    }
    sqlite3_close(pSrc);
    break;
  }

  /*
  **     $db status (step|sort|autoindex)
  **
  ** Display SQLITE_STMTSTATUS_FULLSCAN_STEP or 
  ** SQLITE_STMTSTATUS_SORT for the most recent eval.
  */
  case DB_STATUS: {
    int v;
    const char *zOp;
    if( objc!=3 ){
      Tcl_WrongNumArgs(interp, 2, objv, "(step|sort|autoindex)");
      return TCL_ERROR;
    }
    zOp = Tcl_GetString(objv[2]);
    if( strcmp(zOp, "step")==0 ){
      v = pDb->nStep;
    }else if( strcmp(zOp, "sort")==0 ){
      v = pDb->nSort;
    }else if( strcmp(zOp, "autoindex")==0 ){
      v = pDb->nIndex;
    }else{
      Tcl_AppendResult(interp, 
            "bad argument: should be autoindex, step, or sort", 
            (char*)0);
      return TCL_ERROR;
    }
    Tcl_SetObjResult(interp, Tcl_NewIntObj(v));
    break;
  }
  
  /*
  **     $db timeout MILLESECONDS
  **
  ** Delay for the number of milliseconds specified when a file is locked.
  */
  case DB_TIMEOUT: {
    int ms;
    if( objc!=3 ){
      Tcl_WrongNumArgs(interp, 2, objv, "MILLISECONDS");
      return TCL_ERROR;
    }
    if( Tcl_GetIntFromObj(interp, objv[2], &ms) ) return TCL_ERROR;
    sqlite3_busy_timeout(pDb->db, ms);
    break;
  }
  
  /*
  **     $db total_changes
  **
  ** Return the number of rows that were modified, inserted, or deleted 
  ** since the database handle was created.
  */
  case DB_TOTAL_CHANGES: {
    Tcl_Obj *pResult;
    if( objc!=2 ){
      Tcl_WrongNumArgs(interp, 2, objv, "");
      return TCL_ERROR;
    }
    pResult = Tcl_GetObjResult(interp);
    Tcl_SetIntObj(pResult, sqlite3_total_changes(pDb->db));
    break;
  }

  /*    $db trace ?CALLBACK?
  **
  ** Make arrangements to invoke the CALLBACK routine for each SQL statement
  ** that is executed.  The text of the SQL is appended to CALLBACK before
  ** it is executed.
  */
  case DB_TRACE: {
    if( objc>3 ){
      Tcl_WrongNumArgs(interp, 2, objv, "?CALLBACK?");
      return TCL_ERROR;
    }else if( objc==2 ){
      if( pDb->zTrace ){
        Tcl_AppendResult(interp, pDb->zTrace, (char*)0);
      }
    }else{
      char *zTrace;
      int len;
      if( pDb->zTrace ){
        Tcl_Free(pDb->zTrace);
      }
      zTrace = Tcl_GetStringFromObj(objv[2], &len);
      if( zTrace && len>0 ){
        pDb->zTrace = Tcl_Alloc( len + 1 );
        memcpy(pDb->zTrace, zTrace, len+1);
      }else{
        pDb->zTrace = 0;
      }
#if !defined(SQLITE_OMIT_TRACE) && !defined(SQLITE_OMIT_FLOATING_POINT)
      if( pDb->zTrace ){
        pDb->interp = interp;
        sqlite3_trace(pDb->db, DbTraceHandler, pDb);
      }else{
        sqlite3_trace(pDb->db, 0, 0);
      }
#endif
    }
    break;
  }

  /*    $db transaction [-deferred|-immediate|-exclusive] SCRIPT
  **
  ** Start a new transaction (if we are not already in the midst of a
  ** transaction) and execute the TCL script SCRIPT.  After SCRIPT
  ** completes, either commit the transaction or roll it back if SCRIPT
  ** throws an exception.  Or if no new transation was started, do nothing.
  ** pass the exception on up the stack.
  **
  ** This command was inspired by Dave Thomas's talk on Ruby at the
  ** 2005 O'Reilly Open Source Convention (OSCON).
  */
  case DB_TRANSACTION: {
    Tcl_Obj *pScript;
    const char *zBegin = "SAVEPOINT _tcl_transaction";
    if( objc!=3 && objc!=4 ){
      Tcl_WrongNumArgs(interp, 2, objv, "[TYPE] SCRIPT");
      return TCL_ERROR;
    }

    if( pDb->nTransaction==0 && objc==4 ){
      static const char *TTYPE_strs[] = {
        "deferred",   "exclusive",  "immediate", 0
      };
      enum TTYPE_enum {
        TTYPE_DEFERRED, TTYPE_EXCLUSIVE, TTYPE_IMMEDIATE
      };
      int ttype;
      if( Tcl_GetIndexFromObj(interp, objv[2], TTYPE_strs, "transaction type",
                              0, &ttype) ){
        return TCL_ERROR;
      }
      switch( (enum TTYPE_enum)ttype ){
        case TTYPE_DEFERRED:    /* no-op */;                 break;
        case TTYPE_EXCLUSIVE:   zBegin = "BEGIN EXCLUSIVE";  break;
        case TTYPE_IMMEDIATE:   zBegin = "BEGIN IMMEDIATE";  break;
      }
    }
    pScript = objv[objc-1];

    /* Run the SQLite BEGIN command to open a transaction or savepoint. */
    pDb->disableAuth++;
    rc = sqlite3_exec(pDb->db, zBegin, 0, 0, 0);
    pDb->disableAuth--;
    if( rc!=SQLITE_OK ){
      Tcl_AppendResult(interp, sqlite3_errmsg(pDb->db), (char*)0);
      return TCL_ERROR;
    }
    pDb->nTransaction++;

    /* If using NRE, schedule a callback to invoke the script pScript, then
    ** a second callback to commit (or rollback) the transaction or savepoint
    ** opened above. If not using NRE, evaluate the script directly, then
    ** call function DbTransPostCmd() to commit (or rollback) the transaction 
    ** or savepoint.  */
    if( DbUseNre() ){
      Tcl_NRAddCallback(interp, DbTransPostCmd, cd, 0, 0, 0);
      (void)Tcl_NREvalObj(interp, pScript, 0);
    }else{
      rc = DbTransPostCmd(&cd, interp, Tcl_EvalObjEx(interp, pScript, 0));
    }
    break;
  }

  /*
  **    $db unlock_notify ?script?
  */
  case DB_UNLOCK_NOTIFY: {
#ifndef SQLITE_ENABLE_UNLOCK_NOTIFY
    Tcl_AppendResult(interp, "unlock_notify not available in this build",
                     (char*)0);
    rc = TCL_ERROR;
#else
    if( objc!=2 && objc!=3 ){
      Tcl_WrongNumArgs(interp, 2, objv, "?SCRIPT?");
      rc = TCL_ERROR;
    }else{
      void (*xNotify)(void **, int) = 0;
      void *pNotifyArg = 0;

      if( pDb->pUnlockNotify ){
        Tcl_DecrRefCount(pDb->pUnlockNotify);
        pDb->pUnlockNotify = 0;
      }
  
      if( objc==3 ){
        xNotify = DbUnlockNotify;
        pNotifyArg = (void *)pDb;
        pDb->pUnlockNotify = objv[2];
        Tcl_IncrRefCount(pDb->pUnlockNotify);
      }
  
      if( sqlite3_unlock_notify(pDb->db, xNotify, pNotifyArg) ){
        Tcl_AppendResult(interp, sqlite3_errmsg(pDb->db), (char*)0);
        rc = TCL_ERROR;
      }
    }
#endif
    break;
  }

  /*
  **    $db wal_hook ?script?
  **    $db update_hook ?script?
  **    $db rollback_hook ?script?
  */
  case DB_WAL_HOOK: 
  case DB_UPDATE_HOOK: 
  case DB_ROLLBACK_HOOK: {

    /* set ppHook to point at pUpdateHook or pRollbackHook, depending on 
    ** whether [$db update_hook] or [$db rollback_hook] was invoked.
    */
    Tcl_Obj **ppHook; 
    if( choice==DB_UPDATE_HOOK ){
      ppHook = &pDb->pUpdateHook;
    }else if( choice==DB_WAL_HOOK ){
      ppHook = &pDb->pWalHook;
    }else{
      ppHook = &pDb->pRollbackHook;
    }

    if( objc!=2 && objc!=3 ){
       Tcl_WrongNumArgs(interp, 2, objv, "?SCRIPT?");
       return TCL_ERROR;
    }
    if( *ppHook ){
      Tcl_SetObjResult(interp, *ppHook);
      if( objc==3 ){
        Tcl_DecrRefCount(*ppHook);
        *ppHook = 0;
      }
    }
    if( objc==3 ){
      assert( !(*ppHook) );
      if( Tcl_GetCharLength(objv[2])>0 ){
        *ppHook = objv[2];
        Tcl_IncrRefCount(*ppHook);
      }
    }

    sqlite3_update_hook(pDb->db, (pDb->pUpdateHook?DbUpdateHandler:0), pDb);
    sqlite3_rollback_hook(pDb->db,(pDb->pRollbackHook?DbRollbackHandler:0),pDb);
    sqlite3_wal_hook(pDb->db,(pDb->pWalHook?DbWalHandler:0),pDb);

    break;
  }

  /*    $db version
  **
  ** Return the version string for this database.
  */
  case DB_VERSION: {
    Tcl_SetResult(interp, (char *)sqlite3_libversion(), TCL_STATIC);
    break;
  }


  } /* End of the SWITCH statement */
  return rc;
}

#if SQLITE_TCL_NRE
/*
** Adaptor that provides an objCmd interface to the NRE-enabled
** interface implementation.
*/
static int DbObjCmdAdaptor(
  void *cd,
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *const*objv
){
  return Tcl_NRCallObjProc(interp, DbObjCmd, cd, objc, objv);
}
#endif /* SQLITE_TCL_NRE */

/*
**   sqlite3 DBNAME FILENAME ?-vfs VFSNAME? ?-key KEY? ?-readonly BOOLEAN?
**                           ?-create BOOLEAN? ?-nomutex BOOLEAN?
**
** This is the main Tcl command.  When the "sqlite" Tcl command is
** invoked, this routine runs to process that command.
**
** The first argument, DBNAME, is an arbitrary name for a new
** database connection.  This command creates a new command named
** DBNAME that is used to control that connection.  The database
** connection is deleted when the DBNAME command is deleted.
**
** The second argument is the name of the database file.
**
*/
static int DbMain(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
  SqliteDb *p;
  const char *zArg;
  char *zErrMsg;
  int i;
  const char *zFile;
  const char *zVfs = 0;
  int flags;
  Tcl_DString translatedFilename;
#ifdef SQLITE_HAS_CODEC
  void *pKey = 0;
  int nKey = 0;
#endif
  int rc;

  /* In normal use, each TCL interpreter runs in a single thread.  So
  ** by default, we can turn of mutexing on SQLite database connections.
  ** However, for testing purposes it is useful to have mutexes turned
  ** on.  So, by default, mutexes default off.  But if compiled with
  ** SQLITE_TCL_DEFAULT_FULLMUTEX then mutexes default on.
  */
#ifdef SQLITE_TCL_DEFAULT_FULLMUTEX
  flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_FULLMUTEX;
#else
  flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_NOMUTEX;
#endif

  if( objc==2 ){
    zArg = Tcl_GetStringFromObj(objv[1], 0);
    if( strcmp(zArg,"-version")==0 ){
      Tcl_AppendResult(interp,sqlite3_libversion(), (char*)0);
      return TCL_OK;
    }
    if( strcmp(zArg,"-has-codec")==0 ){
#ifdef SQLITE_HAS_CODEC
      Tcl_AppendResult(interp,"1",(char*)0);
#else
      Tcl_AppendResult(interp,"0",(char*)0);
#endif
      return TCL_OK;
    }
  }
  for(i=3; i+1<objc; i+=2){
    zArg = Tcl_GetString(objv[i]);
    if( strcmp(zArg,"-key")==0 ){
#ifdef SQLITE_HAS_CODEC
      pKey = Tcl_GetByteArrayFromObj(objv[i+1], &nKey);
#endif
    }else if( strcmp(zArg, "-vfs")==0 ){
      zVfs = Tcl_GetString(objv[i+1]);
    }else if( strcmp(zArg, "-readonly")==0 ){
      int b;
      if( Tcl_GetBooleanFromObj(interp, objv[i+1], &b) ) return TCL_ERROR;
      if( b ){
        flags &= ~(SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE);
        flags |= SQLITE_OPEN_READONLY;
      }else{
        flags &= ~SQLITE_OPEN_READONLY;
        flags |= SQLITE_OPEN_READWRITE;
      }
    }else if( strcmp(zArg, "-create")==0 ){
      int b;
      if( Tcl_GetBooleanFromObj(interp, objv[i+1], &b) ) return TCL_ERROR;
      if( b && (flags & SQLITE_OPEN_READONLY)==0 ){
        flags |= SQLITE_OPEN_CREATE;
      }else{
        flags &= ~SQLITE_OPEN_CREATE;
      }
    }else if( strcmp(zArg, "-nomutex")==0 ){
      int b;
      if( Tcl_GetBooleanFromObj(interp, objv[i+1], &b) ) return TCL_ERROR;
      if( b ){
        flags |= SQLITE_OPEN_NOMUTEX;
        flags &= ~SQLITE_OPEN_FULLMUTEX;
      }else{
        flags &= ~SQLITE_OPEN_NOMUTEX;
      }
    }else if( strcmp(zArg, "-fullmutex")==0 ){
      int b;
      if( Tcl_GetBooleanFromObj(interp, objv[i+1], &b) ) return TCL_ERROR;
      if( b ){
        flags |= SQLITE_OPEN_FULLMUTEX;
        flags &= ~SQLITE_OPEN_NOMUTEX;
      }else{
        flags &= ~SQLITE_OPEN_FULLMUTEX;
      }
    }else if( strcmp(zArg, "-uri")==0 ){
      int b;
      if( Tcl_GetBooleanFromObj(interp, objv[i+1], &b) ) return TCL_ERROR;
      if( b ){
        flags |= SQLITE_OPEN_URI;
      }else{
        flags &= ~SQLITE_OPEN_URI;
      }
    }else{
      Tcl_AppendResult(interp, "unknown option: ", zArg, (char*)0);
      return TCL_ERROR;
    }
  }
  if( objc<3 || (objc&1)!=1 ){
    Tcl_WrongNumArgs(interp, 1, objv, 
      "HANDLE FILENAME ?-vfs VFSNAME? ?-readonly BOOLEAN? ?-create BOOLEAN?"
      " ?-nomutex BOOLEAN? ?-fullmutex BOOLEAN? ?-uri BOOLEAN?"
#ifdef SQLITE_HAS_CODEC
      " ?-key CODECKEY?"
#endif
    );
    return TCL_ERROR;
  }
  zErrMsg = 0;
  p = (SqliteDb*)Tcl_Alloc( sizeof(*p) );
  if( p==0 ){
    Tcl_SetResult(interp, (char *)"malloc failed", TCL_STATIC);
    return TCL_ERROR;
  }
  memset(p, 0, sizeof(*p));
  zFile = Tcl_GetStringFromObj(objv[2], 0);
  zFile = Tcl_TranslateFileName(interp, zFile, &translatedFilename);
  rc = sqlite3_open_v2(zFile, &p->db, flags, zVfs);
  Tcl_DStringFree(&translatedFilename);
  if( p->db ){
    if( SQLITE_OK!=sqlite3_errcode(p->db) ){
      zErrMsg = sqlite3_mprintf("%s", sqlite3_errmsg(p->db));
      sqlite3_close(p->db);
      p->db = 0;
    }
  }else{
    zErrMsg = sqlite3_mprintf("%s", sqlite3_errstr(rc));
  }
#ifdef SQLITE_HAS_CODEC
  if( p->db ){
    sqlite3_key(p->db, pKey, nKey);
  }
#endif
  if( p->db==0 ){
    Tcl_SetResult(interp, zErrMsg, TCL_VOLATILE);
    Tcl_Free((char*)p);
    sqlite3_free(zErrMsg);
    return TCL_ERROR;
  }
  p->maxStmt = NUM_PREPARED_STMTS;
  p->interp = interp;
  zArg = Tcl_GetStringFromObj(objv[1], 0);
  if( DbUseNre() ){
    Tcl_NRCreateCommand(interp, zArg, DbObjCmdAdaptor, DbObjCmd,
                        (char*)p, DbDeleteCmd);
  }else{
    Tcl_CreateObjCommand(interp, zArg, DbObjCmd, (char*)p, DbDeleteCmd);
  }
  return TCL_OK;
}

/*
** Provide a dummy Tcl_InitStubs if we are using this as a static
** library.
*/
#ifndef USE_TCL_STUBS
# undef  Tcl_InitStubs
# define Tcl_InitStubs(a,b,c) TCL_VERSION
#endif

/*
** Make sure we have a PACKAGE_VERSION macro defined.  This will be
** defined automatically by the TEA makefile.  But other makefiles
** do not define it.
*/
#ifndef PACKAGE_VERSION
# define PACKAGE_VERSION SQLITE_VERSION
#endif

/*
** Initialize this module.
**
** This Tcl module contains only a single new Tcl command named "sqlite".
** (Hence there is no namespace.  There is no point in using a namespace
** if the extension only supplies one new name!)  The "sqlite" command is
** used to open a new SQLite database.  See the DbMain() routine above
** for additional information.
**
** The EXTERN macros are required by TCL in order to work on windows.
*/
EXTERN int Sqlite3_Init(Tcl_Interp *interp){
  int rc = Tcl_InitStubs(interp, "8.4", 0)==0 ? TCL_ERROR : TCL_OK;
  if( rc==TCL_OK ){
    Tcl_CreateObjCommand(interp, "sqlite3", (Tcl_ObjCmdProc*)DbMain, 0, 0);
#ifndef SQLITE_3_SUFFIX_ONLY
    /* The "sqlite" alias is undocumented.  It is here only to support
    ** legacy scripts.  All new scripts should use only the "sqlite3"
    ** command. */
    Tcl_CreateObjCommand(interp, "sqlite", (Tcl_ObjCmdProc*)DbMain, 0, 0);
#endif
    rc = Tcl_PkgProvide(interp, "sqlite3", PACKAGE_VERSION);
  }
  return rc;
}
EXTERN int Tclsqlite3_Init(Tcl_Interp *interp){ return Sqlite3_Init(interp); }
EXTERN int Sqlite3_Unload(Tcl_Interp *interp, int flags){ return TCL_OK; }
EXTERN int Tclsqlite3_Unload(Tcl_Interp *interp, int flags){ return TCL_OK; }

/* Because it accesses the file-system and uses persistent state, SQLite
** is not considered appropriate for safe interpreters.  Hence, we deliberately
** omit the _SafeInit() interfaces.
*/

#ifndef SQLITE_3_SUFFIX_ONLY
int Sqlite_Init(Tcl_Interp *interp){ return Sqlite3_Init(interp); }
int Tclsqlite_Init(Tcl_Interp *interp){ return Sqlite3_Init(interp); }
int Sqlite_Unload(Tcl_Interp *interp, int flags){ return TCL_OK; }
int Tclsqlite_Unload(Tcl_Interp *interp, int flags){ return TCL_OK; }
#endif

#ifdef TCLSH
/*****************************************************************************
** All of the code that follows is used to build standalone TCL interpreters
** that are statically linked with SQLite.  Enable these by compiling
** with -DTCLSH=n where n can be 1 or 2.  An n of 1 generates a standard
** tclsh but with SQLite built in.  An n of 2 generates the SQLite space
** analysis program.
*/

#if defined(SQLITE_TEST) || defined(SQLITE_TCLMD5)
/*
 * This code implements the MD5 message-digest algorithm.
 * The algorithm is due to Ron Rivest.  This code was
 * written by Colin Plumb in 1993, no copyright is claimed.
 * This code is in the public domain; do with it what you wish.
 *
 * Equivalent code is available from RSA Data Security, Inc.
 * This code has been tested against that, and is equivalent,
 * except that you don't need to include two pages of legalese
 * with every copy.
 *
 * To compute the message digest of a chunk of bytes, declare an
 * MD5Context structure, pass it to MD5Init, call MD5Update as
 * needed on buffers full of bytes, and then call MD5Final, which
 * will fill a supplied 16-byte array with the digest.
 */

/*
 * If compiled on a machine that doesn't have a 32-bit integer,
 * you just set "uint32" to the appropriate datatype for an
 * unsigned 32-bit integer.  For example:
 *
 *       cc -Duint32='unsigned long' md5.c
 *
 */
#ifndef uint32
#  define uint32 unsigned int
#endif

struct MD5Context {
  int isInit;
  uint32 buf[4];
  uint32 bits[2];
  unsigned char in[64];
};
typedef struct MD5Context MD5Context;

/*
 * Note: this code is harmless on little-endian machines.
 */
static void byteReverse (unsigned char *buf, unsigned longs){
        uint32 t;
        do {
                t = (uint32)((unsigned)buf[3]<<8 | buf[2]) << 16 |
                            ((unsigned)buf[1]<<8 | buf[0]);
                *(uint32 *)buf = t;
                buf += 4;
        } while (--longs);
}
/* The four core functions - F1 is optimized somewhat */

/* #define F1(x, y, z) (x & y | ~x & z) */
#define F1(x, y, z) (z ^ (x & (y ^ z)))
#define F2(x, y, z) F1(z, x, y)
#define F3(x, y, z) (x ^ y ^ z)
#define F4(x, y, z) (y ^ (x | ~z))

/* This is the central step in the MD5 algorithm. */
#define MD5STEP(f, w, x, y, z, data, s) \
        ( w += f(x, y, z) + data,  w = w<<s | w>>(32-s),  w += x )

/*
 * The core of the MD5 algorithm, this alters an existing MD5 hash to
 * reflect the addition of 16 longwords of new data.  MD5Update blocks
 * the data and converts bytes into longwords for this routine.
 */
static void MD5Transform(uint32 buf[4], const uint32 in[16]){
        register uint32 a, b, c, d;

        a = buf[0];
        b = buf[1];
        c = buf[2];
        d = buf[3];

        MD5STEP(F1, a, b, c, d, in[ 0]+0xd76aa478,  7);
        MD5STEP(F1, d, a, b, c, in[ 1]+0xe8c7b756, 12);
        MD5STEP(F1, c, d, a, b, in[ 2]+0x242070db, 17);
        MD5STEP(F1, b, c, d, a, in[ 3]+0xc1bdceee, 22);
        MD5STEP(F1, a, b, c, d, in[ 4]+0xf57c0faf,  7);
        MD5STEP(F1, d, a, b, c, in[ 5]+0x4787c62a, 12);
        MD5STEP(F1, c, d, a, b, in[ 6]+0xa8304613, 17);
        MD5STEP(F1, b, c, d, a, in[ 7]+0xfd469501, 22);
        MD5STEP(F1, a, b, c, d, in[ 8]+0x698098d8,  7);
        MD5STEP(F1, d, a, b, c, in[ 9]+0x8b44f7af, 12);
        MD5STEP(F1, c, d, a, b, in[10]+0xffff5bb1, 17);
        MD5STEP(F1, b, c, d, a, in[11]+0x895cd7be, 22);
        MD5STEP(F1, a, b, c, d, in[12]+0x6b901122,  7);
        MD5STEP(F1, d, a, b, c, in[13]+0xfd987193, 12);
        MD5STEP(F1, c, d, a, b, in[14]+0xa679438e, 17);
        MD5STEP(F1, b, c, d, a, in[15]+0x49b40821, 22);

        MD5STEP(F2, a, b, c, d, in[ 1]+0xf61e2562,  5);
        MD5STEP(F2, d, a, b, c, in[ 6]+0xc040b340,  9);
        MD5STEP(F2, c, d, a, b, in[11]+0x265e5a51, 14);
        MD5STEP(F2, b, c, d, a, in[ 0]+0xe9b6c7aa, 20);
        MD5STEP(F2, a, b, c, d, in[ 5]+0xd62f105d,  5);
        MD5STEP(F2, d, a, b, c, in[10]+0x02441453,  9);
        MD5STEP(F2, c, d, a, b, in[15]+0xd8a1e681, 14);
        MD5STEP(F2, b, c, d, a, in[ 4]+0xe7d3fbc8, 20);
        MD5STEP(F2, a, b, c, d, in[ 9]+0x21e1cde6,  5);
        MD5STEP(F2, d, a, b, c, in[14]+0xc33707d6,  9);
        MD5STEP(F2, c, d, a, b, in[ 3]+0xf4d50d87, 14);
        MD5STEP(F2, b, c, d, a, in[ 8]+0x455a14ed, 20);
        MD5STEP(F2, a, b, c, d, in[13]+0xa9e3e905,  5);
        MD5STEP(F2, d, a, b, c, in[ 2]+0xfcefa3f8,  9);
        MD5STEP(F2, c, d, a, b, in[ 7]+0x676f02d9, 14);
        MD5STEP(F2, b, c, d, a, in[12]+0x8d2a4c8a, 20);

        MD5STEP(F3, a, b, c, d, in[ 5]+0xfffa3942,  4);
        MD5STEP(F3, d, a, b, c, in[ 8]+0x8771f681, 11);
        MD5STEP(F3, c, d, a, b, in[11]+0x6d9d6122, 16);
        MD5STEP(F3, b, c, d, a, in[14]+0xfde5380c, 23);
        MD5STEP(F3, a, b, c, d, in[ 1]+0xa4beea44,  4);
        MD5STEP(F3, d, a, b, c, in[ 4]+0x4bdecfa9, 11);
        MD5STEP(F3, c, d, a, b, in[ 7]+0xf6bb4b60, 16);
        MD5STEP(F3, b, c, d, a, in[10]+0xbebfbc70, 23);
        MD5STEP(F3, a, b, c, d, in[13]+0x289b7ec6,  4);
        MD5STEP(F3, d, a, b, c, in[ 0]+0xeaa127fa, 11);
        MD5STEP(F3, c, d, a, b, in[ 3]+0xd4ef3085, 16);
        MD5STEP(F3, b, c, d, a, in[ 6]+0x04881d05, 23);
        MD5STEP(F3, a, b, c, d, in[ 9]+0xd9d4d039,  4);
        MD5STEP(F3, d, a, b, c, in[12]+0xe6db99e5, 11);
        MD5STEP(F3, c, d, a, b, in[15]+0x1fa27cf8, 16);
        MD5STEP(F3, b, c, d, a, in[ 2]+0xc4ac5665, 23);

        MD5STEP(F4, a, b, c, d, in[ 0]+0xf4292244,  6);
        MD5STEP(F4, d, a, b, c, in[ 7]+0x432aff97, 10);
        MD5STEP(F4, c, d, a, b, in[14]+0xab9423a7, 15);
        MD5STEP(F4, b, c, d, a, in[ 5]+0xfc93a039, 21);
        MD5STEP(F4, a, b, c, d, in[12]+0x655b59c3,  6);
        MD5STEP(F4, d, a, b, c, in[ 3]+0x8f0ccc92, 10);
        MD5STEP(F4, c, d, a, b, in[10]+0xffeff47d, 15);
        MD5STEP(F4, b, c, d, a, in[ 1]+0x85845dd1, 21);
        MD5STEP(F4, a, b, c, d, in[ 8]+0x6fa87e4f,  6);
        MD5STEP(F4, d, a, b, c, in[15]+0xfe2ce6e0, 10);
        MD5STEP(F4, c, d, a, b, in[ 6]+0xa3014314, 15);
        MD5STEP(F4, b, c, d, a, in[13]+0x4e0811a1, 21);
        MD5STEP(F4, a, b, c, d, in[ 4]+0xf7537e82,  6);
        MD5STEP(F4, d, a, b, c, in[11]+0xbd3af235, 10);
        MD5STEP(F4, c, d, a, b, in[ 2]+0x2ad7d2bb, 15);
        MD5STEP(F4, b, c, d, a, in[ 9]+0xeb86d391, 21);

        buf[0] += a;
        buf[1] += b;
        buf[2] += c;
        buf[3] += d;
}

/*
 * Start MD5 accumulation.  Set bit count to 0 and buffer to mysterious
 * initialization constants.
 */
static void MD5Init(MD5Context *ctx){
        ctx->isInit = 1;
        ctx->buf[0] = 0x67452301;
        ctx->buf[1] = 0xefcdab89;
        ctx->buf[2] = 0x98badcfe;
        ctx->buf[3] = 0x10325476;
        ctx->bits[0] = 0;
        ctx->bits[1] = 0;
}

/*
 * Update context to reflect the concatenation of another buffer full
 * of bytes.
 */
static 
void MD5Update(MD5Context *ctx, const unsigned char *buf, unsigned int len){
        uint32 t;

        /* Update bitcount */

        t = ctx->bits[0];
        if ((ctx->bits[0] = t + ((uint32)len << 3)) < t)
                ctx->bits[1]++; /* Carry from low to high */
        ctx->bits[1] += len >> 29;

        t = (t >> 3) & 0x3f;    /* Bytes already in shsInfo->data */

        /* Handle any leading odd-sized chunks */

        if ( t ) {
                unsigned char *p = (unsigned char *)ctx->in + t;

                t = 64-t;
                if (len < t) {
                        memcpy(p, buf, len);
                        return;
                }
                memcpy(p, buf, t);
                byteReverse(ctx->in, 16);
                MD5Transform(ctx->buf, (uint32 *)ctx->in);
                buf += t;
                len -= t;
        }

        /* Process data in 64-byte chunks */

        while (len >= 64) {
                memcpy(ctx->in, buf, 64);
                byteReverse(ctx->in, 16);
                MD5Transform(ctx->buf, (uint32 *)ctx->in);
                buf += 64;
                len -= 64;
        }

        /* Handle any remaining bytes of data. */

        memcpy(ctx->in, buf, len);
}

/*
 * Final wrapup - pad to 64-byte boundary with the bit pattern 
 * 1 0* (64-bit count of bits processed, MSB-first)
 */
static void MD5Final(unsigned char digest[16], MD5Context *ctx){
        unsigned count;
        unsigned char *p;

        /* Compute number of bytes mod 64 */
        count = (ctx->bits[0] >> 3) & 0x3F;

        /* Set the first char of padding to 0x80.  This is safe since there is
           always at least one byte free */
        p = ctx->in + count;
        *p++ = 0x80;

        /* Bytes of padding needed to make 64 bytes */
        count = 64 - 1 - count;

        /* Pad out to 56 mod 64 */
        if (count < 8) {
                /* Two lots of padding:  Pad the first block to 64 bytes */
                memset(p, 0, count);
                byteReverse(ctx->in, 16);
                MD5Transform(ctx->buf, (uint32 *)ctx->in);

                /* Now fill the next block with 56 bytes */
                memset(ctx->in, 0, 56);
        } else {
                /* Pad block to 56 bytes */
                memset(p, 0, count-8);
        }
        byteReverse(ctx->in, 14);

        /* Append length in bits and transform */
        memcpy(ctx->in + 14*4, ctx->bits, 8);

        MD5Transform(ctx->buf, (uint32 *)ctx->in);
        byteReverse((unsigned char *)ctx->buf, 4);
        memcpy(digest, ctx->buf, 16);
}

/*
** Convert a 128-bit MD5 digest into a 32-digit base-16 number.
*/
static void MD5DigestToBase16(unsigned char *digest, char *zBuf){
  static char const zEncode[] = "0123456789abcdef";
  int i, j;

  for(j=i=0; i<16; i++){
    int a = digest[i];
    zBuf[j++] = zEncode[(a>>4)&0xf];
    zBuf[j++] = zEncode[a & 0xf];
  }
  zBuf[j] = 0;
}


/*
** Convert a 128-bit MD5 digest into sequency of eight 5-digit integers
** each representing 16 bits of the digest and separated from each
** other by a "-" character.
*/
static void MD5DigestToBase10x8(unsigned char digest[16], char zDigest[50]){
  int i, j;
  unsigned int x;
  for(i=j=0; i<16; i+=2){
    x = digest[i]*256 + digest[i+1];
    if( i>0 ) zDigest[j++] = '-';
    sprintf(&zDigest[j], "%05u", x);
    j += 5;
  }
  zDigest[j] = 0;
}

/*
** A TCL command for md5.  The argument is the text to be hashed.  The
** Result is the hash in base64.  
*/
static int md5_cmd(void*cd, Tcl_Interp *interp, int argc, const char **argv){
  MD5Context ctx;
  unsigned char digest[16];
  char zBuf[50];
  void (*converter)(unsigned char*, char*);

  if( argc!=2 ){
    Tcl_AppendResult(interp,"wrong # args: should be \"", argv[0], 
        " TEXT\"", (char*)0);
    return TCL_ERROR;
  }
  MD5Init(&ctx);
  MD5Update(&ctx, (unsigned char*)argv[1], (unsigned)strlen(argv[1]));
  MD5Final(digest, &ctx);
  converter = (void(*)(unsigned char*,char*))cd;
  converter(digest, zBuf);
  Tcl_AppendResult(interp, zBuf, (char*)0);
  return TCL_OK;
}

/*
** A TCL command to take the md5 hash of a file.  The argument is the
** name of the file.
*/
static int md5file_cmd(void*cd, Tcl_Interp*interp, int argc, const char **argv){
  FILE *in;
  MD5Context ctx;
  void (*converter)(unsigned char*, char*);
  unsigned char digest[16];
  char zBuf[10240];

  if( argc!=2 ){
    Tcl_AppendResult(interp,"wrong # args: should be \"", argv[0], 
        " FILENAME\"", (char*)0);
    return TCL_ERROR;
  }
  in = fopen(argv[1],"rb");
  if( in==0 ){
    Tcl_AppendResult(interp,"unable to open file \"", argv[1], 
         "\" for reading", (char*)0);
    return TCL_ERROR;
  }
  MD5Init(&ctx);
  for(;;){
    int n;
    n = (int)fread(zBuf, 1, sizeof(zBuf), in);
    if( n<=0 ) break;
    MD5Update(&ctx, (unsigned char*)zBuf, (unsigned)n);
  }
  fclose(in);
  MD5Final(digest, &ctx);
  converter = (void(*)(unsigned char*,char*))cd;
  converter(digest, zBuf);
  Tcl_AppendResult(interp, zBuf, (char*)0);
  return TCL_OK;
}

/*
** Register the four new TCL commands for generating MD5 checksums
** with the TCL interpreter.
*/
int Md5_Init(Tcl_Interp *interp){
  Tcl_CreateCommand(interp, "md5", (Tcl_CmdProc*)md5_cmd,
                    MD5DigestToBase16, 0);
  Tcl_CreateCommand(interp, "md5-10x8", (Tcl_CmdProc*)md5_cmd,
                    MD5DigestToBase10x8, 0);
  Tcl_CreateCommand(interp, "md5file", (Tcl_CmdProc*)md5file_cmd,
                    MD5DigestToBase16, 0);
  Tcl_CreateCommand(interp, "md5file-10x8", (Tcl_CmdProc*)md5file_cmd,
                    MD5DigestToBase10x8, 0);
  return TCL_OK;
}
#endif /* defined(SQLITE_TEST) || defined(SQLITE_TCLMD5) */

#if defined(SQLITE_TEST)
/*
** During testing, the special md5sum() aggregate function is available.
** inside SQLite.  The following routines implement that function.
*/
static void md5step(sqlite3_context *context, int argc, sqlite3_value **argv){
  MD5Context *p;
  int i;
  if( argc<1 ) return;
  p = sqlite3_aggregate_context(context, sizeof(*p));
  if( p==0 ) return;
  if( !p->isInit ){
    MD5Init(p);
  }
  for(i=0; i<argc; i++){
    const char *zData = (char*)sqlite3_value_text(argv[i]);
    if( zData ){
      MD5Update(p, (unsigned char*)zData, (int)strlen(zData));
    }
  }
}
static void md5finalize(sqlite3_context *context){
  MD5Context *p;
  unsigned char digest[16];
  char zBuf[33];
  p = sqlite3_aggregate_context(context, sizeof(*p));
  MD5Final(digest,p);
  MD5DigestToBase16(digest, zBuf);
  sqlite3_result_text(context, zBuf, -1, SQLITE_TRANSIENT);
}
int Md5_Register(sqlite3 *db){
  int rc = sqlite3_create_function(db, "md5sum", -1, SQLITE_UTF8, 0, 0, 
                                 md5step, md5finalize);
  sqlite3_overload_function(db, "md5sum", -1);  /* To exercise this API */
  return rc;
}
#endif /* defined(SQLITE_TEST) */


/*
** If the macro TCLSH is one, then put in code this for the
** "main" routine that will initialize Tcl and take input from
** standard input, or if a file is named on the command line
** the TCL interpreter reads and evaluates that file.
*/
#if TCLSH==1
static const char *tclsh_main_loop(void){
  static const char zMainloop[] =
    "set line {}\n"
    "while {![eof stdin]} {\n"
      "if {$line!=\"\"} {\n"
        "puts -nonewline \"> \"\n"
      "} else {\n"
        "puts -nonewline \"% \"\n"
      "}\n"
      "flush stdout\n"
      "append line [gets stdin]\n"
      "if {[info complete $line]} {\n"
        "if {[catch {uplevel #0 $line} result]} {\n"
          "puts stderr \"Error: $result\"\n"
        "} elseif {$result!=\"\"} {\n"
          "puts $result\n"
        "}\n"
        "set line {}\n"
      "} else {\n"
        "append line \\n\n"
      "}\n"
    "}\n"
  ;
  return zMainloop;
}
#endif
#if TCLSH==2
static const char *tclsh_main_loop(void);
#endif

#ifdef SQLITE_TEST
static void init_all(Tcl_Interp *);
static int init_all_cmd(
  ClientData cd,
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){

  Tcl_Interp *slave;
  if( objc!=2 ){
    Tcl_WrongNumArgs(interp, 1, objv, "SLAVE");
    return TCL_ERROR;
  }

  slave = Tcl_GetSlave(interp, Tcl_GetString(objv[1]));
  if( !slave ){
    return TCL_ERROR;
  }

  init_all(slave);
  return TCL_OK;
}

/*
** Tclcmd: db_use_legacy_prepare DB BOOLEAN
**
**   The first argument to this command must be a database command created by
**   [sqlite3]. If the second argument is true, then the handle is configured
**   to use the sqlite3_prepare_v2() function to prepare statements. If it
**   is false, sqlite3_prepare().
*/
static int db_use_legacy_prepare_cmd(
  ClientData cd,
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){
  Tcl_CmdInfo cmdInfo;
  SqliteDb *pDb;
  int bPrepare;

  if( objc!=3 ){
    Tcl_WrongNumArgs(interp, 1, objv, "DB BOOLEAN");
    return TCL_ERROR;
  }

  if( !Tcl_GetCommandInfo(interp, Tcl_GetString(objv[1]), &cmdInfo) ){
    Tcl_AppendResult(interp, "no such db: ", Tcl_GetString(objv[1]), (char*)0);
    return TCL_ERROR;
  }
  pDb = (SqliteDb*)cmdInfo.objClientData;
  if( Tcl_GetBooleanFromObj(interp, objv[2], &bPrepare) ){
    return TCL_ERROR;
  }

  pDb->bLegacyPrepare = bPrepare;

  Tcl_ResetResult(interp);
  return TCL_OK;
}
#endif

/*
** Configure the interpreter passed as the first argument to have access
** to the commands and linked variables that make up:
**
**   * the [sqlite3] extension itself, 
**
**   * If SQLITE_TCLMD5 or SQLITE_TEST is defined, the Md5 commands, and
**
**   * If SQLITE_TEST is set, the various test interfaces used by the Tcl
**     test suite.
*/
static void init_all(Tcl_Interp *interp){
  Sqlite3_Init(interp);

#if defined(SQLITE_TEST) || defined(SQLITE_TCLMD5)
  Md5_Init(interp);
#endif

  /* Install the [register_dbstat_vtab] command to access the implementation
  ** of virtual table dbstat (source file test_stat.c). This command is
  ** required for testfixture and sqlite3_analyzer, but not by the production
  ** Tcl extension.  */
#if defined(SQLITE_TEST) || TCLSH==2
  {
    extern int SqlitetestStat_Init(Tcl_Interp*);
    SqlitetestStat_Init(interp);
  }
#endif

#ifdef SQLITE_TEST
  {
    extern int Sqliteconfig_Init(Tcl_Interp*);
    extern int Sqlitetest1_Init(Tcl_Interp*);
    extern int Sqlitetest2_Init(Tcl_Interp*);
    extern int Sqlitetest3_Init(Tcl_Interp*);
    extern int Sqlitetest4_Init(Tcl_Interp*);
    extern int Sqlitetest5_Init(Tcl_Interp*);
    extern int Sqlitetest6_Init(Tcl_Interp*);
    extern int Sqlitetest7_Init(Tcl_Interp*);
    extern int Sqlitetest8_Init(Tcl_Interp*);
    extern int Sqlitetest9_Init(Tcl_Interp*);
    extern int Sqlitetestasync_Init(Tcl_Interp*);
    extern int Sqlitetest_autoext_Init(Tcl_Interp*);
    extern int Sqlitetest_demovfs_Init(Tcl_Interp *);
    extern int Sqlitetest_func_Init(Tcl_Interp*);
    extern int Sqlitetest_hexio_Init(Tcl_Interp*);
    extern int Sqlitetest_init_Init(Tcl_Interp*);
    extern int Sqlitetest_malloc_Init(Tcl_Interp*);
    extern int Sqlitetest_mutex_Init(Tcl_Interp*);
    extern int Sqlitetestschema_Init(Tcl_Interp*);
    extern int Sqlitetestsse_Init(Tcl_Interp*);
    extern int Sqlitetesttclvar_Init(Tcl_Interp*);
    extern int Sqlitetestfs_Init(Tcl_Interp*);
    extern int SqlitetestThread_Init(Tcl_Interp*);
    extern int SqlitetestOnefile_Init();
    extern int SqlitetestOsinst_Init(Tcl_Interp*);
    extern int Sqlitetestbackup_Init(Tcl_Interp*);
    extern int Sqlitetestintarray_Init(Tcl_Interp*);
    extern int Sqlitetestvfs_Init(Tcl_Interp *);
    extern int Sqlitetestrtree_Init(Tcl_Interp*);
    extern int Sqlitequota_Init(Tcl_Interp*);
    extern int Sqlitemultiplex_Init(Tcl_Interp*);
    extern int SqliteSuperlock_Init(Tcl_Interp*);
    extern int SqlitetestSyscall_Init(Tcl_Interp*);

#if defined(SQLITE_ENABLE_FTS3) || defined(SQLITE_ENABLE_FTS4)
    extern int Sqlitetestfts3_Init(Tcl_Interp *interp);
#endif

#ifdef SQLITE_ENABLE_ZIPVFS
    extern int Zipvfs_Init(Tcl_Interp*);
    Zipvfs_Init(interp);
#endif

    Sqliteconfig_Init(interp);
    Sqlitetest1_Init(interp);
    Sqlitetest2_Init(interp);
    Sqlitetest3_Init(interp);
    Sqlitetest4_Init(interp);
    Sqlitetest5_Init(interp);
    Sqlitetest6_Init(interp);
    Sqlitetest7_Init(interp);
    Sqlitetest8_Init(interp);
    Sqlitetest9_Init(interp);
    Sqlitetestasync_Init(interp);
    Sqlitetest_autoext_Init(interp);
    Sqlitetest_demovfs_Init(interp);
    Sqlitetest_func_Init(interp);
    Sqlitetest_hexio_Init(interp);
    Sqlitetest_init_Init(interp);
    Sqlitetest_malloc_Init(interp);
    Sqlitetest_mutex_Init(interp);
    Sqlitetestschema_Init(interp);
    Sqlitetesttclvar_Init(interp);
    Sqlitetestfs_Init(interp);
    SqlitetestThread_Init(interp);
    SqlitetestOnefile_Init(interp);
    SqlitetestOsinst_Init(interp);
    Sqlitetestbackup_Init(interp);
    Sqlitetestintarray_Init(interp);
    Sqlitetestvfs_Init(interp);
    Sqlitetestrtree_Init(interp);
    Sqlitequota_Init(interp);
    Sqlitemultiplex_Init(interp);
    SqliteSuperlock_Init(interp);
    SqlitetestSyscall_Init(interp);

#if defined(SQLITE_ENABLE_FTS3) || defined(SQLITE_ENABLE_FTS4)
    Sqlitetestfts3_Init(interp);
#endif

    Tcl_CreateObjCommand(
        interp, "load_testfixture_extensions", init_all_cmd, 0, 0
    );
    Tcl_CreateObjCommand(
        interp, "db_use_legacy_prepare", db_use_legacy_prepare_cmd, 0, 0
    );

#ifdef SQLITE_SSE
    Sqlitetestsse_Init(interp);
#endif
  }
#endif
}

#define TCLSH_MAIN main   /* Needed to fake out mktclapp */
int TCLSH_MAIN(int argc, char **argv){
  Tcl_Interp *interp;

#if !defined(_WIN32_WCE)
  if( getenv("BREAK") ){
    fprintf(stderr,
        "attach debugger to process %d and press any key to continue.\n",
        GETPID());
    fgetc(stdin);
  }
#endif

  /* Call sqlite3_shutdown() once before doing anything else. This is to
  ** test that sqlite3_shutdown() can be safely called by a process before
  ** sqlite3_initialize() is. */
  sqlite3_shutdown();

  Tcl_FindExecutable(argv[0]);
  Tcl_SetSystemEncoding(NULL, "utf-8");
  interp = Tcl_CreateInterp();

#if TCLSH==2
  sqlite3_config(SQLITE_CONFIG_SINGLETHREAD);
#endif

  init_all(interp);
  if( argc>=2 ){
    int i;
    char zArgc[32];
    sqlite3_snprintf(sizeof(zArgc), zArgc, "%d", argc-(3-TCLSH));
    Tcl_SetVar(interp,"argc", zArgc, TCL_GLOBAL_ONLY);
    Tcl_SetVar(interp,"argv0",argv[1],TCL_GLOBAL_ONLY);
    Tcl_SetVar(interp,"argv", "", TCL_GLOBAL_ONLY);
    for(i=3-TCLSH; i<argc; i++){
      Tcl_SetVar(interp, "argv", argv[i],
          TCL_GLOBAL_ONLY | TCL_LIST_ELEMENT | TCL_APPEND_VALUE);
    }
    if( TCLSH==1 && Tcl_EvalFile(interp, argv[1])!=TCL_OK ){
      const char *zInfo = Tcl_GetVar(interp, "errorInfo", TCL_GLOBAL_ONLY);
      if( zInfo==0 ) zInfo = Tcl_GetStringResult(interp);
      fprintf(stderr,"%s: %s\n", *argv, zInfo);
      return 1;
    }
  }
  if( TCLSH==2 || argc<=1 ){
    Tcl_GlobalEval(interp, tclsh_main_loop());
  }
  return 0;
}
#endif /* TCLSH */
static const char *tclsh_main_loop(void){
static const char zMainLoop[] = 
  "# Run this TCL script using \"testfixture\" in order get a report that shows\n"
  "# how much disk space is used by a particular data to actually store data\n"
  "# versus how much space is unused.\n"
  "#\n"
  "\n"
  "if {[catch {\n"
  "# Get the name of the database to analyze\n"
  "#\n"
  "proc usage {} {\n"
  "  set argv0 [file rootname [file tail [info nameofexecutable]]]\n"
  "  puts stderr \"Usage: $argv0 database-name\"\n"
  "  exit 1\n"
  "}\n"
  "set file_to_analyze {}\n"
  "set flags(-pageinfo) 0\n"
  "set flags(-stats) 0\n"
  "append argv {}\n"
  "foreach arg $argv {\n"
  "  if {[regexp {^-+pageinfo$} $arg]} {\n"
  "    set flags(-pageinfo) 1\n"
  "  } elseif {[regexp {^-+stats$} $arg]} {\n"
  "    set flags(-stats) 1\n"
  "  } elseif {[regexp {^-} $arg]} {\n"
  "    puts stderr \"Unknown option: $arg\"\n"
  "    usage\n"
  "  } elseif {$file_to_analyze!=\"\"} {\n"
  "    usage\n"
  "  } else {\n"
  "    set file_to_analyze $arg\n"
  "  }\n"
  "}\n"
  "if {$file_to_analyze==\"\"} usage\n"
  "set root_filename $file_to_analyze\n"
  "regexp {^file:(//)?([^?]*)} $file_to_analyze all x1 root_filename\n"
  "if {![file exists $root_filename]} {\n"
  "  puts stderr \"No such file: $root_filename\"\n"
  "  exit 1\n"
  "}\n"
  "if {![file readable $root_filename]} {\n"
  "  puts stderr \"File is not readable: $root_filename\"\n"
  "  exit 1\n"
  "}\n"
  "set true_file_size [file size $root_filename]\n"
  "if {$true_file_size<512} {\n"
  "  puts stderr \"Empty or malformed database: $root_filename\"\n"
  "  exit 1\n"
  "}\n"
  "\n"
  "# Compute the total file size assuming test_multiplexor is being used.\n"
  "# Assume that SQLITE_ENABLE_8_3_NAMES might be enabled\n"
  "#\n"
  "set extension [file extension $root_filename]\n"
  "set pattern $root_filename\n"
  "append pattern {[0-3][0-9][0-9]}\n"
  "foreach f [glob -nocomplain $pattern] {\n"
  "  incr true_file_size [file size $f]\n"
  "  set extension {}\n"
  "}\n"
  "if {[string length $extension]>=2 && [string length $extension]<=4} {\n"
  "  set pattern [file rootname $root_filename]\n"
  "  append pattern {.[0-3][0-9][0-9]}\n"
  "  foreach f [glob -nocomplain $pattern] {\n"
  "    incr true_file_size [file size $f]\n"
  "  }\n"
  "}\n"
  "\n"
  "# Open the database\n"
  "#\n"
  "if {[catch {sqlite3 db $file_to_analyze -uri 1} msg]} {\n"
  "  puts stderr \"error trying to open $file_to_analyze: $msg\"\n"
  "  exit 1\n"
  "}\n"
  "register_dbstat_vtab db\n"
  "\n"
  "db eval {SELECT count(*) FROM sqlite_master}\n"
  "set pageSize [expr {wide([db one {PRAGMA page_size}])}]\n"
  "\n"
  "if {$flags(-pageinfo)} {\n"
  "  db eval {CREATE VIRTUAL TABLE temp.stat USING dbstat}\n"
  "  db eval {SELECT name, path, pageno FROM temp.stat ORDER BY pageno} {\n"
  "    puts \"$pageno $name $path\"\n"
  "  }\n"
  "  exit 0\n"
  "}\n"
  "if {$flags(-stats)} {\n"
  "  db eval {CREATE VIRTUAL TABLE temp.stat USING dbstat}\n"
  "  puts \"BEGIN;\"\n"
  "  puts \"CREATE TABLE stats(\"\n"
  "  puts \"  name       STRING,           /* Name of table or index */\"\n"
  "  puts \"  path       INTEGER,          /* Path to page from root */\"\n"
  "  puts \"  pageno     INTEGER,          /* Page number */\"\n"
  "  puts \"  pagetype   STRING,           /* 'internal', 'leaf' or 'overflow' */\"\n"
  "  puts \"  ncell      INTEGER,          /* Cells on page (0 for overflow) */\"\n"
  "  puts \"  payload    INTEGER,          /* Bytes of payload on this page */\"\n"
  "  puts \"  unused     INTEGER,          /* Bytes of unused space on this page */\"\n"
  "  puts \"  mx_payload INTEGER,          /* Largest payload size of all cells */\"\n"
  "  puts \"  pgoffset   INTEGER,          /* Offset of page in file */\"\n"
  "  puts \"  pgsize     INTEGER           /* Size of the page */\"\n"
  "  puts \");\"\n"
  "  db eval {SELECT quote(name) || ',' ||\n"
  "                  quote(path) || ',' ||\n"
  "                  quote(pageno) || ',' ||\n"
  "                  quote(pagetype) || ',' ||\n"
  "                  quote(ncell) || ',' ||\n"
  "                  quote(payload) || ',' ||\n"
  "                  quote(unused) || ',' ||\n"
  "                  quote(mx_payload) || ',' ||\n"
  "                  quote(pgoffset) || ',' ||\n"
  "                  quote(pgsize) AS x FROM stat} {\n"
  "    puts \"INSERT INTO stats VALUES($x);\"\n"
  "  }\n"
  "  puts \"COMMIT;\"\n"
  "  exit 0\n"
  "}\n"
  "\n"
  "# In-memory database for collecting statistics. This script loops through\n"
  "# the tables and indices in the database being analyzed, adding a row for each\n"
  "# to an in-memory database (for which the schema is shown below). It then\n"
  "# queries the in-memory db to produce the space-analysis report.\n"
  "#\n"
  "sqlite3 mem :memory:\n"
  "set tabledef {CREATE TABLE space_used(\n"
  "   name clob,        -- Name of a table or index in the database file\n"
  "   tblname clob,     -- Name of associated table\n"
  "   is_index boolean, -- TRUE if it is an index, false for a table\n"
  "   nentry int,       -- Number of entries in the BTree\n"
  "   leaf_entries int, -- Number of leaf entries\n"
  "   payload int,      -- Total amount of data stored in this table or index\n"
  "   ovfl_payload int, -- Total amount of data stored on overflow pages\n"
  "   ovfl_cnt int,     -- Number of entries that use overflow\n"
  "   mx_payload int,   -- Maximum payload size\n"
  "   int_pages int,    -- Number of interior pages used\n"
  "   leaf_pages int,   -- Number of leaf pages used\n"
  "   ovfl_pages int,   -- Number of overflow pages used\n"
  "   int_unused int,   -- Number of unused bytes on interior pages\n"
  "   leaf_unused int,  -- Number of unused bytes on primary pages\n"
  "   ovfl_unused int,  -- Number of unused bytes on overflow pages\n"
  "   gap_cnt int,      -- Number of gaps in the page layout\n"
  "   compressed_size int  -- Total bytes stored on disk\n"
  ");}\n"
  "mem eval $tabledef\n"
  "\n"
  "# Create a temporary \"dbstat\" virtual table.\n"
  "#\n"
  "db eval {CREATE VIRTUAL TABLE temp.stat USING dbstat}\n"
  "db eval {CREATE TEMP TABLE dbstat AS SELECT * FROM temp.stat\n"
  "         ORDER BY name, path}\n"
  "db eval {DROP TABLE temp.stat}\n"
  "\n"
  "proc isleaf {pagetype is_index} {\n"
  "  return [expr {$pagetype == \"leaf\" || ($pagetype == \"internal\" && $is_index)}]\n"
  "}\n"
  "proc isoverflow {pagetype is_index} {\n"
  "  return [expr {$pagetype == \"overflow\"}]\n"
  "}\n"
  "proc isinternal {pagetype is_index} {\n"
  "  return [expr {$pagetype == \"internal\" && $is_index==0}]\n"
  "}\n"
  "\n"
  "db func isleaf isleaf\n"
  "db func isinternal isinternal\n"
  "db func isoverflow isoverflow\n"
  "\n"
  "set isCompressed 0\n"
  "set compressOverhead 0\n"
  "set sql { SELECT name, tbl_name FROM sqlite_master WHERE rootpage>0 }\n"
  "foreach {name tblname} [concat sqlite_master sqlite_master [db eval $sql]] {\n"
  "\n"
  "  set is_index [expr {$name!=$tblname}]\n"
  "  db eval {\n"
  "    SELECT \n"
  "      sum(ncell) AS nentry,\n"
  "      sum(isleaf(pagetype, $is_index) * ncell) AS leaf_entries,\n"
  "      sum(payload) AS payload,\n"
  "      sum(isoverflow(pagetype, $is_index) * payload) AS ovfl_payload,\n"
  "      sum(path LIKE '%+000000') AS ovfl_cnt,\n"
  "      max(mx_payload) AS mx_payload,\n"
  "      sum(isinternal(pagetype, $is_index)) AS int_pages,\n"
  "      sum(isleaf(pagetype, $is_index)) AS leaf_pages,\n"
  "      sum(isoverflow(pagetype, $is_index)) AS ovfl_pages,\n"
  "      sum(isinternal(pagetype, $is_index) * unused) AS int_unused,\n"
  "      sum(isleaf(pagetype, $is_index) * unused) AS leaf_unused,\n"
  "      sum(isoverflow(pagetype, $is_index) * unused) AS ovfl_unused,\n"
  "      sum(pgsize) AS compressed_size\n"
  "    FROM temp.dbstat WHERE name = $name\n"
  "  } break\n"
  "\n"
  "  set total_pages [expr {$leaf_pages+$int_pages+$ovfl_pages}]\n"
  "  set storage [expr {$total_pages*$pageSize}]\n"
  "  if {!$isCompressed && $storage>$compressed_size} {\n"
  "    set isCompressed 1\n"
  "    set compressOverhead 14\n"
  "  }\n"
  "\n"
  "  # Column 'gap_cnt' is set to the number of non-contiguous entries in the\n"
  "  # list of pages visited if the b-tree structure is traversed in a top-down\n"
  "  # fashion (each node visited before its child-tree is passed). Any overflow\n"
  "  # chains present are traversed from start to finish before any child-tree\n"
  "  # is.\n"
  "  #\n"
  "  set gap_cnt 0\n"
  "  set prev 0\n"
  "  db eval {\n"
  "    SELECT pageno, pagetype FROM temp.dbstat\n"
  "     WHERE name=$name\n"
  "     ORDER BY pageno\n"
  "  } {\n"
  "    if {$prev>0 && $pagetype==\"leaf\" && $pageno!=$prev+1} {\n"
  "      incr gap_cnt\n"
  "    }\n"
  "    set prev $pageno\n"
  "  }\n"
  "  mem eval {\n"
  "    INSERT INTO space_used VALUES(\n"
  "      $name,\n"
  "      $tblname,\n"
  "      $is_index,\n"
  "      $nentry,\n"
  "      $leaf_entries,\n"
  "      $payload,     \n"
  "      $ovfl_payload,\n"
  "      $ovfl_cnt,   \n"
  "      $mx_payload,\n"
  "      $int_pages,\n"
  "      $leaf_pages,  \n"
  "      $ovfl_pages, \n"
  "      $int_unused, \n"
  "      $leaf_unused,\n"
  "      $ovfl_unused,\n"
  "      $gap_cnt,\n"
  "      $compressed_size\n"
  "    );\n"
  "  }\n"
  "}\n"
  "\n"
  "proc integerify {real} {\n"
  "  if {[string is double -strict $real]} {\n"
  "    return [expr {wide($real)}]\n"
  "  } else {\n"
  "    return 0\n"
  "  }\n"
  "}\n"
  "mem function int integerify\n"
  "\n"
  "# Quote a string for use in an SQL query. Examples:\n"
  "#\n"
  "# [quote {hello world}]   == {'hello world'}\n"
  "# [quote {hello world's}] == {'hello world''s'}\n"
  "#\n"
  "proc quote {txt} {\n"
  "  return [string map {' ''} $txt]\n"
  "}\n"
  "\n"
  "# Output a title line\n"
  "#\n"
  "proc titleline {title} {\n"
  "  if {$title==\"\"} {\n"
  "    puts [string repeat * 79]\n"
  "  } else {\n"
  "    set len [string length $title]\n"
  "    set stars [string repeat * [expr 79-$len-5]]\n"
  "    puts \"*** $title $stars\"\n"
  "  }\n"
  "}\n"
  "\n"
  "# Generate a single line of output in the statistics section of the\n"
  "# report.\n"
  "#\n"
  "proc statline {title value {extra {}}} {\n"
  "  set len [string length $title]\n"
  "  set dots [string repeat . [expr 50-$len]]\n"
  "  set len [string length $value]\n"
  "  set sp2 [string range {          } $len end]\n"
  "  if {$extra ne \"\"} {\n"
  "    set extra \" $extra\"\n"
  "  }\n"
  "  puts \"$title$dots $value$sp2$extra\"\n"
  "}\n"
  "\n"
  "# Generate a formatted percentage value for $num/$denom\n"
  "#\n"
  "proc percent {num denom {of {}}} {\n"
  "  if {$denom==0.0} {return \"\"}\n"
  "  set v [expr {$num*100.0/$denom}]\n"
  "  set of {}\n"
  "  if {$v==100.0 || $v<0.001 || ($v>1.0 && $v<99.0)} {\n"
  "    return [format {%5.1f%% %s} $v $of]\n"
  "  } elseif {$v<0.1 || $v>99.9} {\n"
  "    return [format {%7.3f%% %s} $v $of]\n"
  "  } else {\n"
  "    return [format {%6.2f%% %s} $v $of]\n"
  "  }\n"
  "}\n"
  "\n"
  "proc divide {num denom} {\n"
  "  if {$denom==0} {return 0.0}\n"
  "  return [format %.2f [expr double($num)/double($denom)]]\n"
  "}\n"
  "\n"
  "# Generate a subreport that covers some subset of the database.\n"
  "# the $where clause determines which subset to analyze.\n"
  "#\n"
  "proc subreport {title where showFrag} {\n"
  "  global pageSize file_pgcnt compressOverhead\n"
  "\n"
  "  # Query the in-memory database for the sum of various statistics \n"
  "  # for the subset of tables/indices identified by the WHERE clause in\n"
  "  # $where. Note that even if the WHERE clause matches no rows, the\n"
  "  # following query returns exactly one row (because it is an aggregate).\n"
  "  #\n"
  "  # The results of the query are stored directly by SQLite into local \n"
  "  # variables (i.e. $nentry, $nleaf etc.).\n"
  "  #\n"
  "  mem eval \"\n"
  "    SELECT\n"
  "      int(sum(nentry)) AS nentry,\n"
  "      int(sum(leaf_entries)) AS nleaf,\n"
  "      int(sum(payload)) AS payload,\n"
  "      int(sum(ovfl_payload)) AS ovfl_payload,\n"
  "      max(mx_payload) AS mx_payload,\n"
  "      int(sum(ovfl_cnt)) as ovfl_cnt,\n"
  "      int(sum(leaf_pages)) AS leaf_pages,\n"
  "      int(sum(int_pages)) AS int_pages,\n"
  "      int(sum(ovfl_pages)) AS ovfl_pages,\n"
  "      int(sum(leaf_unused)) AS leaf_unused,\n"
  "      int(sum(int_unused)) AS int_unused,\n"
  "      int(sum(ovfl_unused)) AS ovfl_unused,\n"
  "      int(sum(gap_cnt)) AS gap_cnt,\n"
  "      int(sum(compressed_size)) AS compressed_size\n"
  "    FROM space_used WHERE $where\" {} {}\n"
  "\n"
  "  # Output the sub-report title, nicely decorated with * characters.\n"
  "  #\n"
  "  puts \"\"\n"
  "  titleline $title\n"
  "  puts \"\"\n"
  "\n"
  "  # Calculate statistics and store the results in TCL variables, as follows:\n"
  "  #\n"
  "  # total_pages: Database pages consumed.\n"
  "  # total_pages_percent: Pages consumed as a percentage of the file.\n"
  "  # storage: Bytes consumed.\n"
  "  # payload_percent: Payload bytes used as a percentage of $storage.\n"
  "  # total_unused: Unused bytes on pages.\n"
  "  # avg_payload: Average payload per btree entry.\n"
  "  # avg_fanout: Average fanout for internal pages.\n"
  "  # avg_unused: Average unused bytes per btree entry.\n"
  "  # ovfl_cnt_percent: Percentage of btree entries that use overflow pages.\n"
  "  #\n"
  "  set total_pages [expr {$leaf_pages+$int_pages+$ovfl_pages}]\n"
  "  set total_pages_percent [percent $total_pages $file_pgcnt]\n"
  "  set storage [expr {$total_pages*$pageSize}]\n"
  "  set payload_percent [percent $payload $storage {of storage consumed}]\n"
  "  set total_unused [expr {$ovfl_unused+$int_unused+$leaf_unused}]\n"
  "  set avg_payload [divide $payload $nleaf]\n"
  "  set avg_unused [divide $total_unused $nleaf]\n"
  "  if {$int_pages>0} {\n"
  "    # TODO: Is this formula correct?\n"
  "    set nTab [mem eval \"\n"
  "      SELECT count(*) FROM (\n"
  "          SELECT DISTINCT tblname FROM space_used WHERE $where AND is_index=0\n"
  "      )\n"
  "    \"]\n"
  "    set avg_fanout [mem eval \"\n"
  "      SELECT (sum(leaf_pages+int_pages)-$nTab)/sum(int_pages) FROM space_used\n"
  "          WHERE $where AND is_index = 0\n"
  "    \"]\n"
  "    set avg_fanout [format %.2f $avg_fanout]\n"
  "  }\n"
  "  set ovfl_cnt_percent [percent $ovfl_cnt $nleaf {of all entries}]\n"
  "\n"
  "  # Print out the sub-report statistics.\n"
  "  #\n"
  "  statline {Percentage of total database} $total_pages_percent\n"
  "  statline {Number of entries} $nleaf\n"
  "  statline {Bytes of storage consumed} $storage\n"
  "  if {$compressed_size!=$storage} {\n"
  "    set compressed_size [expr {$compressed_size+$compressOverhead*$total_pages}]\n"
  "    set pct [expr {$compressed_size*100.0/$storage}]\n"
  "    set pct [format {%5.1f%%} $pct]\n"
  "    statline {Bytes used after compression} $compressed_size $pct\n"
  "  }\n"
  "  statline {Bytes of payload} $payload $payload_percent\n"
  "  statline {Average payload per entry} $avg_payload\n"
  "  statline {Average unused bytes per entry} $avg_unused\n"
  "  if {[info exists avg_fanout]} {\n"
  "    statline {Average fanout} $avg_fanout\n"
  "  }\n"
  "  if {$showFrag && $total_pages>1} {\n"
  "    set fragmentation [percent $gap_cnt [expr {$total_pages-1}]]\n"
  "    statline {Non-sequential pages} $gap_cnt $fragmentation\n"
  "  }\n"
  "  statline {Maximum payload per entry} $mx_payload\n"
  "  statline {Entries that use overflow} $ovfl_cnt $ovfl_cnt_percent\n"
  "  if {$int_pages>0} {\n"
  "    statline {Index pages used} $int_pages\n"
  "  }\n"
  "  statline {Primary pages used} $leaf_pages\n"
  "  statline {Overflow pages used} $ovfl_pages\n"
  "  statline {Total pages used} $total_pages\n"
  "  if {$int_unused>0} {\n"
  "    set int_unused_percent [\n"
  "         percent $int_unused [expr {$int_pages*$pageSize}] {of index space}]\n"
  "    statline \"Unused bytes on index pages\" $int_unused $int_unused_percent\n"
  "  }\n"
  "  statline \"Unused bytes on primary pages\" $leaf_unused [\n"
  "     percent $leaf_unused [expr {$leaf_pages*$pageSize}] {of primary space}]\n"
  "  statline \"Unused bytes on overflow pages\" $ovfl_unused [\n"
  "     percent $ovfl_unused [expr {$ovfl_pages*$pageSize}] {of overflow space}]\n"
  "  statline \"Unused bytes on all pages\" $total_unused [\n"
  "               percent $total_unused $storage {of all space}]\n"
  "  return 1\n"
  "}\n"
  "\n"
  "# Calculate the overhead in pages caused by auto-vacuum. \n"
  "#\n"
  "# This procedure calculates and returns the number of pages used by the \n"
  "# auto-vacuum 'pointer-map'. If the database does not support auto-vacuum,\n"
  "# then 0 is returned. The two arguments are the size of the database file in\n"
  "# pages and the page size used by the database (in bytes).\n"
  "proc autovacuum_overhead {filePages pageSize} {\n"
  "\n"
  "  # Set $autovacuum to non-zero for databases that support auto-vacuum.\n"
  "  set autovacuum [db one {PRAGMA auto_vacuum}]\n"
  "\n"
  "  # If the database is not an auto-vacuum database or the file consists\n"
  "  # of one page only then there is no overhead for auto-vacuum. Return zero.\n"
  "  if {0==$autovacuum || $filePages==1} {\n"
  "    return 0\n"
  "  }\n"
  "\n"
  "  # The number of entries on each pointer map page. The layout of the\n"
  "  # database file is one pointer-map page, followed by $ptrsPerPage other\n"
  "  # pages, followed by a pointer-map page etc. The first pointer-map page\n"
  "  # is the second page of the file overall.\n"
  "  set ptrsPerPage [expr double($pageSize/5)]\n"
  "\n"
  "  # Return the number of pointer map pages in the database.\n"
  "  return [expr wide(ceil( ($filePages-1.0)/($ptrsPerPage+1.0) ))]\n"
  "}\n"
  "\n"
  "\n"
  "# Calculate the summary statistics for the database and store the results\n"
  "# in TCL variables. They are output below. Variables are as follows:\n"
  "#\n"
  "# pageSize:      Size of each page in bytes.\n"
  "# file_bytes:    File size in bytes.\n"
  "# file_pgcnt:    Number of pages in the file.\n"
  "# file_pgcnt2:   Number of pages in the file (calculated).\n"
  "# av_pgcnt:      Pages consumed by the auto-vacuum pointer-map.\n"
  "# av_percent:    Percentage of the file consumed by auto-vacuum pointer-map.\n"
  "# inuse_pgcnt:   Data pages in the file.\n"
  "# inuse_percent: Percentage of pages used to store data.\n"
  "# free_pgcnt:    Free pages calculated as (<total pages> - <in-use pages>)\n"
  "# free_pgcnt2:   Free pages in the file according to the file header.\n"
  "# free_percent:  Percentage of file consumed by free pages (calculated).\n"
  "# free_percent2: Percentage of file consumed by free pages (header).\n"
  "# ntable:        Number of tables in the db.\n"
  "# nindex:        Number of indices in the db.\n"
  "# nautoindex:    Number of indices created automatically.\n"
  "# nmanindex:     Number of indices created manually.\n"
  "# user_payload:  Number of bytes of payload in table btrees \n"
  "#                (not including sqlite_master)\n"
  "# user_percent:  $user_payload as a percentage of total file size.\n"
  "\n"
  "### The following, setting $file_bytes based on the actual size of the file\n"
  "### on disk, causes this tool to choke on zipvfs databases. So set it based\n"
  "### on the return of [PRAGMA page_count] instead.\n"
  "if 0 {\n"
  "  set file_bytes  [file size $file_to_analyze]\n"
  "  set file_pgcnt  [expr {$file_bytes/$pageSize}]\n"
  "}\n"
  "set file_pgcnt  [db one {PRAGMA page_count}]\n"
  "set file_bytes  [expr {$file_pgcnt * $pageSize}]\n"
  "\n"
  "set av_pgcnt    [autovacuum_overhead $file_pgcnt $pageSize]\n"
  "set av_percent  [percent $av_pgcnt $file_pgcnt]\n"
  "\n"
  "set sql {SELECT sum(leaf_pages+int_pages+ovfl_pages) FROM space_used}\n"
  "set inuse_pgcnt   [expr wide([mem eval $sql])]\n"
  "set inuse_percent [percent $inuse_pgcnt $file_pgcnt]\n"
  "\n"
  "set free_pgcnt    [expr {$file_pgcnt-$inuse_pgcnt-$av_pgcnt}]\n"
  "set free_percent  [percent $free_pgcnt $file_pgcnt]\n"
  "set free_pgcnt2   [db one {PRAGMA freelist_count}]\n"
  "set free_percent2 [percent $free_pgcnt2 $file_pgcnt]\n"
  "\n"
  "set file_pgcnt2 [expr {$inuse_pgcnt+$free_pgcnt2+$av_pgcnt}]\n"
  "\n"
  "set ntable [db eval {SELECT count(*)+1 FROM sqlite_master WHERE type='table'}]\n"
  "set nindex [db eval {SELECT count(*) FROM sqlite_master WHERE type='index'}]\n"
  "set sql {SELECT count(*) FROM sqlite_master WHERE name LIKE 'sqlite_autoindex%'}\n"
  "set nautoindex [db eval $sql]\n"
  "set nmanindex [expr {$nindex-$nautoindex}]\n"
  "\n"
  "# set total_payload [mem eval \"SELECT sum(payload) FROM space_used\"]\n"
  "set user_payload [mem one {SELECT int(sum(payload)) FROM space_used\n"
  "     WHERE NOT is_index AND name NOT LIKE 'sqlite_master'}]\n"
  "set user_percent [percent $user_payload $file_bytes]\n"
  "\n"
  "# Output the summary statistics calculated above.\n"
  "#\n"
  "puts \"/** Disk-Space Utilization Report For $root_filename\"\n"
  "puts \"\"\n"
  "statline {Page size in bytes} $pageSize\n"
  "statline {Pages in the whole file (measured)} $file_pgcnt\n"
  "statline {Pages in the whole file (calculated)} $file_pgcnt2\n"
  "statline {Pages that store data} $inuse_pgcnt $inuse_percent\n"
  "statline {Pages on the freelist (per header)} $free_pgcnt2 $free_percent2\n"
  "statline {Pages on the freelist (calculated)} $free_pgcnt $free_percent\n"
  "statline {Pages of auto-vacuum overhead} $av_pgcnt $av_percent\n"
  "statline {Number of tables in the database} $ntable\n"
  "statline {Number of indices} $nindex\n"
  "statline {Number of defined indices} $nmanindex\n"
  "statline {Number of implied indices} $nautoindex\n"
  "if {$isCompressed} {\n"
  "  statline {Size of uncompressed content in bytes} $file_bytes\n"
  "  set efficiency [percent $true_file_size $file_bytes]\n"
  "  statline {Size of compressed file on disk} $true_file_size $efficiency\n"
  "} else {\n"
  "  statline {Size of the file in bytes} $file_bytes\n"
  "}\n"
  "statline {Bytes of user payload stored} $user_payload $user_percent\n"
  "\n"
  "# Output table rankings\n"
  "#\n"
  "puts \"\"\n"
  "titleline \"Page counts for all tables with their indices\"\n"
  "puts \"\"\n"
  "mem eval {SELECT tblname, count(*) AS cnt, \n"
  "              int(sum(int_pages+leaf_pages+ovfl_pages)) AS size\n"
  "          FROM space_used GROUP BY tblname ORDER BY size+0 DESC, tblname} {} {\n"
  "  statline [string toupper $tblname] $size [percent $size $file_pgcnt]\n"
  "}\n"
  "puts \"\"\n"
  "titleline \"Page counts for all tables and indices separately\"\n"
  "puts \"\"\n"
  "mem eval {\n"
  "  SELECT\n"
  "       upper(name) AS nm,\n"
  "       int(int_pages+leaf_pages+ovfl_pages) AS size\n"
  "    FROM space_used\n"
  "   ORDER BY size+0 DESC, name} {} {\n"
  "  statline $nm $size [percent $size $file_pgcnt]\n"
  "}\n"
  "if {$isCompressed} {\n"
  "  puts \"\"\n"
  "  titleline \"Bytes of disk space used after compression\"\n"
  "  puts \"\"\n"
  "  set csum 0\n"
  "  mem eval {SELECT tblname,\n"
  "                  int(sum(compressed_size)) +\n"
  "                         $compressOverhead*sum(int_pages+leaf_pages+ovfl_pages)\n"
  "                        AS csize\n"
  "          FROM space_used GROUP BY tblname ORDER BY csize+0 DESC, tblname} {} {\n"
  "    incr csum $csize\n"
  "    statline [string toupper $tblname] $csize [percent $csize $true_file_size]\n"
  "  }\n"
  "  set overhead [expr {$true_file_size - $csum}]\n"
  "  if {$overhead>0} {\n"
  "    statline {Header and free space} $overhead [percent $overhead $true_file_size]\n"
  "  }\n"
  "}\n"
  "\n"
  "# Output subreports\n"
  "#\n"
  "if {$nindex>0} {\n"
  "  subreport {All tables and indices} 1 0\n"
  "}\n"
  "subreport {All tables} {NOT is_index} 0\n"
  "if {$nindex>0} {\n"
  "  subreport {All indices} {is_index} 0\n"
  "}\n"
  "foreach tbl [mem eval {SELECT DISTINCT tblname name FROM space_used\n"
  "                       ORDER BY name}] {\n"
  "  set qn [quote $tbl]\n"
  "  set name [string toupper $tbl]\n"
  "  set n [mem eval {SELECT count(*) FROM space_used WHERE tblname=$tbl}]\n"
  "  if {$n>1} {\n"
  "    set idxlist [mem eval \"SELECT name FROM space_used\n"
  "                            WHERE tblname='$qn' AND is_index\n"
  "                            ORDER BY 1\"]\n"
  "    subreport \"Table $name and all its indices\" \"tblname='$qn'\" 0\n"
  "    subreport \"Table $name w/o any indices\" \"name='$qn'\" 1\n"
  "    if {[llength $idxlist]>1} {\n"
  "      subreport \"Indices of table $name\" \"tblname='$qn' AND is_index\" 0\n"
  "    }\n"
  "    foreach idx $idxlist {\n"
  "      set qidx [quote $idx]\n"
  "      subreport \"Index [string toupper $idx] of table $name\" \"name='$qidx'\" 1\n"
  "    }\n"
  "  } else {\n"
  "    subreport \"Table $name\" \"name='$qn'\" 1\n"
  "  }\n"
  "}\n"
  "\n"
  "# Output instructions on what the numbers above mean.\n"
  "#\n"
  "puts \"\"\n"
  "titleline Definitions\n"
  "puts {\n"
  "Page size in bytes\n"
  "\n"
  "    The number of bytes in a single page of the database file.  \n"
  "    Usually 1024.\n"
  "\n"
  "Number of pages in the whole file\n"
  "}\n"
  "puts \"    The number of $pageSize-byte pages that go into forming the complete\n"
  "    database\"\n"
  "puts {\n"
  "Pages that store data\n"
  "\n"
  "    The number of pages that store data, either as primary B*Tree pages or\n"
  "    as overflow pages.  The number at the right is the data pages divided by\n"
  "    the total number of pages in the file.\n"
  "\n"
  "Pages on the freelist\n"
  "\n"
  "    The number of pages that are not currently in use but are reserved for\n"
  "    future use.  The percentage at the right is the number of freelist pages\n"
  "    divided by the total number of pages in the file.\n"
  "\n"
  "Pages of auto-vacuum overhead\n"
  "\n"
  "    The number of pages that store data used by the database to facilitate\n"
  "    auto-vacuum. This is zero for databases that do not support auto-vacuum.\n"
  "\n"
  "Number of tables in the database\n"
  "\n"
  "    The number of tables in the database, including the SQLITE_MASTER table\n"
  "    used to store schema information.\n"
  "\n"
  "Number of indices\n"
  "\n"
  "    The total number of indices in the database.\n"
  "\n"
  "Number of defined indices\n"
  "\n"
  "    The number of indices created using an explicit CREATE INDEX statement.\n"
  "\n"
  "Number of implied indices\n"
  "\n"
  "    The number of indices used to implement PRIMARY KEY or UNIQUE constraints\n"
  "    on tables.\n"
  "\n"
  "Size of the file in bytes\n"
  "\n"
  "    The total amount of disk space used by the entire database files.\n"
  "\n"
  "Bytes of user payload stored\n"
  "\n"
  "    The total number of bytes of user payload stored in the database. The\n"
  "    schema information in the SQLITE_MASTER table is not counted when\n"
  "    computing this number.  The percentage at the right shows the payload\n"
  "    divided by the total file size.\n"
  "\n"
  "Percentage of total database\n"
  "\n"
  "    The amount of the complete database file that is devoted to storing\n"
  "    information described by this category.\n"
  "\n"
  "Number of entries\n"
  "\n"
  "    The total number of B-Tree key/value pairs stored under this category.\n"
  "\n"
  "Bytes of storage consumed\n"
  "\n"
  "    The total amount of disk space required to store all B-Tree entries\n"
  "    under this category.  The is the total number of pages used times\n"
  "    the pages size.\n"
  "\n"
  "Bytes of payload\n"
  "\n"
  "    The amount of payload stored under this category.  Payload is the data\n"
  "    part of table entries and the key part of index entries.  The percentage\n"
  "    at the right is the bytes of payload divided by the bytes of storage \n"
  "    consumed.\n"
  "\n"
  "Average payload per entry\n"
  "\n"
  "    The average amount of payload on each entry.  This is just the bytes of\n"
  "    payload divided by the number of entries.\n"
  "\n"
  "Average unused bytes per entry\n"
  "\n"
  "    The average amount of free space remaining on all pages under this\n"
  "    category on a per-entry basis.  This is the number of unused bytes on\n"
  "    all pages divided by the number of entries.\n"
  "\n"
  "Non-sequential pages\n"
  "\n"
  "    The number of pages in the table or index that are out of sequence.\n"
  "    Many filesystems are optimized for sequential file access so a small\n"
  "    number of non-sequential pages might result in faster queries,\n"
  "    especially for larger database files that do not fit in the disk cache.\n"
  "    Note that after running VACUUM, the root page of each table or index is\n"
  "    at the beginning of the database file and all other pages are in a\n"
  "    separate part of the database file, resulting in a single non-\n"
  "    sequential page.\n"
  "\n"
  "Maximum payload per entry\n"
  "\n"
  "    The largest payload size of any entry.\n"
  "\n"
  "Entries that use overflow\n"
  "\n"
  "    The number of entries that user one or more overflow pages.\n"
  "\n"
  "Total pages used\n"
  "\n"
  "    This is the number of pages used to hold all information in the current\n"
  "    category.  This is the sum of index, primary, and overflow pages.\n"
  "\n"
  "Index pages used\n"
  "\n"
  "    This is the number of pages in a table B-tree that hold only key (rowid)\n"
  "    information and no data.\n"
  "\n"
  "Primary pages used\n"
  "\n"
  "    This is the number of B-tree pages that hold both key and data.\n"
  "\n"
  "Overflow pages used\n"
  "\n"
  "    The total number of overflow pages used for this category.\n"
  "\n"
  "Unused bytes on index pages\n"
  "\n"
  "    The total number of bytes of unused space on all index pages.  The\n"
  "    percentage at the right is the number of unused bytes divided by the\n"
  "    total number of bytes on index pages.\n"
  "\n"
  "Unused bytes on primary pages\n"
  "\n"
  "    The total number of bytes of unused space on all primary pages.  The\n"
  "    percentage at the right is the number of unused bytes divided by the\n"
  "    total number of bytes on primary pages.\n"
  "\n"
  "Unused bytes on overflow pages\n"
  "\n"
  "    The total number of bytes of unused space on all overflow pages.  The\n"
  "    percentage at the right is the number of unused bytes divided by the\n"
  "    total number of bytes on overflow pages.\n"
  "\n"
  "Unused bytes on all pages\n"
  "\n"
  "    The total number of bytes of unused space on all primary and overflow \n"
  "    pages.  The percentage at the right is the number of unused bytes \n"
  "    divided by the total number of bytes.\n"
  "}\n"
  "\n"
  "# Output a dump of the in-memory database. This can be used for more\n"
  "# complex offline analysis.\n"
  "#\n"
  "titleline {}\n"
  "puts \"The entire text of this report can be sourced into any SQL database\"\n"
  "puts \"engine for further analysis.  All of the text above is an SQL comment.\"\n"
  "puts \"The data used to generate this report follows:\"\n"
  "puts \"*/\"\n"
  "puts \"BEGIN;\"\n"
  "puts $tabledef\n"
  "unset -nocomplain x\n"
  "mem eval {SELECT * FROM space_used} x {\n"
  "  puts -nonewline \"INSERT INTO space_used VALUES\"\n"
  "  set sep (\n"
  "  foreach col $x(*) {\n"
  "    set v $x($col)\n"
  "    if {$v==\"\" || ![string is double $v]} {set v '[quote $v]'}\n"
  "    puts -nonewline $sep$v\n"
  "    set sep ,\n"
  "  }\n"
  "  puts \");\"\n"
  "}\n"
  "puts \"COMMIT;\"\n"
  "\n"
  "} err]} {\n"
  "  puts \"ERROR: $err\"\n"
  "  puts $errorInfo\n"
  "  exit 1\n"
  "}\n"
  "";
  return zMainLoop;
}
