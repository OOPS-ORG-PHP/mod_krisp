/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 0d995f370bde364a9df7d065b51e11d4105beae5 */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_krisp_buildver, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

#define arginfo_krisp_version arginfo_krisp_buildver

#define arginfo_krisp_uversion arginfo_krisp_buildver

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_krisp_open, 0, 0, KRISP, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, datafile, IS_STRING, 1, "null")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(1, err, IS_STRING, 1, "null")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(arginfo_krisp_search, 0, 2, MAY_BE_OBJECT|MAY_BE_FALSE)
	ZEND_ARG_OBJ_INFO(0, link, KRISP, 0)
	ZEND_ARG_TYPE_INFO(0, host, IS_STRING, 0)
ZEND_END_ARG_INFO()

#define arginfo_krisp_search_ex arginfo_krisp_search

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_krisp_close, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_OBJ_INFO(0, link, KRISP, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_krisp_netmask, 0, 2, IS_OBJECT, 0)
	ZEND_ARG_TYPE_INFO(0, start, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, end, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(arginfo_krisp_network, 0, 2, MAY_BE_STRING|MAY_BE_FALSE)
	ZEND_ARG_TYPE_INFO(0, ip, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, mask, IS_STRING, 0)
ZEND_END_ARG_INFO()

#define arginfo_krisp_broadcast arginfo_krisp_network

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_krisp_prefix2mask, 0, 1, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, prefix, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_krisp_mask2prefix, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, mask, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_krisp_set_mtime_interval, 0, 2, _IS_BOOL, 0)
	ZEND_ARG_OBJ_INFO(0, link, resource, 0)
	ZEND_ARG_TYPE_INFO(0, sec, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_krisp_set_debug, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_OBJ_INFO(0, link, resource, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, switch, IS_LONG, 0, "true")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_KRISP___construct, 0, 0, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, datafile, IS_STRING, 1, "null")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(arginfo_class_KRISP_search, 0, 1, MAY_BE_OBJECT|MAY_BE_FALSE)
	ZEND_ARG_TYPE_INFO(0, host, IS_STRING, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_KRISP_search_ex arginfo_class_KRISP_search

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_KRISP_close, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_KRISP_mtimeInterval, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, sec, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_KRISP_debug, 0, 0, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, switch, IS_LONG, 0, "true")
ZEND_END_ARG_INFO()
