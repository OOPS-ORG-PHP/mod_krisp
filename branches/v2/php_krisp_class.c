/**
 * Project: mod_krisp :: KRISP database php extension
 * File:    php_krisp_class.c
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
 * @version     SVN: $Id$
 * @since       File available since release 0.0.1
 */

/*
 * PHP5 Korea IPS Class API
 */

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

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */