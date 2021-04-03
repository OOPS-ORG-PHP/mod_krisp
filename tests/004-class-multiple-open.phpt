--TEST--
Check for krisp class multiple open
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
try {
	$kr = new KRISP ();
	$r = $kr->search('8.8.8.8');
	echo $r->ccode . "\n";
	$kr->close ();
	unset ($kr);

	$kr = new KRISP ();
	$r = $kr->search('8.8.8.8');
	echo $r->ccode . "\n";
	$kr->close ();
	unset ($kr);

	$kr = new KRISP ();
	$kr1 = new KRISP ();
	$r = $kr->search('8.8.8.8');
	$r1 = $kr1->search('8.8.8.8');
	echo $r->ccode . "\n";
	echo $r1->ccode . "\n";
	$kr->close ();
	$kr1->close ();
} catch ( KRISPException $e ) {
    fprintf (STDERR, "%s\n", $e->getMessage ());
	$err = preg_split ('/\r?\n/', $e->getTraceAsString ());
    print_r ($err);
}
?>
--EXPECT--
US
US
US
US
