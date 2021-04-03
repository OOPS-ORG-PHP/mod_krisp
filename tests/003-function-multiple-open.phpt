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

unset ($kr);

$kr = krisp_open ();
$r = krisp_search ($kr, '8.8.8.8');
echo $r->ccode . "\n";
$r = krisp_search ($kr, '168.126.63.1');
echo $r->ccode . "\n";

$kr1 = krisp_open ();
$r = krisp_search ($kr1, '168.126.63.1');
echo $r->ccode . "\n";
krisp_close ($kr1);

krisp_close ($kr);
?>
--EXPECT--
US
KR
KR
