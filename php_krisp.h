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

  $Id: php_krisp.h,v 1.14 2010-06-18 13:34:50 oops Exp $
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
PHP_FUNCTION(krisp_prefix2long);
PHP_FUNCTION(krisp_long2prefix);

/* 
  	Declare any global variables you may need between the BEGIN
	and END macros here:     
 */

ZEND_BEGIN_MODULE_GLOBALS(krisp)
	char err[1024];
ZEND_END_MODULE_GLOBALS(krisp)

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

#define BUILDNO "2.0.0"

#define phpext_krisp_ptr krisp_module_ptr


/*
 * KRISP library header
 */

#include <krisp.h>

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