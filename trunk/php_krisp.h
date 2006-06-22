/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2006 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.0 of the PHP license,       |
  | that is bundled with this package in the file LICENSE, and is        |
  | available at through the world-wide-web at                           |
  | http://www.php.net/license/3_0.txt.                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: JoungKyun.Kim <http://www.oops.org>                          |
  +----------------------------------------------------------------------+

  $Id: php_krisp.h,v 1.2 2006-06-22 04:40:20 oops Exp $
*/

#ifndef PHP_KRISP_H
#define PHP_KRISP_H

#ifdef PHP_WIN32
#define PHP_KRISP_API __declspec(dllexport)
#else
#define PHP_KRISP_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

extern zend_module_entry krisp_module_entry;
#define krisp_module_ptr &krisp_module_entry

PHP_MINIT_FUNCTION(krisp);
PHP_MSHUTDOWN_FUNCTION(krisp);
PHP_RINIT_FUNCTION(krisp);
PHP_RSHUTDOWN_FUNCTION(krisp);
PHP_MINFO_FUNCTION(krisp);

PHP_FUNCTION(krisp_buildver);
PHP_FUNCTION(krisp_version);
PHP_FUNCTION(krisp_uversion);
PHP_FUNCTION(krisp_open);
PHP_FUNCTION(krisp_close);
PHP_FUNCTION(krisp_search);
PHP_FUNCTION(krisp_error);

/* 
  	Declare any global variables you may need between the BEGIN
	and END macros here:     

ZEND_BEGIN_MODULE_GLOBALS(krisp)
	int   global_value;
	char *global_string;
ZEND_END_MODULE_GLOBALS(krisp)
*/

/* In every utility function you add that needs to use variables 
   in php_krisp_globals, call TSRM_FETCH(); after declaring other 
   variables used by that function, or better yet, pass in TSRMG_CC
   after the last function argument and declare your utility function
   with TSRMG_DC after the last declared argument.  Always refer to
   the globals in your function as KRISP_G(variable).  You are 
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/

#ifdef ZTS
#define KRISP_G(v) TSRMG(krisp_globals_id, zend_krisp_globals *, v)
#else
#define KRISP_G(v) (krisp_globals.v)
#endif

#define BUILDNO "1.0.0"

#define phpext_krisp_ptr krisp_module_ptr


/*
 * KRISP library header
 */

#include <krversion.h>
extern char dberr[1024];

#if defined(HAVE_LIBSQLITE3)
#include <sqlite3.h>
#else
#include <sqlite.h>
#endif

/* GeoIP extension start */
#ifdef HAVE_LIBGEOIP
#include <GeoIP.h>
#define INCLUDE_GEOIP_HEADER_OK
#endif

#ifndef INCLUDE_GEOIP_HEADER_OK
typedef struct GeoIPTag {
	FILE *GeoIPDatabase;
	char *file_path;
	unsigned char *cache;
	unsigned char *index_cache;
	unsigned int *databaseSegments;
	char databaseType;
	time_t mtime;
	int flags;
	char record_length;
	int record_iter; /* used in GeoIP_next_record */
} GeoIP;

#define GEOIP_API
GEOIP_API GeoIP* GeoIP_new(int flags);

typedef enum {
	GEOIP_STANDARD = 0,
	GEOIP_MEMORY_CACHE = 1,
	GEOIP_CHECK_CACHE = 2,
	GEOIP_INDEX_CACHE = 4,
} GeoIPOptions;
#endif
/* GeoIP extension end */

typedef struct db_argument {
#if defined(HAVE_LIBSQLITE3)
	sqlite3         *c;     /* db resource */
	sqlite3_stmt    *vm;    /* sqlite vm */
#else
	sqlite          *c;     /* db resource */
	sqlite_vm       *vm;    /* sqlite vm */
#endif
	int             r;      /* execute result code */
	int             rows;   /* vm rows */
	int             cols;   /* number of columns */
	char *          err;    /* vm error message */
	const char **   rowdata;
	const char **   colname;
	GeoIP *         gi;     /* GeoIP resource */
} KR_API;

typedef struct netinfos {
	char            key[16];
	char            ip[256];
	char            netmask[16];
	char            network[16];
	char            broadcast[16];
	char            icode[128];
	char            iname[128];
#ifdef HAVE_LIBGEOIP
	char            gcode[4];
	char            gname[128];
#endif
} KRNET_API;

typedef struct krisp_info {
	KR_API *db;
	int rsrc;
} KRISP_API;

#endif	/* PHP_KRISP_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
