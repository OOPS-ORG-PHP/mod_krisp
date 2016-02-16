/**
 * Project: mod_krisp :: KRISP database php extension
 * File:    krisp.c
 *
 * Copyright (c) 2015 JoungKyun.Kim
 *
 * LICENSE: GPL
 *
 * @category    Database
 * @package     mod_krisp
 * @author      JoungKyun.Kim <http://oops.org>
 * @copyright   2015 OOPS.org
 * @license     GPL
 * @version     CVS: $Id$
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
#include <unistd.h>

#include "php.h"
#include "php_ini.h"
#include "SAPI.h"
#include "ext/standard/info.h"

#include "zend_exceptions.h"
#include "zend_interfaces.h"

#include "php_krisp.h"

#if PHP_API_VERSION < 20151012
#error "************ PHP version dependency problems *******************"
#error "This package requires over php 7.0.0 !!"
#error "If you build with php under 7.0.0, use mod_krisp 2.x version"
#error "You can download mod_krisp 2.x at http://mirror.oops.org/pub/oops/libkrisp/APIs/php/"
#endif

/* If you declare any globals in php_krisp.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(krisp)
 */

/* True global resources - no need for thread safety here */
static int le_krisp;

ZEND_BEGIN_ARG_INFO_EX(arginfo_krisp_open, 0, 0, 0)
	ZEND_ARG_INFO(0, database)
	ZEND_ARG_INFO(1, error)
ZEND_END_ARG_INFO()

#include "php_krisp_class.h"

/* {{{ krisp_functions[]
 *
 * Every user visible function must have an entry in krisp_functions[].
 */
const zend_function_entry krisp_functions[] = {
	PHP_FE(krisp_buildver,           NULL)
	PHP_FE(krisp_version,            NULL)
	PHP_FE(krisp_uversion,           NULL)
	PHP_FE(krisp_open,               arginfo_krisp_open)
	PHP_FE(krisp_search,             NULL)
	PHP_FE(krisp_search_ex,          NULL)
	PHP_FE(krisp_close,              NULL)
	PHP_FE(krisp_netmask,            NULL)
	PHP_FE(krisp_network,            NULL)
	PHP_FE(krisp_broadcast,          NULL)
	PHP_FE(krisp_prefix2mask,        NULL)
	PHP_FE(krisp_mask2prefix,        NULL)
	PHP_FE(krisp_set_mtime_interval, NULL)
	PHP_FE(krisp_set_debug,          NULL)
	PHP_FE_END
};
/* }}} */

/* {{{ krisp_module_entry
 */
zend_module_entry krisp_module_entry = {
#if ZEND_MODULE_API_NO >= 20050922
	STANDARD_MODULE_HEADER_EX,
	NULL,
	krisp_deps,
#elif ZEND_MODULE_API_NO >= 20010901
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

static void _close_krisp_link(zend_resource * res)
{
	PRINT_CALL_API_NAME;

	if ( ! res )
		return;

	//kr_printf ("GC_REFCOUNT(res)              --> %d\n", GC_REFCOUNT(res));
	kr_printf ("res                            --> %d\n", res);
	if ( res->ptr ) {
		KRISP_API * kr = (KRISP_API *) res->ptr;


		kr_printf ("kr handler    -------------------> %d\n", kr->handler);
		if ( kr->handler != NULL )
			kr_close (&kr->handler);
		kr_printf ("kr handler    -------------------> %d\n", kr->handler);

		kr_printf ("kr befer free -------------------> %d\n", kr);
		kr_safe_efree (kr);
		kr_printf ("kr after free -------------------> %d\n", kr);
	}
}

/* {{{ INCLUDE KRISP Classify API */
#include "php_krisp_class.c"
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(krisp)
{
	zend_class_entry ce;
	zend_class_entry cex;
	le_krisp = zend_register_list_destructors_ex (_close_krisp_link, NULL, "krisp link", module_number);

	/* Register KRISP Class */
	memcpy(&krisp_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	INIT_CLASS_ENTRY (ce, "KRISP", krisp_methods);
	ce.create_object = krisp_object_new;
	krisp_object_handlers.offset = XtOffsetOf(KROBJ, std);
	krisp_object_handlers.clone_obj = NULL;
	krisp_object_handlers.free_obj = (zend_object_free_obj_t) krisp_object_free_storage;
	krisp_ce = zend_register_internal_class(&ce);
	ce.ce_flags |= ZEND_ACC_FINAL;

	/* Register KRISP Exception Class */
	memcpy(&krisp_object_handlers_exception, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	INIT_CLASS_ENTRY(cex, "KRISPException", krisp_methods_exception);
#if defined(HAVE_SPL)
	krisp_ce_exception = zend_register_internal_class_ex(&cex, spl_ce_RuntimeException);
#else
	krisp_ce_exception = zend_register_internal_class_ex(&cex, zend_ce_exception);
#endif
	cex.ce_flags |= ZEND_ACC_FINAL;

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(krisp)
{
	php_info_print_table_start ();
	php_info_print_table_colspan_header (2, "krisp extension support");
	php_info_print_table_row (2, "Summary", "Get Korea ISP Information");
	php_info_print_table_row (2, "URL", "http://devel.oops.org/");
	php_info_print_table_row (2, "Build version", BUILDNO);
	php_info_print_table_row (2, "Library version", KRISP_VERSION);
	php_info_print_table_colspan_header (2, "Library support");
	php_info_print_table_row (2, "SQLite", SQLITE_VERSION);
	php_info_print_table_end ();
}
/* }}} */

/* {{{ proto (string) krisp_buildver (void)
 *  print krisp extension build number */
PHP_FUNCTION(krisp_buildver)
{
	RETURN_STRING (BUILDNO);
}
/* }}} */

/* {{{ proto (string) krisp_version (void)
 *  print krisp library version */
PHP_FUNCTION(krisp_version)
{
	RETURN_STRING (KRISP_VERSION);
}
/* }}} */

/* {{{ proto (string) krisp_uversion (void)
 *  print krisp library uversion */
PHP_FUNCTION(krisp_uversion)
{
	RETURN_STRING (KRISP_UVERSION);
}
/* }}} */

/* {{{ proto (resource) krisp_open (string datafile)
 *  return krisp database open resource */
PHP_FUNCTION(krisp_open)
{
	zend_string       * database = NULL;
	zval              * error = NULL;
	KRISP_API         * kr = NULL;
	KROBJ             * obj;
	char              * db;
	char                err[1024];

	zval              * object = getThis ();
	zend_error_handling error_handling;

	PRINT_CALL_API_NAME;

	if ( object ) {
		KRISP_REPLACE_ERROR_HANDLING;
		obj = Z_KRISP_P (object);
	}

	if ( krisp_parameters ("|Sz/", &database, &error) == FAILURE ) {
		return;
	}

	if ( database )
		db = ZSTR_LEN (database) ? ZSTR_VAL (database) : NULL;
	else
		db = NULL;

	if ( db != NULL ) {
		if ( php_check_open_basedir (db) ) {
			RETURN_FALSE;
		}
	}

	kr = (KRISP_API *) emalloc (sizeof (KRISP_API));
	kr_printf ("kr        ------------------------> %d\n", kr);

	if ( object ) {
		obj->db = kr;
		kr_printf ("obj->db   ------------------------> %d\n", obj->db);
	}

	if ( kr_open_safe (&kr->handler, db, err) == false ) {
		php_error_docref (NULL, E_WARNING, "%s", err);
		if ( error ) {
			zval_dtor (error);
			ZVAL_STRING (error, err);
		}
		kr_safe_efree (kr);
		RETURN_FALSE;
	}

	kr->rsrc = zend_register_resource (kr, le_krisp);
	kr_printf ("kr->rsrc  ------------------------> %d\n", kr->rsrc);
	if ( ! object ) {
		RETVAL_RES (kr->rsrc);
	}
}
/* }}} */

/* {{{ proto (object|false) krisp_search (resource link, string host)
 *  return isp information array */
PHP_FUNCTION(krisp_search)
{
	zval              * krisp_link;
	zend_string       * host;
	KRISP_API         * kr;

	KRNET_API           isp;
	char                rip[16];
	ulong               networkv;
	ulong               broadcastv;

	zval              * object = getThis ();
	KROBJ             * obj;
	zend_error_handling error_handling;

	PRINT_CALL_API_NAME;

	if ( object ) {
		KRISP_REPLACE_ERROR_HANDLING;
		if ( krisp_parameters ("S", &host) == FAILURE) {
			return;
		}
	} else {
		if ( krisp_parameters ("rS", &krisp_link, &host) == FAILURE) {
			return;
		}
	}

	if ( ZSTR_LEN (host) == 0 ) {
		php_error_docref (NULL, E_WARNING, "Expects the value of host");
		RETURN_FALSE;
	}

	if ( object ) {
		obj = Z_KRISP_P (object);
		kr_printf ("obj->db   ------------------------> %d\n", obj->db);

		kr = obj->db;
		if ( ! kr || kr->handler == NULL ) {
			if ( ! kr )
				kr = NULL;
			php_error_docref (NULL, E_WARNING, "No KRISP object available");
			RETURN_FALSE;
		}
	} else
		KR_FETCH_RESOURCE (kr, KRISP_API *, krisp_link, "KRISP database", le_krisp);

	SAFECPY_256 (isp.ip, ZSTR_VAL (host));
	isp.verbose = kr->handler->verbose;

	if ( kr_search (&isp, kr->handler) ) {
		php_error_docref (NULL, E_WARNING, "%s", isp.err);
		RETURN_FALSE;
	}

	if ( object_init (return_value) == FAILURE ) {
		php_error_docref (NULL, E_WARNING, "Failure object initialize");
		RETURN_FALSE;
	}

	networkv = network (isp.start, isp.netmask);
	broadcastv = broadcast (isp.start, isp.netmask);

	add_property_string (return_value, "host", ZSTR_VAL(host));
	add_property_string (return_value, "ip", isp.ip);
	add_property_string (return_value, "start", long2ip_r (isp.start, rip));
	add_property_string (return_value, "end", long2ip_r (isp.end, rip));
	add_property_string (return_value, "netmask", long2ip_r (isp.netmask, rip));
	add_property_string (return_value, "network", long2ip_r (networkv, rip));
	add_property_string (return_value, "broadcast", long2ip_r (broadcastv, rip));
	add_property_string (return_value, "icode", isp.icode);
	add_property_string (return_value, "iname", isp.iname);
	add_property_string (return_value, "ccode", isp.ccode);
	add_property_string (return_value, "cname", isp.cname);
}
/* }}} */

/* {{{ proto (object|false) krisp_search_ex (resource link, string host)
 *  return isp information array */
PHP_FUNCTION(krisp_search_ex)
{
	zval              * krisp_link;
	zend_string       * host;
	zend_string       * table = NULL;
	KRISP_API         * kr;

	KRNET_API_EX        isp;
	char                rip[16];
	zval                dummy;
	ulong               netmask;
	ulong               networkv;
	ulong               broadcastv;
	char              * otable;

	zval              * object = getThis ();
	KROBJ             * obj;
	zend_error_handling error_handling;

	PRINT_CALL_API_NAME;

	if ( object ) {
		KRISP_REPLACE_ERROR_HANDLING;
		if ( krisp_parameters ("S|S", &host, &table) == FAILURE) {
			return;
		}

	} else {
		if ( krisp_parameters ("rS|S", &krisp_link, &host, &table) == FAILURE) {
			return;
		}
	}

	if ( ! table )
		otable = "krisp";
	else
		otable = ZSTR_LEN (table) ? ZSTR_VAL (table) : "krisp";

	if ( ZSTR_LEN (host) == 0) {
		php_error_docref (NULL, E_WARNING, "Expects the value of host");
		RETURN_FALSE;
	}

	if ( object ) {
		obj = Z_KRISP_P (object);
		kr = obj->db;
		if ( ! kr || kr->handler == NULL ) {
			php_error_docref (NULL, E_WARNING, "No KRISP object available");
			RETURN_FALSE;
		}
	} else
		KR_FETCH_RESOURCE (kr, KRISP_API *, krisp_link, "KRISP database", le_krisp);

	SAFECPY_256 (isp.ip, ZSTR_VAL (host));
	isp.verbose = kr->handler->verbose;
	kr->handler->table = otable;

	if ( kr_search_ex (&isp, kr->handler) ) {
		php_error_docref (NULL, E_WARNING, isp.err);
		initStruct_ex (&isp, true);
		RETURN_FALSE;
	}

	if ( object_init (return_value) == FAILURE ) {
		initStruct_ex (&isp, true);
		php_error_docref (NULL, E_WARNING, "Failure object initialize");
		RETURN_FALSE;
	}

	if ( array_init (&dummy) == FAILURE ) {
		initStruct_ex (&isp, true);
		php_error_docref (NULL, E_WARNING, "Failure array initialize");
		RETURN_FALSE;
	}

	netmask = guess_netmask (isp.start, isp.end);
	networkv = network (isp.start, netmask);
	broadcastv = broadcast (isp.start, netmask);

	add_property_string (return_value, "host", ZSTR_VAL (host));
	add_property_string (return_value, "ip", isp.ip);
	add_property_string (return_value, "start", long2ip_r (isp.start, rip));
	add_property_string (return_value, "end", long2ip_r (isp.end, rip));
	add_property_string (return_value, "netmask", long2ip_r (netmask, rip));
	add_property_string (return_value, "network", long2ip_r (networkv, rip));
	add_property_string (return_value, "broadcast", long2ip_r (broadcastv, rip));
	add_property_long (return_value, "size", isp.size);

	{
		short r;

		for ( r=0; r<isp.size; r++ )
			add_index_string (&dummy, r, isp.dummy[r]);
	}

	add_property_zval (return_value, "data", &dummy);
	initStruct_ex (&isp, true);
}
/* }}} */

/* {{{ proto (bool) krisp_close (resource link)
 *  close krisp database */
PHP_FUNCTION(krisp_close)
{
	zval      * krisp_link;
	KRISP_API *	kr;

	zval      * object = getThis ();
	KROBJ     * obj;

	PRINT_CALL_API_NAME;

	if ( object ) {
		obj = Z_KRISP_P (object);
		kr_printf ("obj->db   ------------------------> %d\n", obj->db);

		if ( ! obj->db ) {
			RETURN_TRUE;
		}

		kr_printf ("obj->db->rsrc          -----------> %d\n", obj->db->rsrc);
		kr_printf ("obj->db->rsrc->ptr     -----------> %d\n", obj->db->rsrc->ptr);
		kr_printf ("obj->db->rsrc->type    -----------> %d\n", obj->db->rsrc->type);
		// if call zend_list_close,
		// calling zend_register_list_destructors_ex (_close_krisp_link on this source) too.
		zend_list_close (obj->db->rsrc);

		// free'd obj->db in _close_krisp_link, so set NULL in here.
		obj->db = NULL;
		kr_printf ("obj->db   ------------------------> %d\n", obj->db);
	} else {
		if ( krisp_parameters ("r", &krisp_link) == FAILURE)
			return;
		KR_FETCH_RESOURCE (kr, KRISP_API *, krisp_link, "KRISP database", le_krisp);

		/* call the _close_krisp_link
		 * Z_RES_P(krisp_link) sames kr->rsrc
		 */
		zend_list_close (Z_RES_P(krisp_link));
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto (object) krisp_netmask (string start, string end)
 *  return netmask and prefix about given ip range */
PHP_FUNCTION(krisp_netmask)
{
	char          rip[16];
	zend_string * start;
	zend_string * end;
	ulong         lstart;
	ulong         lend;
	ulong         mask;

	if ( krisp_parameters ("SS", &start, &end) == FAILURE )
		return;

	if ( ZSTR_LEN (start) == 0 ) {
		php_error_docref (NULL, E_WARNING, "start address is none");
		RETURN_FALSE;
	}

	if ( ZSTR_LEN (end) == 0 ) {
		php_error_docref (NULL, E_WARNING, "end address is none");
		RETURN_FALSE;
	}

	lstart = krisp_format_convert (ZSTR_VAL (start));
	lend   = krisp_format_convert (ZSTR_VAL (end));

	if ( object_init (return_value) == FAILURE ) {
		php_error_docref (NULL, E_WARNING, "Failure object initialize");
		RETURN_FALSE;
	}

	mask = guess_netmask (lstart, lend);
	add_property_string (return_value, "mask", long2ip_r (mask, rip));
	add_property_long (return_value, "prefix", long2prefix (mask));
}
/* }}} */

static void krisp_network_broadcast (INTERNAL_FUNCTION_PARAMETERS, zend_bool type) // {{{
{
	zend_string * ip;
	zend_string * mask;
	ulong         lip;
	ulong         lmask;
	char          rip[16];

	if ( krisp_parameters ("SS", &ip, &mask) == FAILURE )
		return;

	if ( ZSTR_LEN (ip) == 0) {
		php_error_docref (NULL, E_WARNING, "First argument (IP address) is none");
		RETURN_FALSE;
	}

	if ( ZSTR_LEN (mask) == 0) {
		php_error_docref (NULL, E_WARNING, "Second argument (Network mask) is none");
		RETURN_FALSE;
	}

	lip   = krisp_format_convert (ZSTR_VAL (ip));
	lmask = krisp_format_convert (ZSTR_VAL (mask));

	RETURN_STRING (
			type ?
				long2ip_r (broadcast (lip, lmask), rip) :
				long2ip_r (network (lip, lmask), rip)
	);
} // }}}

/* {{{ proto (string|false) krisp_network (string ip, string mask)
 *  reuturn network address about given ip and network mask */
PHP_FUNCTION(krisp_network)
{
	krisp_network_broadcast (INTERNAL_FUNCTION_PARAM_PASSTHRU, 0);
}
/* }}} */

/* {{{ proto (string|false) krisp_broadcast (string ip, string mask)
 *  reuturn broadcast address about given ip and network mask */
PHP_FUNCTION(krisp_broadcast)
{
	krisp_network_broadcast (INTERNAL_FUNCTION_PARAM_PASSTHRU, 1);
}
/* }}} */

/* {{{ proto (string) krisp_prefix2mask (int prefix)
 *  return unsigned long value for given network prefix */
PHP_FUNCTION(krisp_prefix2mask)
{
	short prefix;
	char  rip[16];

	if ( krisp_parameters ("l", &prefix) == FAILURE )
		return;

	RETURN_STRING (long2ip_r (prefix2long (prefix), rip));
}
/* }}} */

/* {{{ proto (int) krisp_mask2prefix (string mask)
 *  return short network prefix for given long network mask */
PHP_FUNCTION(krisp_mask2prefix)
{
	zend_string * mask;

	if ( krisp_parameters ("S", &mask) == FAILURE )
		return;

	RETURN_LONG (long2prefix (ip2long (ZSTR_VAL (mask))));
}
/* }}} */

/* {{{ proto (bool) krisp_set_mtime_interval (resoruce link, int sec)
 *  set krisp database mtime check interval */
PHP_FUNCTION(krisp_set_mtime_interval)
{
	zval              * object = getThis ();
	KROBJ             * obj;
	zval              * krisp_link;
	time_t              sec;
	KRISP_API         * kr;
	zend_error_handling error_handling;

	PRINT_CALL_API_NAME;

	if ( object) {
		KRISP_REPLACE_ERROR_HANDLING;
		if ( krisp_parameters ("l", &sec) == FAILURE ) {
			return;
		}

		obj = Z_KRISP_P (object);
		kr = obj->db;

		if ( ! kr || kr->handler == NULL ) {
			php_error_docref (NULL, E_WARNING, "No KRISP object available");
			RETURN_FALSE;
		}
	} else {
		if ( krisp_parameters ("rl", &krisp_link, &sec) == FAILURE ) {
			return;
		}

		KR_FETCH_RESOURCE (kr, KRISP_API *, krisp_link, "KRISP database", le_krisp);
	}

	kr->handler->db_time_stamp_interval = sec;

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto (bool) krisp_set_debug (resoruce link[, int switch = true])
 *  print libkrisp debug messages */
PHP_FUNCTION(krisp_set_debug)
{
	zval              * object = getThis ();
	KROBJ             * obj;
	zval              * krisp_link;
	zend_long           switches = true;
	KRISP_API         * kr;
	zend_error_handling error_handling;

	PRINT_CALL_API_NAME;

	if ( object) {
		KRISP_REPLACE_ERROR_HANDLING;
		if ( krisp_parameters ("|l", &switches) == FAILURE ) {
			return;
		}

		obj = Z_KRISP_P (object);
		kr = obj->db;

		if ( ! kr || kr->handler == NULL ) {
			php_error_docref (NULL, E_WARNING, "No KRISP object available");
			RETURN_FALSE;
		}
	} else {
		if ( krisp_parameters ("r|l", &krisp_link, &switches) == FAILURE ) {
			return;
		}

		KR_FETCH_RESOURCE (kr, KRISP_API *, krisp_link, "KRISP database", le_krisp);
	}

	kr->handler->verbose = switches;

	RETURN_TRUE;
}
/* }}} */

/*
 * Internal APIs
 */

PHP_KRISP_API ulong krisp_format_convert (char * v) {
	if ( strchr (v, '.') == NULL )
		return (ulong) strtoul (v, NULL, 10);
	return ip2long (v);
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
