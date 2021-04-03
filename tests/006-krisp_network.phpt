--TEST--
Check for krisp_network function
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
echo krisp_network ('192.168.10.44', '255.255.255.224');
?>
--EXPECT--
192.168.10.32
