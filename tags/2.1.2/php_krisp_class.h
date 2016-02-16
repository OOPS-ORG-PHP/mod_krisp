/*
 * $Id: php_krisp_class.h 76 2012-11-05 05:24:43Z oops $
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
#if PHP_MAJOR_VERSION >= 5
#if defined(HAVE_SPL) && ((PHP_MAJOR_VERSION > 5) || (PHP_MAJOR_VERSION == 5 && PHP_MINOR_VERSION >= 1))
extern PHPAPI zend_class_entry *spl_ce_RuntimeException;
extern PHPAPI zend_class_entry *spl_ce_Countable;
#endif

#define KRISP_REPLACE_ERROR_HANDLING \
	zend_replace_error_handling ( \
		object ? EH_THROW : EH_NORMAL, \
		krisp_ce_exception, \
		&error_handling TSRMLS_CC \
	)

#define KRISP_RESTORE_ERROR_HANDLING zend_restore_error_handling (&error_handling TSRMLS_CC)
#else
#define KRISP_REPLACE_ERROR_HANDLIN0 int krisp_error_dummy_handing = 1
#define KRISP_RESTORE_ERROR_HANDLING krisp_error_dummy_handing = 0
#endif
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
const zend_function_entry krisp_methods[] = {
	PHP_ME_MAPPING (__construct,   krisp_open,               arginfo_krisp_open, ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING (close,         krisp_close,              NULL,               ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING (search,        krisp_search,             NULL,               ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING (searchEx,      krisp_search_ex,          NULL,               ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING (mtimeInterval, krisp_set_mtime_interval, NULL,               ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING (debug,         krisp_set_debug,          NULL,               ZEND_ACC_PUBLIC)
	{NULL, NULL, NULL}
};

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