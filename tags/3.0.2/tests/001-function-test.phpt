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
function result($v, $err = '') {
	if ( $v === true )
		echo "OK\n";
	else {
		if ( $err )
			echo $err . "\n";
		else
			echo "Failure\n";
	}
}

function krisp_safe_close($v) {
	if (is_resource($v))
		krisp_close($v);
}

$kr = krisp_open();
result (is_resource($kr));
krisp_safe_close($kr);

$kr = krisp_open(NULL, $err);
result (is_resource($kr), $err);
krisp_safe_close($kr);

$kr = krisp_open('', $err);
result (is_resource($kr), $err);
krisp_safe_close($kr);

$kr = @krisp_open('/usr/share/krisp/krisp1.dat', $err);
result (is_resource($kr), $err);
krisp_safe_close($kr);

$kr = krisp_open('/usr/share/krisp/krisp.dat', $err);
result (is_resource($kr), $err);

krisp_set_mtime_interval ($kr, 0);
krisp_set_debug ($kr, false);

$r = @krisp_search($kr, '8.8.8.8');
result (($r->ccode == 'US'));
krisp_safe_close($kr);

?>
--EXPECT--
OK
OK
OK
kr_open:: Can't find database (/usr/share/krisp/krisp1.dat)
OK
OK
