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

  $Id: php_krisp.c,v 1.1.1.1 2006-06-20 07:49:28 oops Exp $
*/

/*
 * PHP5 Korea IPS library module "krsip"
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "php.h"
#include "php_ini.h"
#include "SAPI.h"
#include "ext/standard/info.h"

#include "php_krisp.h"

/* If you declare any globals in php_krisp.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(krisp)
*/

/* True global resources - no need for thread safety here */
static int le_krisp;
char krerr[1024] = { 0, };

/* {{{ krisp_functions[]
 *
 * Every user visible function must have an entry in krisp_functions[].
 */
function_entry krisp_functions[] = {
	PHP_FE(krisp_buildver,			NULL)
	PHP_FE(krisp_version,			NULL)
	PHP_FE(krisp_uversion,			NULL)
	PHP_FE(krisp_open,				NULL)
	PHP_FE(krisp_search,			NULL)
	PHP_FE(krisp_close,				NULL)
	PHP_FE(krisp_error,				NULL)
	{NULL, NULL, NULL}
};
/* }}} */

/* {{{ krisp_module_entry
 */
zend_module_entry krisp_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"krisp",
	krisp_functions,
	PHP_MINIT(krisp),
	NULL,
	NULL,
	NULL,
	PHP_MINFO(krisp),
#if ZEND_MODULE_API_NO >= 20010901
	BUILDNO, /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_KRISP
ZEND_GET_MODULE(krisp)
#endif

static void _close_krisp_link(zend_rsrc_list_entry *rsrc TSRMLS_DC)
{
	KRISP_API *kr = (KRISP_API *)rsrc->ptr;
	kr_close (kr->db);
	efree (kr->db);
	efree (kr);
}

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(krisp)
{
	le_krisp = zend_register_list_destructors_ex(_close_krisp_link, NULL, "krisp link", module_number);
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(krisp)
{
	php_info_print_table_start();
	php_info_print_table_colspan_header(2, "krisp extension support");
	php_info_print_table_row(2, "Summary", "Get Korea ISP Information");
	php_info_print_table_row(2, "URL", "http://devel.oops.org/");
	php_info_print_table_row(2, "Build version", BUILDNO);
	php_info_print_table_row(2, "Library version", KRISP_VERSION);
	php_info_print_table_colspan_header(2, "Library support");
	php_info_print_table_row(2, "SQLite", SQLITE_VERSION);
	php_info_print_table_row(2, "GeoIP",
#ifdef HAVE_LIBGEOIP
			"enabled"
#else
			"disabled"
#endif
	);
	php_info_print_table_end();
}
/* }}} */

/* {{{ proto char krisp_buildver (void)
 *  print krisp extension build number */
PHP_FUNCTION(krisp_buildver)
{
	RETURN_STRING (BUILDNO, 1);
}
/* }}} */

/* {{{ proto char krisp_version (void)
 *  print krisp library version */
PHP_FUNCTION(krisp_version)
{
	RETURN_STRING (KRISP_VERSION, 1);
}
/* }}} */

/* {{{ proto char krisp_uversion (void)
 *  print krisp library uversion */
PHP_FUNCTION(krisp_uversion)
{
	RETURN_STRING (KRISP_UVERSION, 1);
}
/* }}} */

/* {{{ proto resource krisp_open (char datafile)
 *  return krisp database open resource */
PHP_FUNCTION(krisp_open)
{
	zval **datafile;
	KRISP_API *kr;

	struct stat f;
	char *df;
	int r;

	switch (ZEND_NUM_ARGS ()) {
		case 1:
			if ( zend_get_parameters_ex(1, &datafile) == FAILURE )
				WRONG_PARAM_COUNT;

			break;
		default:
				WRONG_PARAM_COUNT;
	}

	if ( Z_STRLEN_PP (datafile) == 0) {
		sprintf (krerr, "length of datafile argument is 0");
		RETURN_FALSE;
	}

	convert_to_string_ex(datafile);
	df = Z_STRVAL_PP(datafile);

	r = stat (df, &f);
	
	if ( r == -1 ) {
		sprintf (krerr, "datafile not found : %s", df);
		RETURN_FALSE;
	}

	if ( f.st_size < 1 ) {
		sprintf (krerr, "datafile size is zero: %s", df);
		RETURN_FALSE;
	}

	kr = (KRISP_API *) emalloc (sizeof (KRISP_API));
	kr->db = (KR_API *) emalloc (sizeof (KR_API));

#ifdef HAVE_LIBGEOIP
	kr->db->gi = GeoIP_new (GEOIP_MEMORY_CACHE);
#else
	kr->db->gi = NULL;
#endif

	if ( kr_open (kr->db, df) ) {
		strcpy (krerr, dberr);
		efree (kr->db);
		efree (kr);
		RETURN_FALSE;
	}

	kr->rsrc = ZEND_REGISTER_RESOURCE(return_value, kr, le_krisp);
}
/* }}} */

/* {{{ proto char krisp_search (struct ispinfo *, KR_API *)
 *  return isp information array */
PHP_FUNCTION(krisp_search)
{
	zval **krisp_link, **host;
	KRISP_API *kr;

	KRNET_API isp;
	char *addr;

	switch (ZEND_NUM_ARGS ()) {
		case 2:
			if ( zend_get_parameters_ex(2, &krisp_link, &host) == FAILURE )
				WRONG_PARAM_COUNT;

			if ( Z_STRLEN_PP (host) == 0) {
				sprintf (krerr, "length of host argument is 0");
				RETURN_FALSE;
			}
			break;
		default:
				WRONG_PARAM_COUNT;
	}

	convert_to_string_ex(host);
	addr = Z_STRVAL_PP(host);

	if ( strlen (addr) > 255 ) {
		strncpy (isp.ip, addr, 255);
		isp.ip[255] = 0;
	} else {
		strcpy (isp.ip, addr);
	}

	ZEND_FETCH_RESOURCE (kr, KRISP_API *, krisp_link, -1, "KRISP database", le_krisp);

	kr_search (&isp, kr->db);

	if ( array_init (return_value) == FAILURE ) {
		sprintf (krerr, "Failure array init");
		RETURN_FALSE;
	}

	add_assoc_string (return_value, "host", addr, 1);
	add_assoc_string (return_value, "ip", isp.ip, 1);
	add_assoc_string (return_value, "key", isp.key, 1);
	add_assoc_string (return_value, "netmask", isp.netmask, 1);
	add_assoc_string (return_value, "network", isp.network, 1);
	add_assoc_string (return_value, "broadcast", isp.broadcast, 1);
	add_assoc_string (return_value, "icode", isp.serv, 1);
	add_assoc_string (return_value, "iname", isp.org, 1);
#ifdef HAVE_LIBGEOIP
	add_assoc_string (return_value, "gcode", isp.code, 1);
	add_assoc_string (return_value, "gname", isp.nation, 1);
#endif
}
/* }}} */

/* {{{ proto char krisp_close (resource)
 *  close krisp database */
PHP_FUNCTION(krisp_close)
{
	zval **krisp_link;
	KRISP_API *kr;

	switch (ZEND_NUM_ARGS ()) {
		case 1:
			if ( zend_get_parameters_ex(1, &krisp_link) == FAILURE )
				WRONG_PARAM_COUNT;

			break;
		default:
				WRONG_PARAM_COUNT;
	}

	ZEND_FETCH_RESOURCE (kr, KRISP_API *, krisp_link, -1, "KRISP database", le_krisp);

	zend_list_delete(Z_RESVAL_PP(krisp_link));
}
/* }}} */

/* {{{ proto char krisp_error (void)
 *  close krisp database */
PHP_FUNCTION(krisp_error)
{
	RETURN_STRING (krerr, 1);
}

/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
