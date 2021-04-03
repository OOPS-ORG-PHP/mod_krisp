--TEST--
Check for krisp_netmask function
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
$mask = krisp_netmask ('192.168.10.44', '192.168.10.58');
echo "{$mask->mask}\n{$mask->prefix}";
?>
--EXPECT--
255.255.255.224
27
