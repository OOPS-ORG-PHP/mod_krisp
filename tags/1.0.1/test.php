#!/usr/bin/php
<?

#dl ("../../../local/src/mycvs/php/mod_krisp/modules/krisp.so");
$searches = array ('oops.org', 'kornet.net', 'yahoo.com');

/*
 * open krips database
 *
 * resource krisp_open (database)
 *
 * if failed, return FALSE
 */
$c = krisp_open ("/usr/share/krisp/krisp.dat");

if ( $c === FALSE ) {
    /*
     * print krisp error message
     *
     * string krisp_error (void)
     */
	echo krisp_error () . "\n";
	return 1;
}

foreach ( $searches as $v ) :
    /*
     * search krsip database
     *
     * array krisp_search (krsip hdandl, host)
     *   return :
     *       array (
     *               key,
     *               ip,
     *               netmask,
     *               network,
     *               broadcast,
     *               org,        // ISP code
     *               serv,       // ISP name
     *               code,       // GeoIP nation code
     *               nation,     // GeoIP nation name
     *             );
     */
	$r = krisp_search ($c, $v);
	$err = krisp_error ();

	if ( $r === FALSE || $err ) :
		echo "ERROR: $err\n";
		continue;
	endif;

	print_r ($r);

endforeach;

/*
 * close krisp database 
 */
krisp_close ($c);
?>
