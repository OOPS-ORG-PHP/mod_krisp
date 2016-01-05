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

	PRINT_CALL_API_NAME;

	if ( ! intern )
		return;


	if ( intern->db != NULL ) {
		kr_printf ("intern->db    --------------------> %d\n", intern->db);
		if ( intern->db->rsrc ) {
			//kr_printf ("GC_REFCOUNT(intern->db->rsrc) --> %d\n", GC_REFCOUNT(intern->db->rsrc));
			// close krisp handler
			zend_list_close (intern->db->rsrc);
		}

		if ( intern->db != NULL ) {
			if ( intern->db->handler != NULL ) {
				kr_close (&intern->db->handler);
				intern->db->handler = NULL;
			}
			kr_printf ("intern->db before free   ---------> %d\n", intern->db);
			safe_efree (intern->db);
			kr_printf ("intern->db after free    ---------> %d\n", intern->db);
		}
	}

	kr_printf ("...\n");
	zend_object_std_dtor (&intern->std);
	kr_printf ("...\n");
}

static zend_object * krisp_object_new (zend_class_entry *class_type) {
	KROBJ * intern;

	PRINT_CALL_API_NAME;

	intern = ecalloc (1, sizeof (KROBJ) + zend_object_properties_size (class_type));
	zend_object_std_init (&intern->std,class_type TSRMLS_CC);
	intern->std.handlers = &krisp_object_handlers;

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
