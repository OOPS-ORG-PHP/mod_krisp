/*
 * +----------------------------------------------------------------------+
 * | Copyright 2021. JoungKyun.Kim all rights reserved.                   |
 * +----------------------------------------------------------------------+
 * | This source file is subject to version 3.01 of the PHP license,      |
 * | that is bundled with this package in the file LICENSE, and is        |
 * | available through the world-wide-web at the following url:           |
 * | http://www.php.net/license/3_01.txt                                  |
 * | If you did not receive a copy of the PHP license and are unable to   |
 * | obtain it through the world-wide-web, please send a note to          |
 * | license@php.net so we can mail you a copy immediately.               |
 * +----------------------------------------------------------------------+
 * | Authors: JoungKyun.Kim <hostmaster@oops.org>                         |
 * +----------------------------------------------------------------------+
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
PHP_FUNCTION(krisp_search_ex);
PHP_FUNCTION(krisp_netmask);
PHP_FUNCTION(krisp_network);
PHP_FUNCTION(krisp_broadcast);
PHP_FUNCTION(krisp_prefix2mask);
PHP_FUNCTION(krisp_mask2prefix);
PHP_FUNCTION(krisp_set_mtime_interval);
PHP_FUNCTION(krisp_set_debug);

/* 
  	Declare any global variables you may need between the BEGIN
	and END macros here:     
 */

/*
ZEND_BEGIN_MODULE_GLOBALS(krisp)
	char err[1024];
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

#define BUILDNO "3.0.7"

#define phpext_krisp_ptr krisp_module_ptr

/*
 * Compatible legacy version
 */
#if PHP_VERSION_ID >= 80000
#  define TSRMLS_CC
#else
#  undef ZEND_ARG_OBJ_INFO
#  define ZEND_ARG_OBJ_INFO(a,b,c,d) ZEND_ARG_TYPE_INFO(a,b,IS_RESOURCE,d)
#  define ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(a,b,c,d) ZEND_BEGIN_ARG_INFO_EX(a,0,b,c)
#  define ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(a,b,c,d,e) ZEND_ARG_TYPE_INFO(a,b,c,d)
#endif

#if PHP_VERSION_ID < 70400
#  ifdef ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX
#    undef ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX
#  endif
#  define ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(a,b,c,d,e) ZEND_BEGIN_ARG_INFO_EX(a,0,b,c)
#endif

#if PHP_VERSION_ID < 70200
#  define ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(a,b,c,d,e) ZEND_BEGIN_ARG_INFO_EX(a,0,b,c)
#endif

/*
 * KRISP library header
 */

#include <ipcalc.h>
#include <krisp.h>

typedef struct krisp_info {
	// for Class
	zend_resource * rsrc;
	KR_API *	handler;
} KRISP_API;

ulong krisp_format_convert (char *);

#define krisp_parameters(...) \
	zend_parse_parameters (ZEND_NUM_ARGS (), __VA_ARGS__)

#define KR_FETCH_RESOURCE(a,b,c,d,e) \
	if ( (a = (b) zend_fetch_resource_ex (c, d, e)) == NULL ) { \
		RETURN_FALSE; \
	}

#ifndef kr_safe_efree
#define kr_safe_efree(x) do { if (x!=NULL) { efree (x); x = NULL; } } while (0)
#endif

//#define KR_DEBUG
#ifdef  KR_DEBUG
#	define PRINT_CALL_API_NAME php_printf ("*** ---------------> Call %s (%s:%d)\n", __func__, __FILE__, __LINE__);
#	define kr_printf(...) \
		php_printf ("    "); \
		php_printf (__VA_ARGS__)
#else
#	define PRINT_CALL_API_NAME
#	define kr_printf
#endif

#endif	/* PHP_KRISP_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
