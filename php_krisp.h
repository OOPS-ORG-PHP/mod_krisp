/*
 * $Id$
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

/* PHP_VERSION_ID is supported from 5.2 */
#ifndef PHP_VERSION_ID
#  if ZEND_MODULE_API_NO > 20050922
#    define PHP_VERSION_ID 50100
#  elif ZEND_MODULE_API_NO > 20041030
#    define PHP_VERSION_ID 50000
#  elif ZEND_MODULE_API_NO > 20020429
#    define PHP_VERSION_ID 40400
#  elif ZEND_MODULE_API_NO > 20020429
#    define PHP_VERSION_ID 40300
#  elif ZEND_MODULE_API_NO > 20010901
#    define PHP_VERSION_ID 40200
#  elif ZEND_MODULE_API_NO > 20001222
#    define PHP_VERSION_ID 40100
#  elif ZEND_MODULE_API_NO == 20001222
#    define PHP_VERSION_ID 40000
#  endif
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

#define BUILDNO "2.1.3"

#define phpext_krisp_ptr krisp_module_ptr


/*
 * KRISP library header
 */

#include <ipcalc.h>
#include <krisp.h>

typedef struct krisp_info {
	KR_API *	db;
	// for Class
	int			rsrc;
} KRISP_API;

ulong krisp_format_convert (char *);

#define krisp_parameters(...) \
	zend_parse_parameters (ZEND_NUM_ARGS () TSRMLS_CC, __VA_ARGS__)

#endif	/* PHP_KRISP_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
