--TEST--
Check for krisp_krisp_mask2prefix function
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
echo krisp_mask2prefix ('255.255.255.128');
?>
--EXPECT--
25
