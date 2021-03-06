/*
 * Copyright (C) 2019 by JoungKyun.Kim <https://joungkyun.github.com>
 *    all rights resolved.
 *
 * This file is part of libkrisp
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

/**
 * Project: mod_krisp :: KRISP database php extension
 * File:    php_krisp_class.c
 *
 * Copyright (c) 2019 JoungKyun.Kim
 *
 * LICENSE: GPL
 *
 * @category    Database
 * @package     mod_krisp
 * @author      JoungKyun.Kim <https://joungkyun.github.com>
 * @copyright   2019 OOPS.org
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
