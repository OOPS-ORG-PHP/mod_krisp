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

#ifndef PHP_KRISP_CLASS_H
#define PHP_KRISP_CLASS_H

/* {{{ Exception entry */
const zend_function_entry krisp_methods_exception[] = {
	{NULL, NULL, NULL}
};
/* }}} */

/* Exception declear {{{
 *
 */
#if defined(HAVE_SPL)
extern PHPAPI zend_class_entry *spl_ce_RuntimeException;
extern PHPAPI zend_class_entry *spl_ce_Countable;
#endif

#define KRISP_REPLACE_ERROR_HANDLING \
	zend_replace_error_handling ( \
		object ? EH_THROW : EH_NORMAL, \
		krisp_ce_exception, \
		&error_handling \
	)

#define KRISP_RESTORE_ERROR_HANDLING_IF \
	if (object) \
		zend_restore_error_handling (&error_handling)

#define KRISP_RESTORE_ERROR_HANDLING \
	zend_restore_error_handling (&error_handling)

#define KRISP_THROW_EXCEPTION(msg) \
	if (object) \
		zend_throw_exception(krisp_ce_exception, msg, 0)
/* }}} */

/* {{{ krisp_deps[]
 *
 * KRISP dependancies
 */
const zend_module_dep krisp_deps[] = {
#if defined(HAVE_SPL)
	ZEND_MOD_REQUIRED("spl")
#endif
	ZEND_MOD_END
};
/* }}} */

/* {{{ For Class declears */
/* {{{ const zend_function_entry krisp_methods[] */
const zend_function_entry krisp_methods[] = {
	PHP_ME_MAPPING (__construct,   krisp_open,               arginfo_class_KRISP___construct,   ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING (search,        krisp_search,             arginfo_class_KRISP_search,        ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING (searchEx,      krisp_search_ex,          arginfo_class_KRISP_search_ex,     ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING (close,         krisp_close,              arginfo_class_KRISP_close,         ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING (mtimeInterval, krisp_set_mtime_interval, arginfo_class_KRISP_mtimeInterval, ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING (debug,         krisp_set_debug,          arginfo_class_KRISP_debug,         ZEND_ACC_PUBLIC)
	{NULL, NULL, NULL}
};
/* }}} */

#define krisp_method_parameters(...) \
	zend_parse_parameters (ZEND_NUM_ARGS (), getThis (), __VA_ARGS__)

zend_class_entry * krisp_ce;
zend_class_entry * krisp_ce_exception;
static zend_object_handlers krisp_object_handlers;
static zend_object_handlers krisp_object_handlers_exception;

typedef struct _krisp_object {
	KRISP_API   * db;
	zend_object   std;
} KROBJ;

/* For Class declears }}} */

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
