/*
 * Copyright (C) 2016 by JoungKyun.Kim <https://joungkyun.github.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
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
const zend_function_entry krisp_methods[] = {
	PHP_ME_MAPPING (__construct,   krisp_open,               arginfo_krisp_open, ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING (close,         krisp_close,              NULL,               ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING (search,        krisp_search,             NULL,               ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING (searchEx,      krisp_search_ex,          NULL,               ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING (mtimeInterval, krisp_set_mtime_interval, NULL,               ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING (debug,         krisp_set_debug,          NULL,               ZEND_ACC_PUBLIC)
	{NULL, NULL, NULL}
};

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
