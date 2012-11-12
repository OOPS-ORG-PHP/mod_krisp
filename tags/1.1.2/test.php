#!/usr/bin/php
<?
if ( ! extension_loaded ('krisp') )
	dl ('krisp.so');

$searches = array ('oops.org', 'kornet.net', 'yahoo.com');

/*
 * open krips database
 *
 * resource krisp_open (database)
 *
 * if failed, return FALSE
 *
 * whether search geoip city database
 * geoip city database requires big memory, so, don't use with fork model as apache
 * $geoip['geocity'] = 1;
 *
 * GeoIP database open type
 *
 * geoip : string dopen or constant GEOIP_OPENTYPE
 * geoipisp: string iopen or constant GEOISP_OPENTYPE
 * geoipcity: string copen or constant GEOCITY_OPENTYPE
 *
 * $geoip['dopen'] = GEOIP_MEMORY_CACHE | GEOIP_CHECK_CACHE;
 * $geoip['iopen'] = GEOIP_INDEX_CACHE | GEOIP_CHECK_CACHE;
 * $geoip['copen'] = GEOIP_INDEX_CACHE | GEOIP_CHECK_CACHE;
 *  or follows
 * $geoip[GEOIP_OPENTYPE] = GEOIP_MEMORY_CACHE | GEOIP_CHECK_CACHE;
 * $geoip[GEOISP_OPENTYPE] = GEOIP_MEMORY_CACHE | GEOIP_CHECK_CACHE;
 * $geoip[GEOCITY_OPENTYPE] = GEOIP_MEMORY_CACHE | GEOIP_CHECK_CACHE;
 *
 */
$geoip = array ('geocity' => 1,
				GEOIP_OPENTYPE => GEOIP_MEMORY_CACHE | GEOIP_CHECK_CACHE,
				GEOISP_OPENTYPE => GEOIP_MEMORY_CACHE | GEOIP_CHECK_CACHE,
				GEOCITY_OPENTYPE => GEOIP_MEMORY_CACHE | GEOIP_CHECK_CACHE);
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
     *               gcode,      // GeoIP nation code
     *               gname,      // GeoIP nation name
     *               gcity,      // GeoIP nation name
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
