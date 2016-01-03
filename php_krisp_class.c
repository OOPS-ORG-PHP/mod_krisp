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

#define Z_KRISP_P(zv) krisp_fetch_object(Z_OBJ_P(zv))

static inline KROBJ * krisp_fetch_object (zend_object * obj) {
	return (KROBJ *) ((char *) obj - XtOffsetOf(KROBJ, std));
}

static void krisp_object_free_storage (zend_object * object) {
	KROBJ * intern = (KROBJ *) krisp_fetch_object (object);

	if ( ! intern )
		return;

	if ( intern->u.db != NULL ) {
		if ( intern->u.db->rsrc ) {
			// close krisp handler
			zend_list_close (intern->u.db->rsrc);
		}

		if ( intern->u.db != NULL ) {
			if ( intern->u.db->db != NULL ) {
				kr_close (&intern->u.db->db);
				intern->u.db->db = NULL;
			}
			safe_efree (intern->u.db);
			intern->u.db = NULL;
		}
	}

	zend_object_std_dtor (&intern->std);
}

static zend_object * krisp_object_new (zend_class_entry *class_type) {
	KROBJ * intern;

	intern = ecalloc (1, sizeof (KROBJ) + zend_object_properties_size (class_type));
	zend_object_std_init (&intern->std,class_type TSRMLS_CC);
	object_properties_init(&intern->std, class_type);
	intern->std.handlers = &krisp_object_handlers;

	return &intern->std;
}

static zend_object * krisp_object_new_exception (zend_class_entry *class_type) {
	KROBJ * intern;

	intern = ecalloc (1, sizeof (KROBJ) + zend_object_properties_size (class_type));
	zend_object_std_init (&intern->std,class_type TSRMLS_CC);
	object_properties_init(&intern->std, class_type);
	intern->std.handlers = &krisp_object_handlers_exception;

	return &intern->std;
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
