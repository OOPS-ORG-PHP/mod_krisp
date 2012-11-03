/**
 * Project: mod_krisp :: KRISP database php extension
 * File:    krisp.c
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
 * @version     CVS: $Id$
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
#include <unistd.h>

#include "php.h"
#include "php_ini.h"
#include "SAPI.h"
#include "ext/standard/info.h"

#include "zend_exceptions.h"
#include "zend_interfaces.h"

#include "php_krisp.h"

/* If you declare any globals in php_krisp.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(krisp)
 */

/* True global resources - no need for thread safety here */
static int le_krisp;

ZEND_BEGIN_ARG_INFO_EX(arginfo_krisp_open, 0, 0, 0)
	ZEND_ARG_INFO(0, database)
	ZEND_ARG_INFO(1, error)
ZEND_END_ARG_INFO()

/* {{{ krisp_functions[]
 *
 * Every user visible function must have an entry in krisp_functions[].
 */
const zend_function_entry krisp_functions[] = {
	PHP_FE(krisp_buildver,			NULL)
	PHP_FE(krisp_version,			NULL)
	PHP_FE(krisp_uversion,			NULL)
	PHP_FE(krisp_open,				arginfo_krisp_open)
	PHP_FE(krisp_search,			NULL)
	PHP_FE(krisp_search_ex,			NULL)
	PHP_FE(krisp_close,				NULL)
	PHP_FE(krisp_netmask,			NULL)
	PHP_FE(krisp_network,			NULL)
	PHP_FE(krisp_broadcast,			NULL)
	PHP_FE(krisp_prefix2mask,		NULL)
	PHP_FE(krisp_mask2prefix,		NULL)
	PHP_FE(krisp_set_mtime_interval,	NULL)
	PHP_FE(krisp_set_debug,			NULL)
	{NULL, NULL, NULL}
};
/* }}} */

/* {{{ krisp_deps[]
 *
 * KRISP dependancies
 */
const zend_module_dep krisp_deps[] = {
#if defined(HAVE_SPL) && ((PHP_MAJOR_VERSION > 5) || (PHP_MAJOR_VERSION == 5 && PHP_MINOR_VERSION >= 1))
	ZEND_MOD_REQUIRED("spl")
#endif
	{NULL, NULL, NULL}
};
/* }}} */

/* {{{ For Class declears */
#define REGISTER_KRISP_CLASS(parent) { \
	zend_class_entry ce; \
	INIT_CLASS_ENTRY (ce, "KRISP", krisp_methods); \
	ce.create_object = krisp_object_new_main; \
	krisp_ce = zend_register_internal_class_ex (&ce, parent, NULL TSRMLS_CC); \
	memcpy(&krisp_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers)); \
	krisp_object_handlers.clone_obj = NULL; \
	krisp_ce->ce_flags |= ZEND_ACC_FINAL_CLASS; \
}


#define REGISTER_KRISP_PER_CLASS(name, c_name, parent) { \
	zend_class_entry ce; \
	INIT_CLASS_ENTRY(ce, "KRISP" # name, krisp_methods_ ## c_name); \
	ce.create_object = krisp_object_new_ ## c_name; \
	krisp_ce_ ## c_name = zend_register_internal_class_ex(&ce, parent, NULL TSRMLS_CC); \
	memcpy(&krisp_object_handlers_ ## c_name, zend_get_std_object_handlers(), sizeof(zend_object_handlers)); \
	krisp_object_handlers_ ## c_name.clone_obj = NULL; \
	krisp_ce_ ## c_name->ce_flags |= ZEND_ACC_FINAL_CLASS; \
}

zend_class_entry * krisp_ce;
zend_class_entry * krisp_ce_exception;
static zend_object_handlers krisp_object_handlers;
static zend_object_handlers krisp_object_handlers_exception;

typedef struct _krisp_object {
	zend_object     std;
	union {
		KRISP_API * db;
		void      * ptr;
	} u;
} KROBJ;

const zend_function_entry krisp_methods[] = {
	PHP_ME_MAPPING (__construct,   krisp_open,               arginfo_krisp_open, ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING (close,         krisp_close,              NULL,               ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING (search,        krisp_search,             NULL,               ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING (searchEx,      krisp_search_ex,          NULL,               ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING (mtimeInterval, krisp_set_mtime_interval, NULL,               ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING (debug,         krisp_set_debug,          NULL,               ZEND_ACC_PUBLIC)
	{NULL, NULL, NULL}
};
/* For Class declears }}} */

/* {{{ Exception entry */
const zend_function_entry krisp_methods_exception[] = {
	{NULL, NULL, NULL}
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

static void _close_krisp_link(zend_rsrc_list_entry * rsrc TSRMLS_DC)
{
	KRISP_API *	kr = (KRISP_API *) rsrc->ptr;
	kr_close (&kr->db);
	free (kr);
}

/* {{{ Class API */

static int krisp_free_persistent (zend_rsrc_list_entry * le, void * ptr TSRMLS_DC) {
	return le->ptr == ptr ? ZEND_HASH_APPLY_REMOVE : ZEND_HASH_APPLY_KEEP;
}

static void krisp_object_free_storage (void * object TSRMLS_DC) {
	KROBJ * intern = (KROBJ *) object;

	zend_object_std_dtor (&intern->std TSRMLS_CC);

	if ( intern->u.ptr ) {
		if ( intern->u.db->rsrc ) {
			zend_list_delete (intern->u.db->rsrc);
			zend_hash_apply_with_argument (
					&EG(persistent_list),
					(apply_func_arg_t) krisp_free_persistent,
					&intern->u.ptr TSRMLS_CC
			);
		}
	}

	efree(object);
}

static void krisp_object_new (zend_class_entry *class_type, zend_object_handlers *handlers, zend_object_value *retval TSRMLS_DC)
{
	KROBJ * intern;
	zval  * tmp;

	intern = emalloc (sizeof (KROBJ));
	memset (intern, 0, sizeof (KROBJ));

	zend_object_std_init (&intern->std, class_type TSRMLS_CC);
	retval->handle = zend_objects_store_put(
		intern,
		(zend_objects_store_dtor_t) zend_objects_destroy_object,
		(zend_objects_free_object_storage_t) krisp_object_free_storage,
		NULL TSRMLS_CC
	);
	retval->handlers = handlers;
}

static zend_object_value krisp_object_new_main (zend_class_entry * class_type TSRMLS_DC) {
	zend_object_value retval;

	krisp_object_new (class_type, &krisp_object_handlers, &retval TSRMLS_CC);
	return retval;
}

static zend_object_value krisp_object_new_exception (zend_class_entry * class_type TSRMLS_DC) {
	zend_object_value retval;

	krisp_object_new (class_type, &krisp_object_handlers_exception, &retval TSRMLS_CC);
	return retval;
}
/* Class API }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(krisp)
{
	le_krisp = zend_register_list_destructors_ex (_close_krisp_link, NULL, "krisp link", module_number);

	REGISTER_KRISP_CLASS(NULL);

	krisp_ce->ce_flags &= ~ZEND_ACC_FINAL_CLASS;
	krisp_ce->constructor->common.fn_flags |= ZEND_ACC_FINAL;

#if defined(HAVE_SPL) && ((PHP_MAJOR_VERSION > 5) || (PHP_MAJOR_VERSION == 5 && PHP_MINOR_VERSION >= 1))
	REGISTER_KRISP_PER_CLASS(Exception, exception, spl_ce_RuntimeException);
#elif PHP_MAJOR_VERSION >= 5
	REGISTER_KRISP_PER_CLASS(Exception, exception, zend_exception_get_default(TSRMLS_C));
#endif

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
	char      * database = NULL;
	int         database_len;
	zval      * error = NULL;
	KRISP_API * kr;

	char        err[1024];

	zval      * object = getThis ();
	zend_error_handling error_handling;

	KRISP_REPLACE_ERROR_HANDLING;
	if ( zend_parse_parameters (ZEND_NUM_ARGS () TSRMLS_CC, "|sz", &database, &database_len, &error) == FAILURE ) {
		KRISP_RESTORE_ERROR_HANDLING;
		return;
	}

	if ( database != NULL && database_len < 1 )
		database = NULL;

	if ( database != NULL ) {
		if ( php_check_open_basedir (database TSRMLS_CC) ) {
			KRISP_RESTORE_ERROR_HANDLING;
			RETURN_FALSE;
		}
	}

	kr = (KRISP_API *) malloc (sizeof (KRISP_API));

	if ( kr_open_safe (&kr->db, database, err) == false ) {
		php_error_docref (NULL TSRMLS_CC, E_WARNING, "%s", err);
		if ( error != NULL ) {
			zval_dtor (error);
			ZVAL_STRING (error, err, 1);
		}
		free (kr);
		kr = NULL;
		KRISP_RESTORE_ERROR_HANDLING;
		RETURN_FALSE;
	}

	kr->rsrc = ZEND_REGISTER_RESOURCE (
			object ? NULL : return_value,
			kr, le_krisp
	);

	if ( object ) {
		KROBJ * obj;
		obj = (KROBJ *) zend_object_store_get_object (object TSRMLS_CC);
		obj->u.db = kr;
	}

	KRISP_RESTORE_ERROR_HANDLING;
}
/* }}} */

/* {{{ proto object krisp_search (resource, char *host)
 *  return isp information array */
PHP_FUNCTION(krisp_search)
{
	zval      * krisp_link;
	char      * host;
	int         host_len;
	KRISP_API * kr;

	KRNET_API   isp;
	char        rip[16];
	ulong       networkv;
	ulong       broadcastv;

	zval      * object = getThis ();
	KROBJ     * obj;
	zend_error_handling error_handling;

	KRISP_REPLACE_ERROR_HANDLING;
	if ( object ) {
		if ( zend_parse_parameters (ZEND_NUM_ARGS () TSRMLS_CC, "s", &host, &host_len) == FAILURE) {
			KRISP_RESTORE_ERROR_HANDLING;
			return;
		}
	} else {
		if ( zend_parse_parameters (ZEND_NUM_ARGS () TSRMLS_CC, "rs", &krisp_link, &host, &host_len) == FAILURE) {
			KRISP_RESTORE_ERROR_HANDLING;
			return;
		}
	}

	if ( host_len == 0 ) {
		php_error_docref (NULL TSRMLS_CC, E_WARNING, "length of host argument is 0");
		KRISP_RESTORE_ERROR_HANDLING;
		RETURN_FALSE;
	}

	if ( object ) {
		obj = (KROBJ *) zend_object_store_get_object (object TSRMLS_CC);
		kr = obj->u.db;
		if ( ! kr || kr->db == NULL ) {
			php_error_docref (NULL TSRMLS_CC, E_WARNING, "No KRISP object available");
			KRISP_RESTORE_ERROR_HANDLING;
			RETURN_FALSE;
		}
	} else
		ZEND_FETCH_RESOURCE (kr, KRISP_API *, &krisp_link, -1, "KRISP database", le_krisp);

	SAFECPY_256 (isp.ip, host);
	isp.verbose = kr->db->verbose;

	if ( kr_search (&isp, kr->db) ) {
		php_error_docref (NULL TSRMLS_CC, E_WARNING, "%s", isp.err);
		KRISP_RESTORE_ERROR_HANDLING;
		RETURN_FALSE;
	}

	if ( object_init (return_value) == FAILURE ) {
		php_error_docref (NULL TSRMLS_CC, E_WARNING, "Failure object initialize");
		KRISP_RESTORE_ERROR_HANDLING;
		RETURN_FALSE;
	}

	networkv = network (isp.start, isp.netmask);
	broadcastv = broadcast (isp.start, isp.netmask);

	add_property_string (return_value, "host", host, 1);
	add_property_string (return_value, "ip", isp.ip, 1);
	add_property_string (return_value, "start", long2ip_r (isp.start, rip), 1);
	add_property_string (return_value, "end", long2ip_r (isp.end, rip), 1);
	add_property_string (return_value, "netmask", long2ip_r (isp.netmask, rip), 1);
	add_property_string (return_value, "network", long2ip_r (networkv, rip), 1);
	add_property_string (return_value, "broadcast", long2ip_r (broadcastv, rip), 1);
	add_property_string (return_value, "icode", isp.icode, 1);
	add_property_string (return_value, "iname", isp.iname, 1);
	add_property_string (return_value, "ccode", isp.ccode, 1);
	add_property_string (return_value, "cname", isp.cname, 1);

	KRISP_RESTORE_ERROR_HANDLING;
}
/* }}} */

/* {{{ proto object krisp_search_ex (resource, char *host)
 *  return isp information array */
PHP_FUNCTION(krisp_search_ex)
{
	zval *			krisp_link;
	char *			host;
	int				host_len;
	KRISP_API *		kr;

	KRNET_API_EX	isp;
	char			rip[16];
	zval *			dummy = NULL;
	char *			table;
	int				table_len;
	ulong			netmask;
	ulong			networkv;
	ulong			broadcastv;

	zval      * object = getThis ();
	KROBJ     * obj;
	zend_error_handling error_handling;

	KRISP_REPLACE_ERROR_HANDLING;
	if ( object ) {
		if ( zend_parse_parameters (ZEND_NUM_ARGS () TSRMLS_CC, "s|s", &host, &host_len, &table, &table_len) == FAILURE) {
			KRISP_RESTORE_ERROR_HANDLING;
			return;
		}
	} else {
		if ( zend_parse_parameters (ZEND_NUM_ARGS () TSRMLS_CC, "rs|s", &krisp_link, &host, &host_len, &table, &table_len) == FAILURE) {
			KRISP_RESTORE_ERROR_HANDLING;
			return;
		}

	}

	{
		int arglen = object ? 1 : 2;
		if ( ZEND_NUM_ARGS () == arglen ) {
			table = "krisp";
			table_len = 5;
		}
	}

	if ( table_len == 0 )
		table = "krisp";

	if ( strlen (host) == 0) {
		KRISP_RESTORE_ERROR_HANDLING;

		php_error_docref (NULL TSRMLS_CC, E_WARNING, "length of host argument is 0");
		RETURN_FALSE;
	}

	if ( object ) {
		obj = (KROBJ *) zend_object_store_get_object (object TSRMLS_CC);
		kr = obj->u.db;
		if ( ! kr || kr->db == NULL ) {
			php_error_docref (NULL TSRMLS_CC, E_WARNING, "No KRISP object available");
			KRISP_RESTORE_ERROR_HANDLING;
			RETURN_FALSE;
		}
	} else
		ZEND_FETCH_RESOURCE (kr, KRISP_API *, &krisp_link, -1, "KRISP database", le_krisp);

	SAFECPY_256 (isp.ip, host);
	isp.verbose = kr->db->verbose;
	kr->db->table = table;

	if ( kr_search_ex (&isp, kr->db) ) {
		KRISP_RESTORE_ERROR_HANDLING;

		php_error_docref (NULL TSRMLS_CC, E_WARNING, "%s", isp.err);
		initStruct_ex (&isp, true);
		RETURN_FALSE;
	}

	if ( object_init (return_value) == FAILURE ) {
		initStruct_ex (&isp, true);
		php_error_docref (NULL TSRMLS_CC, E_WARNING, "Failure object initialize");
		KRISP_RESTORE_ERROR_HANDLING;
		RETURN_FALSE;
	}

	MAKE_STD_ZVAL(dummy);
	if ( array_init (dummy) == FAILURE ) {
		initStruct_ex (&isp, true);
		php_error_docref (NULL TSRMLS_CC, E_WARNING, "Failure array initialize");
		KRISP_RESTORE_ERROR_HANDLING;
		RETURN_FALSE;
	}

	netmask = guess_netmask (isp.start, isp.end);
	networkv = network (isp.start, netmask);
	broadcastv = broadcast (isp.start, netmask);

	add_property_string (return_value, "host", host, 1);
	add_property_string (return_value, "ip", isp.ip, 1);
	add_property_string (return_value, "start", long2ip_r (isp.start, rip), 1);
	add_property_string (return_value, "end", long2ip_r (isp.end, rip), 1);
	add_property_string (return_value, "netmask", long2ip_r (netmask, rip), 1);
	add_property_string (return_value, "network", long2ip_r (networkv, rip), 1);
	add_property_string (return_value, "broadcast", long2ip_r (broadcastv, rip), 1);
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
	zval      * krisp_link;
	KRISP_API *	kr;

	zval      * object = getThis ();
	KROBJ     * obj;

	if ( object ) {
		obj = (KROBJ *) zend_object_store_get_object (object TSRMLS_CC);
		kr = obj->u.db;
		if ( ! kr || kr->db != NULL )
			RETURN_TRUE;
		zend_list_delete (obj->u.db->rsrc);
	} else {
		if ( zend_parse_parameters (ZEND_NUM_ARGS () TSRMLS_CC, "r", &krisp_link) == FAILURE)
			return;
		ZEND_FETCH_RESOURCE (kr, KRISP_API *, &krisp_link, -1, "KRISP database", le_krisp);
		zend_list_delete(Z_RESVAL_P(krisp_link));
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto object krisp_netmask (start, end)
 *  return netmask and prefix about given ip range */
PHP_FUNCTION(krisp_netmask)
{
	char	rip[16];
	char *	start;
	int		start_len;
	char *	end;
	int		end_len;
	ulong	lstart;
	ulong	lend;
	ulong	mask;

	if ( zend_parse_parameters (ZEND_NUM_ARGS () TSRMLS_CC, "ss", &start, &start_len, &end, &end_len) == FAILURE )
		return;

	if ( start_len == 0 ) {
		php_error_docref (NULL TSRMLS_CC, E_WARNING, "start address is none");
		RETURN_FALSE;
	}

	if ( end_len == 0 ) {
		php_error_docref (NULL TSRMLS_CC, E_WARNING, "end address is none");
		RETURN_FALSE;
	}

	lstart = krisp_format_convert (start);
	lend   = krisp_format_convert (end);

	if ( object_init (return_value) == FAILURE ) {
		php_error_docref (NULL TSRMLS_CC, E_WARNING, "Failure object initialize");
		RETURN_FALSE;
	}

	mask = guess_netmask (lstart, lend);
	add_property_string (return_value, "mask", long2ip_r (mask, rip), 1);
	add_property_long (return_value, "prefix", long2prefix (mask));
}
/* }}} */

static void krisp_network_broadcast (INTERNAL_FUNCTION_PARAMETERS, zend_bool type) // {{{
{
	char *	ip;
	int		ip_len;
	char *	mask;
	int		mask_len;
	ulong	lip;
	ulong	lmask;
	char	rip[16];

	if ( zend_parse_parameters (ZEND_NUM_ARGS () TSRMLS_CC, "ss", &ip, &ip_len, &mask, &mask_len) == FAILURE )
		return;

	if ( ip_len == 0) {
		php_error_docref (NULL TSRMLS_CC, E_WARNING, "First argument (IP address) is none");
		RETURN_FALSE;
	}

	if ( mask_len == 0) {
		php_error_docref (NULL TSRMLS_CC, E_WARNING, "Second argument (Network mask) is none");
		RETURN_FALSE;
	}

	lip   = krisp_format_convert (ip);
	lmask = krisp_format_convert (mask);

	RETURN_STRING (
			type ?
				long2ip_r (broadcast (lip, lmask), rip) :
				long2ip_r (network (lip, lmask), rip),
			1
	);
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
	short	prefix;
	char	rip[16];

	if ( zend_parse_parameters (ZEND_NUM_ARGS () TSRMLS_CC, "l", &prefix) == FAILURE )
		return;

	RETURN_STRING (long2ip_r (prefix2long (prefix), rip), 1);
}
/* }}} */

/* {{{ proto short krisp_mask2prefix (mask)
 *  return short network prefix for given long network mask */
PHP_FUNCTION(krisp_mask2prefix)
{
	char *	mask;
	int		mask_len;

	if ( zend_parse_parameters (ZEND_NUM_ARGS () TSRMLS_CC, "s", &mask, &mask_len) == FAILURE )
		return;

	RETURN_LONG (long2prefix (ip2long (mask)));
}
/* }}} */

/* {{{ proto void krisp_set_mtime_interval (resoruce, sec)
 *  set krisp database mtime check interval */
PHP_FUNCTION(krisp_set_mtime_interval)
{
	zval      * object = getThis ();
	zval      * krisp_link;
	time_t      sec;
	KRISP_API * kr;
	KROBJ     * obj;
	zend_error_handling error_handling;

	KRISP_REPLACE_ERROR_HANDLING;

	if ( object) {
		if ( zend_parse_parameters (ZEND_NUM_ARGS () TSRMLS_CC, "l", &sec) == FAILURE ) {
			KRISP_RESTORE_ERROR_HANDLING;
			return;
		}

		obj = (KROBJ *) zend_object_store_get_object (object TSRMLS_CC);
		kr = obj->u.db;

		if ( ! kr || kr->db == NULL ) {
			php_error_docref (NULL TSRMLS_CC, E_WARNING, "No KRISP object available");
			RETURN_FALSE;
		}
	} else {
		if ( zend_parse_parameters (ZEND_NUM_ARGS () TSRMLS_CC, "rl", &krisp_link, &sec) == FAILURE ) {
			KRISP_RESTORE_ERROR_HANDLING;
			return;
		}

		ZEND_FETCH_RESOURCE (kr, KRISP_API *, &krisp_link, -1, "KRISP database", le_krisp);
	}
	KRISP_RESTORE_ERROR_HANDLING;

	kr->db->db_time_stamp_interval = sec;

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto void krisp_set_debug (resoruce[, switch = true])
 *  print libkrisp debug messages */
PHP_FUNCTION(krisp_set_debug)
{
	zval      * object = getThis ();
	zval      * krisp_link;
	zend_bool   switches = true;
	KRISP_API * kr;
	KROBJ     * obj;
	zend_error_handling error_handling;

	KRISP_REPLACE_ERROR_HANDLING;

	if ( object) {
		if ( zend_parse_parameters (ZEND_NUM_ARGS () TSRMLS_CC, "l", &switches) == FAILURE ) {
			KRISP_RESTORE_ERROR_HANDLING;
			return;
		}

		obj = (KROBJ *) zend_object_store_get_object (object TSRMLS_CC);
		kr = obj->u.db;

		if ( ! kr || kr->db == NULL ) {
			php_error_docref (NULL TSRMLS_CC, E_WARNING, "No KRISP object available");
			RETURN_FALSE;
		}
	} else {
		if ( zend_parse_parameters (ZEND_NUM_ARGS () TSRMLS_CC, "r|l", &krisp_link, &switches) == FAILURE ) {
			KRISP_RESTORE_ERROR_HANDLING;
			return;
		}

		ZEND_FETCH_RESOURCE (kr, KRISP_API *, &krisp_link, -1, "KRISP database", le_krisp);
	}
	KRISP_RESTORE_ERROR_HANDLING;

	kr->db->verbose = switches;

	RETURN_TRUE;
}
/* }}} */

/*
 * Internal APIs
 */

ulong krisp_format_convert (char * v) {
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
