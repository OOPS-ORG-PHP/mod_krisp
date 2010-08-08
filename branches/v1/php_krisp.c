/**
 * Project: mod_krisp :: KRISP database php extension
 * File:    php_krisp.c
 *
 * PHP Version 5
 *
 * Copyright (c) 1997-2010 JoungKyun.Kim
 *
 * LICENSE: LGPL
 *
 * @category    Database
 * @package     mod_krisp
 * @author      JoungKyun.Kim <http://oops.org>
 * @copyright   1997-2010 OOPS.org
 * @license     LGPL
 * @version     CVS: $Id: php_krisp.c,v 1.11.2.1 2010-08-08 16:46:06 oops Exp $
 * @link        http://pear.oops.org/package/krisp
 * @since       File available since release 0.0.1
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
#ifdef HAVE_LIBGEOIP
/* set 1, search GeoIPCity database if enabled search GeoIPCity
 *  * default value is 0 */
extern short geocity;
extern short geocity_type;
extern short geoisp_type;
extern short geo_type;
#endif
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
	free (kr->db);
	free (kr);
}

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(krisp)
{
	le_krisp = zend_register_list_destructors_ex(_close_krisp_link, NULL, "krisp link", module_number);

#ifdef HAVE_LIBGEOIP
	/* define geoip open method */
	REGISTER_LONG_CONSTANT ("GEOIP_STANDARD",     GEOIP_STANDARD, CONST_PERSISTENT | CONST_CS);
	REGISTER_LONG_CONSTANT ("GEOIP_MEMORY_CACHE", GEOIP_MEMORY_CACHE, CONST_PERSISTENT | CONST_CS);
	REGISTER_LONG_CONSTANT ("GEOIP_CHECK_CACHE",  GEOIP_CHECK_CACHE, CONST_PERSISTENT | CONST_CS);
	REGISTER_LONG_CONSTANT ("GEOIP_INDEX_CACHE",  GEOIP_INDEX_CACHE, CONST_PERSISTENT | CONST_CS);

	REGISTER_LONG_CONSTANT ("GEOCITY",          GEOCITY,          CONST_PERSISTENT | CONST_CS);
	REGISTER_LONG_CONSTANT ("GEOIP_OPENTYPE",   GEOIP_OPENTYPE,   CONST_PERSISTENT | CONST_CS);
	REGISTER_LONG_CONSTANT ("GEOISP_OPENTYPE",  GEOISP_OPENTYPE,  CONST_PERSISTENT | CONST_CS);
	REGISTER_LONG_CONSTANT ("GEOCITY_OPENTYPE", GEOCITY_OPENTYPE, CONST_PERSISTENT | CONST_CS);
#endif
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
			"Enabled"
#else
			"Disabled"
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
#ifdef HAVE_LIBGEOIP
	zval **ipoint;
	zval **geoopt;
	zval *geoarr;
	zval **tmp;
	HashPosition pos;
	char *keyn;
	uint keyl;
	ulong nkeyl;
#endif
	KRISP_API *kr;

	struct stat f;
	char *df = NULL;
	int r;

	switch (ZEND_NUM_ARGS ()) {
#ifdef HAVE_LIBGEOIP
		case 2:
			if ( zend_get_parameters_ex(2, &datafile, &geoopt) == FAILURE )
				WRONG_PARAM_COUNT;

			if ( Z_TYPE_PP (geoopt) != IS_ARRAY ) {
				sprintf (krerr, "second argument is not array");
				RETURN_FALSE;
			}

			SEPARATE_ZVAL (geoopt);
			geoarr = *geoopt;
			if ( zend_hash_num_elements (Z_ARRVAL_P(geoarr)) ) {
				zend_hash_internal_pointer_reset_ex (Z_ARRVAL_P(geoarr), &pos);

				while ( zend_hash_get_current_data_ex (Z_ARRVAL_P(geoarr), (void **) &tmp, &pos) == SUCCESS ) {
					if ( (*tmp)->type != IS_LONG )
						continue;

					convert_to_long_ex (tmp)

					switch (zend_hash_get_current_key_ex (Z_ARRVAL_P(geoarr), &keyn, &keyl, &nkeyl, 1, &pos)) {
						case HASH_KEY_IS_STRING:
							//php_printf ("#### [ %s ] => ", keyn);
							if ( ! strcmp ("geocity", keyn) )
								geocity = Z_LVAL_PP(tmp);
							else if ( ! strcmp ("dopen", keyn) )
								geo_type = Z_LVAL_PP(tmp);
							else if ( ! strcmp ("iopen", keyn) )
								geoisp_type = Z_LVAL_PP(tmp);
							else if ( ! strcmp ("copen", keyn) )
								geocity_type = Z_LVAL_PP(tmp);

							break;
						case  HASH_KEY_IS_LONG:
							//php_printf ("#### [ %d ] => ", nkeyl);
							if ( nkeyl == GEOCITY )
								geocity = Z_LVAL_PP(tmp);
							else if ( nkeyl == GEOIP_OPENTYPE )
								geo_type = Z_LVAL_PP(tmp);
							else if ( nkeyl == GEOISP_OPENTYPE )
								geoisp_type = Z_LVAL_PP(tmp);
							else if ( nkeyl == GEOCITY_OPENTYPE )
								geocity_type = Z_LVAL_PP(tmp);

							break;
						default:
							continue;
					}

					//php_printf ("#### %d\n", (*tmp)->value);
					zend_hash_move_forward_ex(Z_ARRVAL_P(geoarr), &pos);
				}
			}
			break;
#endif
		case 1:
			if ( zend_get_parameters_ex(1, &datafile) == FAILURE )
				WRONG_PARAM_COUNT;

			break;
		case 0:
			break;
		default:
				WRONG_PARAM_COUNT;
	}

	if ( ZEND_NUM_ARGS () > 0 && Z_STRLEN_PP (datafile) != 0) {
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
	}

	kr = (KRISP_API *) malloc (sizeof (KRISP_API));
	kr->db = (KR_API *) malloc (sizeof (KR_API));

	if ( kr_open (kr->db, df) ) {
		strcpy (krerr, dberr);
		free (kr->db);
		free (kr);
		RETURN_FALSE;
	}

	kr->rsrc = ZEND_REGISTER_RESOURCE(return_value, kr, le_krisp);
}
/* }}} */

/* {{{ proto char krisp_search (resource, char *host)
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

			break;
		default:
				WRONG_PARAM_COUNT;
	}

	if ( Z_STRLEN_PP (host) == 0) {
		sprintf (krerr, "length of host argument is 0");
		RETURN_FALSE;
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
	add_assoc_string (return_value, "icode", isp.icode, 1);
	add_assoc_string (return_value, "iname", isp.iname, 1);
	add_assoc_string (return_value, "ccode", isp.ccode, 1);
	add_assoc_string (return_value, "cname", isp.cname, 1);
	if ( strlen (isp.city) )
		add_assoc_string (return_value, "city", isp.city, 1);
	if ( strlen (isp.region) )
		add_assoc_string (return_value, "region", isp.region, 1);
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
