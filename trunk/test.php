#!/usr/bin/php
<?
/* $Id: test.php,v 1.6 2010-06-18 13:35:54 oops Exp $ */

if ( ! extension_loaded ('krisp') )
	dl ('krisp.so');

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
	break;
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
$prefix = krisp_long2prefix (ip2long ($mask));
echo " * Prefix     : {$prefix}\n";
$mask = long2ip (krisp_prefix2long ($prefix));
echo " * Mask       : {$mask}\n";
?>
