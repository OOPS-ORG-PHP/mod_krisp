#!/usr/bin/php
<?php
/* $Id$ */

if ( ! extension_loaded ('krisp') ) {
	if ( version_compare (PHP_VERSION, '5.4.0', '<') ) {
		dl ('krisp.so');
	} else {
		fprintf (STDERR, "KRISP extension is not loaded\n");
		exit (1);
	}
}

$searches = array ('oops.org', 'kornet.net', 'yahoo.com', 'kldp.org');

/*
 * open krips database
 *
 * resource krisp_open (database)
 *
 * if failed, return FALSE
 */
#$c = krisp_open ("/usr/share/krisp/krisp.dat", $err);
#$c = krisp_open ('asdf', $err);
#$c = krisp_open ('', $err);
#$c = krisp_open (NULL, $err);
$c = krisp_open ();

if ( $c === false ) {
	echo "ERROR: {$err}\n";
	return 1;
}

krisp_set_mtime_interval ($c, 0);
krisp_set_debug ($c);

foreach ( $searches as $v ) {
    /*
     * search krsip database
     *
     * object krisp_search (krsip resource, host)
     * object krisp_search_ex (krsip resource, host[, table='krisp'])
     */
	$r = krisp_search ($c, $v);

	if ( $r === false )
		continue;

	print_r ($r);
	krisp_set_debug ($c, false);
	//break;
}

/*
 * close krisp database 
 */
krisp_close ($c);

echo "Get subnet mask with 192.168.10.44 - 192.168.10.58 => ";
$mask = krisp_netmask ('192.168.10.44', '192.168.10.58');
echo "{$mask->mask} : {$mask->prefix}\n";
echo " * network address   : " . krisp_network ('192.168.10.44', $mask->mask) . "\n";
echo " * broadcast address : " . krisp_broadcast('192.168.10.44', $mask->mask) . "\n";

echo "\nNetmask <-> Prefix\n";
unset ($mask);
$mask = '255.255.255.128';
echo " * Given Mask : {$mask}\n";
$prefix = krisp_mask2prefix ($mask);
echo " * Prefix     : {$prefix}\n";
$mask = krisp_prefix2mask ($prefix);
echo " * Mask       : {$mask}\n";
?>
