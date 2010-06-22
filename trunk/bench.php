#!/usr/bin/php
<?
/* $Id: bench.php,v 1.1 2010-06-22 03:09:00 oops Exp $ */

if ( ! extension_loaded ('krisp') )
	dl ('krisp.so');

function get_microtime($old, $new) {
	$start = explode(" ", $old);
	$end = explode(" ", $new);

	return sprintf("%.2f", ($end[1] + $end[0]) - ($start[1] + $start[0]));
}

function random_ip () {
	return mt_rand (16777216, 4294967296);
}

if ( $argc != 2 || $argv[1] == '-h' ) {
	error_log ("Usage: {$argv[0]} query_limit");
	exit (1);
}

$c = krisp_open ();

$t1 = microtime ();

for ( $i=0; $i<$argv[1]; $i++ ) {
	$v = random_ip ();
	$v = long2ip ($v);
	$r = krisp_search ($c, $v);
}

$t2 = microtime ();

krisp_close ($c);

printf ("Execute time: %f sec\n", get_microtime ($t1, $t2));

?>
