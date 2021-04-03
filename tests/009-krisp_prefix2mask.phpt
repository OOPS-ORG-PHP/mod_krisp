--TEST--
Check for krisp_krisp_prefix2mask function
--SKIPIF--
<?php
if ( ! extension_loaded ('krisp') ) {
	print 'skip';
}
?>
--POST--
--GET--
--INI--
--FILE--
<?php
echo krisp_prefix2mask (25);
?>
--EXPECT--
255.255.255.128
