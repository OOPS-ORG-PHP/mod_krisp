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

/**
 * Project: mod_krisp :: KRISP database php extension
 * File:    php_krisp_class.c
 *
 * Copyright 2021. JoungKyun.Kim all rights reserved.
 *
 * LICENSE: PHP v3.0.1
 *
 * @category    Database
 * @package     mod_krisp
 * @author      JoungKyun.Kim <https://joungkyun.github.com>
 * @copyright   2021. JoungKyun.Kim
 * @license     PHP v3.0.1
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
		kr_printf ("intern->db->rsrc         ---------> %d\n", intern->db->rsrc);
		if ( intern->db->rsrc ) {
			//kr_printf ("GC_REFCOUNT(intern->db->rsrc) --> %d\n", GC_REFCOUNT(intern->db->rsrc));
			// close krisp handler
			kr_printf ("intern->db->rsrc->ptr    ---------> %d\n", intern->db->rsrc->ptr);
			kr_printf ("intern->db->rsrc->type   ---------> %d\n", intern->db->rsrc->type);

			// if call zend_list_close,
			// calling zend_register_list_destructors_ex (_close_krisp_link on this source) too.
			zend_list_close (intern->db->rsrc);
		}

		// free'd intern->db in _close_krisp_link, so set NULL in here.
		intern->db = NULL;
		kr_printf ("intern->db    --------------------> %d\n", intern->db);
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
