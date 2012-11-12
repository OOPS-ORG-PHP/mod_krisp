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
  | Author: JoungKyun.Kim <http://oops.org>                              |
  +----------------------------------------------------------------------+

  $Id: php_krisp.c,v 1.15 2010-07-02 18:46:34 oops Exp $
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
#include <unistd.h>

#include "php.h"
#include "php_ini.h"
#include "SAPI.h"
#include "ext/standard/info.h"

#include "php_krisp.h"

/* If you declare any globals in php_krisp.h uncomment this:
 */
ZEND_DECLARE_MODULE_GLOBALS(krisp)
static PHP_GINIT_FUNCTION(krisp);

/* True global resources - no need for thread safety here */
static int le_krisp;

/* {{{ krisp_functions[]
 *
 * Every user visible function must have an entry in krisp_functions[].
 */
function_entry krisp_functions[] = {
	PHP_FE(krisp_buildver,			NULL)
	PHP_FE(krisp_version,			NULL)
	PHP_FE(krisp_uversion,			NULL)
	PHP_FE(krisp_open,				second_arg_force_ref)
	PHP_FE(krisp_search,			NULL)
	PHP_FE(krisp_search_ex,			NULL)
	PHP_FE(krisp_close,				NULL)
	PHP_FE(krisp_netmask,			NULL)
	PHP_FE(krisp_network,			NULL)
	PHP_FE(krisp_broadcast,			NULL)
	PHP_FE(krisp_prefix2mask,		NULL)
	PHP_FE(krisp_mask2prefix,		NULL)
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
	PHP_MODULE_GLOBALS(krisp),
	PHP_GINIT(krisp),
	NULL,
	NULL,
	STANDARD_MODULE_PROPERTIES_EX
};
/* }}} */

#ifdef COMPILE_DL_KRISP
ZEND_GET_MODULE(krisp)
#endif

/* {{{ PHP_GINIT_FUNCTION
 */
static PHP_GINIT_FUNCTION(krisp)
{
	memset (krisp_globals->err, 0, 1024);
}
/* }}} */

static void _close_krisp_link(zend_rsrc_list_entry *rsrc TSRMLS_DC)
{
	KRISP_API *kr = (KRISP_API *)rsrc->ptr;
	kr_close (kr->db);
	free (kr);
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
	php_info_print_table_end();
}
/* }}} */

/* {{{ proto string krisp_buildver (void)
 *  print krisp extension build number */
PHP_FUNCTION(krisp_buildver)
{
	RETURN_STRING (BUILDNO, 1);
}
/* }}} */

/* {{{ proto string krisp_version (void)
 *  print krisp library version */
PHP_FUNCTION(krisp_version)
{
	RETURN_STRING (KRISP_VERSION, 1);
}
/* }}} */

/* {{{ proto string krisp_uversion (void)
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
	zval **datafile = NULL;
	zval **error = NULL;
	KRISP_API *kr;

	char *df = NULL;
	char err[1024];

	switch (ZEND_NUM_ARGS ()) {
		case 2:
			if ( zend_get_parameters_ex(2, &datafile, &error) == FAILURE )
				WRONG_PARAM_COUNT;

			break;
		case 1:
			if ( zend_get_parameters_ex(1, &datafile) == FAILURE )
				WRONG_PARAM_COUNT;

			break;
		case 0:
			break;
		default:
				WRONG_PARAM_COUNT;
	}

	if ( ZEND_NUM_ARGS () > 0 ) {
		convert_to_string_ex(datafile);
		df = Z_STRVAL_PP(datafile);

		if ( strlen (df) < 1 )
			df = NULL;
	}

	kr = (KRISP_API *) malloc (sizeof (KRISP_API));

	if ( kr_open_safe (&kr->db, df, err) == false ) {
		php_error_docref (NULL TSRMLS_CC, E_WARNING, "%s", err);
		if ( ZEND_NUM_ARGS () == 2 ) {
			zval_dtor (*error);
			ZVAL_STRING (*error, err, 1);
		}
		free (kr);
		RETURN_FALSE;
	}

	kr->rsrc = ZEND_REGISTER_RESOURCE(return_value, kr, le_krisp);
}
/* }}} */

/* {{{ proto object krisp_search (resource, char *host)
 *  return isp information array */
PHP_FUNCTION(krisp_search)
{
	zval **krisp_link, **host;
	KRISP_API *kr;

	KRNET_API isp;
	char rip[16];
	char *addr;
	ulong network;
	ulong broadcast;

	switch (ZEND_NUM_ARGS ()) {
		case 2:
			if ( zend_get_parameters_ex(2, &krisp_link, &host) == FAILURE )
				WRONG_PARAM_COUNT;

			break;
		default:
				WRONG_PARAM_COUNT;
	}

	if ( Z_STRLEN_PP (host) == 0) {
		php_error_docref (NULL TSRMLS_CC, E_WARNING, "length of host argument is 0");
		RETURN_FALSE;
	}

	convert_to_string_ex(host);
	addr = Z_STRVAL_PP(host);
	SAFECPY_256 (isp.ip, addr);
	isp.verbose = false;

	ZEND_FETCH_RESOURCE (kr, KRISP_API *, krisp_link, -1, "KRISP database", le_krisp);

	if ( kr_search (&isp, kr->db) ) {
		php_error_docref (NULL TSRMLS_CC, E_WARNING, "%s", isp.err);
		RETURN_FALSE;
	}

	if ( object_init (return_value) == FAILURE ) {
		php_error_docref (NULL TSRMLS_CC, E_WARNING, "Failure object initialize");
		RETURN_FALSE;
	}

	network = kr_network (isp.start, isp.netmask);
	broadcast = kr_broadcast (isp.start, isp.netmask);

	add_property_string (return_value, "host", addr, 1);
	add_property_string (return_value, "ip", isp.ip, 1);
	add_property_string (return_value, "start", kr_long2ip_r (isp.start, rip), 1);
	add_property_string (return_value, "end", kr_long2ip_r (isp.end, rip), 1);
	add_property_string (return_value, "netmask", kr_long2ip_r (isp.netmask, rip), 1);
	add_property_string (return_value, "network", kr_long2ip_r (network, rip), 1);
	add_property_string (return_value, "broadcast", kr_long2ip_r (broadcast, rip), 1);
	add_property_string (return_value, "icode", isp.icode, 1);
	add_property_string (return_value, "iname", isp.iname, 1);
	add_property_string (return_value, "ccode", isp.ccode, 1);
	add_property_string (return_value, "cname", isp.cname, 1);
}
/* }}} */

/* {{{ proto object krisp_search_ex (resource, char *host)
 *  return isp information array */
PHP_FUNCTION(krisp_search_ex)
{
	zval **krisp_link, **host, **tablename;
	KRISP_API *kr;

	KRNET_API_EX isp;
	char rip[16];
	zval *dummy = NULL;
	char *addr;
	char *table;
	ulong netmask;
	ulong network;
	ulong broadcast;

	switch (ZEND_NUM_ARGS ()) {
		case 3:
			if ( zend_get_parameters_ex(3, &krisp_link, &host, &tablename) == FAILURE )
				WRONG_PARAM_COUNT;

			convert_to_string_ex (tablename);
			table = Z_STRVAL_PP (tablename);
			break;
		case 2:
			if ( zend_get_parameters_ex(2, &krisp_link, &host) == FAILURE )
				WRONG_PARAM_COUNT;

			table = "krisp";
			break;
		default:
				WRONG_PARAM_COUNT;
	}

	if ( Z_STRLEN_PP (host) == 0) {
		php_error_docref (NULL TSRMLS_CC, E_WARNING, "length of host argument is 0");
		RETURN_FALSE;
	}

	convert_to_string_ex(host);
	addr = Z_STRVAL_PP(host);
	SAFECPY_256 (isp.ip, addr);
	isp.verbose = false;

	ZEND_FETCH_RESOURCE (kr, KRISP_API *, krisp_link, -1, "KRISP database", le_krisp);
	kr->db->table = table;

	if ( kr_search_ex (&isp, kr->db) ) {
		php_error_docref (NULL TSRMLS_CC, E_WARNING, "%s", isp.err);
		initStruct_ex (&isp, true);
		RETURN_FALSE;
	}

	if ( object_init (return_value) == FAILURE ) {
		initStruct_ex (&isp, true);
		php_error_docref (NULL TSRMLS_CC, E_WARNING, "Failure object initialize");
		RETURN_FALSE;
	}

	MAKE_STD_ZVAL(dummy);
	if ( array_init (dummy) == FAILURE ) {
		initStruct_ex (&isp, true);
		php_error_docref (NULL TSRMLS_CC, E_WARNING, "Failure array initialize");
		RETURN_FALSE;
	}

	netmask = kr_netmask (isp.start, isp.end);
	network = kr_network (isp.start, netmask);
	broadcast = kr_broadcast (isp.start, netmask);

	add_property_string (return_value, "host", addr, 1);
	add_property_string (return_value, "ip", isp.ip, 1);
	add_property_string (return_value, "start", kr_long2ip_r (isp.start, rip), 1);
	add_property_string (return_value, "end", kr_long2ip_r (isp.end, rip), 1);
	add_property_string (return_value, "netmask", kr_long2ip_r (netmask, rip), 1);
	add_property_string (return_value, "network", kr_long2ip_r (network, rip), 1);
	add_property_string (return_value, "broadcast", kr_long2ip_r (broadcast, rip), 1);
	add_property_long (return_value, "size", isp.size);

	{
		short r;

		for ( r=0; r<isp.size; r++ )
			add_index_string (dummy, r, isp.dummy[r], 1);
	}

	add_property_zval (return_value, "data", dummy);
	initStruct_ex (&isp, true);
}
/* }}} */

/* {{{ proto void krisp_close (resource)
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

/* {{{ proto object krisp_netmask (start, end)
 *  return netmask and prefix about given ip range */
PHP_FUNCTION(krisp_netmask)
{
	zval **startip, **endip;
	char rip[16];
	char *start;
	char *end;
	ulong lstart;
	ulong lend;
	ulong mask;

	switch (ZEND_NUM_ARGS ()) {
		case 2:
			if ( zend_get_parameters_ex(2, &startip, &endip) == FAILURE )
				WRONG_PARAM_COUNT;

			break;
		default:
				WRONG_PARAM_COUNT;
	}

	if ( Z_STRLEN_PP (startip) == 0) {
		php_error_docref (NULL TSRMLS_CC, E_WARNING, "length that start address is 0");
		RETURN_FALSE;
	}

	if ( Z_STRLEN_PP (endip) == 0) {
		php_error_docref (NULL TSRMLS_CC, E_WARNING, "length that end address is 0");
		RETURN_FALSE;
	}

	convert_to_string_ex (startip);
	start = Z_STRVAL_PP(startip);

	convert_to_string_ex (endip);
	end = Z_STRVAL_PP(endip);

	if ( strchr (start, '.') == NULL )
		lstart = (ulong) strtoul (start, NULL, 10);
	else
		lstart = kr_ip2long (start);

	if ( strchr (end, '.') == NULL )
		lend = (ulong) strtoul (end, NULL, 10);
	else
		lend = kr_ip2long (end);

	if ( object_init (return_value) == FAILURE ) {
		php_error_docref (NULL TSRMLS_CC, E_WARNING, "Failure object initialize");
		RETURN_FALSE;
	}

	mask = kr_netmask (lstart, lend);
	add_property_string (return_value, "mask", kr_long2ip_r (mask, rip), 1);
	add_property_long (return_value, "prefix", kr_long2prefix (mask));
}
/* }}} */

static void krisp_network_broadcast (INTERNAL_FUNCTION_PARAMETERS, zend_bool type) // {{{
{
	zval **gip, **gmask;
	char *ip;
	char *mask;
	ulong lip;
	ulong lmask;
	char rip[16];

	switch (ZEND_NUM_ARGS ()) {
		case 2:
			if ( zend_get_parameters_ex(2, &gip, &gmask) == FAILURE )
				WRONG_PARAM_COUNT;

			break;
		default:
				WRONG_PARAM_COUNT;
	}

	if ( Z_STRLEN_PP (gip) == 0) {
		php_error_docref (NULL TSRMLS_CC, E_WARNING, "length that IP address is 0");
		RETURN_FALSE;
	}

	if ( Z_STRLEN_PP (gmask) == 0) {
		php_error_docref (NULL TSRMLS_CC, E_WARNING, "length of network mask is 0");
		RETURN_FALSE;
	}

	convert_to_string_ex (gip);
	ip = Z_STRVAL_PP(gip);

	convert_to_string_ex (gmask);
	mask = Z_STRVAL_PP(gmask);

	if ( strchr (ip, '.') == NULL )
		lip = (ulong) strtoul (ip, NULL, 10);
	else
		lip = kr_ip2long (ip);

	if ( strchr (mask, '.') == NULL )
		lmask = (ulong) strtoul (mask, NULL, 10);
	else
		lmask = kr_ip2long (mask);

	if ( type ) {
		RETURN_STRING (kr_long2ip_r (kr_broadcast (lip, lmask), rip), 1);
	} else {
		RETURN_STRING (kr_long2ip_r (kr_network (lip, lmask), rip), 1);
	}
} // }}}

/* {{{ proto string krisp_network (ip, mask)
 *  reuturn network address about given ip and network mask */
PHP_FUNCTION(krisp_network)
{
	krisp_network_broadcast (INTERNAL_FUNCTION_PARAM_PASSTHRU, 0);
}
/* }}} */

/* {{{ proto string krisp_broadcast (ip, mask)
 *  reuturn broadcast address about given ip and network mask */
PHP_FUNCTION(krisp_broadcast)
{
	krisp_network_broadcast (INTERNAL_FUNCTION_PARAM_PASSTHRU, 1);
}
/* }}} */

/* {{{ proto string krisp_prefix2mask (prefix)
 *  return unsigned long value for given network prefix */
PHP_FUNCTION(krisp_prefix2mask)
{
	zval **prefix_digit;
	short prefix;
	char rip[16];

	switch (ZEND_NUM_ARGS ()) {
		case 1:
			if ( zend_get_parameters_ex(1, &prefix_digit) == FAILURE )
				WRONG_PARAM_COUNT;

			break;
		default:
				WRONG_PARAM_COUNT;
	}

	convert_to_long_ex (prefix_digit);
	prefix = Z_LVAL_PP(prefix_digit);

	RETURN_STRING (kr_long2ip_r (kr_prefix2long (prefix), rip), 1);
}
/* }}} */

/* {{{ proto short krisp_mask2prefix (mask)
 *  return short network prefix for given long network mask */
PHP_FUNCTION(krisp_mask2prefix)
{
	zval **mask_z;
	char * mask;

	switch (ZEND_NUM_ARGS ()) {
		case 1:
			if ( zend_get_parameters_ex(1, &mask_z) == FAILURE )
				WRONG_PARAM_COUNT;

			break;
		default:
				WRONG_PARAM_COUNT;
	}

	convert_to_string_ex (mask_z);
	mask = Z_STRVAL_PP(mask_z);

	RETURN_LONG (kr_long2prefix (kr_ip2long (mask)));
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
