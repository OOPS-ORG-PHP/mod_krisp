#!/usr/bin/php
<?php
/* $Id$ */

/*
 * KRISP OOP style test file
 */

if ( ! extension_loaded ('krisp') ) {
	if ( version_compare (PHP_VERSION, '5.4.0', '<') ) {
		dl ('krisp.so');
	} else {
		fprintf (STDERR, "KRISP extension is not loaded\n");
		exit (1);
	}
}

$searches = array ('oops.org', 'kornet.net', 'yahoo.com', 'kldp.org');

try {
	/*
	 * open krips database
	 *
	 * resource krisp_open (database)
	 *
	 * if failed, return FALSE
	 */
	#$kr = new KRISP ("/usr/share/krisp/krisp.dat", $err);
	#$kr = new KRISP ('asdf', $err);
	#$kr = new KRISP ('', $err);
	#$kr = new KRISP (NULL, $err);
	$kr = new KRISP ();

	#$kr->mtimeInterval (0);
	#$kr->debug ();

	foreach ( $searches as $v ) {
		/*
		 * search krsip database
		 *
		 * object KRISP::search (host)
		 * object KRISP::searchEx (host[, table='krisp'])
		 */
		$r = $kr->search ($v);

		if ( $r === false )
			continue;

		print_r ($r);
		#break;
	}

	/*
	 * close krisp database 
	 */
	$kr->close ();
} catch ( KRISPException $e ) {
    fprintf (STDERR, "%s\n", $e->getMessage ());
	$err = preg_split ('/\r?\n/', $e->getTraceAsString ());
    print_r ($err);
}
?>
