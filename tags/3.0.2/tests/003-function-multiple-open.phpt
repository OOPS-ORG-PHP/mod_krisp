--TEST--
Check for krisp database multiple open
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
for ( $i=0; $i<2; $i++ ) {
	$kr = krisp_open ();
	$r = krisp_search($kr, '8.8.8.8');
	krisp_close ($kr);
}
?>
--EXPECT--
