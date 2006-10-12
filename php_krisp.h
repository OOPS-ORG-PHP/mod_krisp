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

  $Id: php_krisp.h,v 1.9 2006-10-12 17:08:39 oops Exp $
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

#define BUILDNO "1.1.3"

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
#include <GeoIPCity.h>
#define INCLUDE_GEOIP_HEADER_OK

#define GEOCITY				0
#define	GEOIP_OPENTYPE		1
#define GEOISP_OPENTYPE		2
#define GEOCITY_OPENTYPE	3
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

typedef enum {
	GEOIP_STANDARD = 0,
	GEOIP_MEMORY_CACHE = 1,
	GEOIP_CHECK_CACHE = 2,
	GEOIP_INDEX_CACHE = 4,
} GeoIPOptions;

typedef enum {
	GEOIP_COUNTRY_EDITION     = 1,
	GEOIP_REGION_EDITION_REV0 = 7,
	GEOIP_CITY_EDITION_REV0   = 6,
	GEOIP_ORG_EDITION         = 5,
	GEOIP_ISP_EDITION         = 4,
	GEOIP_CITY_EDITION_REV1   = 2,
	GEOIP_REGION_EDITION_REV1 = 3,
	GEOIP_PROXY_EDITION       = 8,
	GEOIP_ASNUM_EDITION       = 9,
	GEOIP_NETSPEED_EDITION    = 10,
	GEOIP_DOMAIN_EDITION      = 11
} GeoIPDBTypes;

typedef struct GeoIPRecordTag {
	char *country_code;
	char *country_code3;
	char *country_name;
	char *region;
	char *city;
	char *postal_code;
	float latitude;
	float longitude;
	int dma_code;
	int area_code;
} GeoIPRecord;

#define GEOIP_API
GEOIP_API GeoIP* GeoIP_new(int flags);
GEOIP_API void GeoIP_delete(GeoIP* gi);
GEOIP_API int GeoIP_id_by_name (GeoIP* gi, const char *host);
GEOIP_API int GeoIP_db_avail(int type);
GEOIP_API char *GeoIP_org_by_name (GeoIP* gi, const char *host);
GeoIPRecord * GeoIP_record_by_name (GeoIP* gi, const char *host);
void _GeoIP_setup_dbfilename (void);
extern const char GeoIP_country_code[247][3];
extern const char * GeoIP_country_name[247];
#endif
/* GeoIP extension end */

typedef struct GeoIPvarTag {
	GeoIP *         gid;
	GeoIP *         gic;
	GeoIP *         gip;
} GeoIPvar;

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
	GeoIPvar *      gi;     /* GeoIP resource */
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
	char            gcity[64];
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
