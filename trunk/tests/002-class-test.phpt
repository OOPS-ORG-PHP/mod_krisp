--TEST--
Check for krisp database open API
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
	$kr->mtimeInterval (0);
	$kr->debug ();
	$kr->debug (false);

	$r = $kr->search ('8.8.8.8');

	if ( $r === false )
		throw new KRISPException ('search failed', E_WARNING);

	echo $r->ccode . "\n";

	$kr->close ();

} catch ( KRISPException $e ) {
    fprintf (STDERR, "%s\n", $e->getMessage ());
	$err = preg_split ('/\r?\n/', $e->getTraceAsString ());
    print_r ($err);
}
?>
--EXPECT--
US
