<?php
/** @--generate-function-entries **/

function krisp_buildver (): string { }
function krisp_version (): string { }
function krisp_uversion (): string { }
function krisp_open (?string $datafile = null, ?string &$err = null): KRISP { }
function krisp_search (KRISP $link, string $host): object|false { }
function krisp_search_ex (KRISP $link, string $host): object|false { }
function krisp_close (KRISP $link): bool { }
function krisp_netmask (string $start, string $end): object { }
function krisp_network (string $ip, string $mask): string|false { }
function krisp_broadcast (string $ip, string $mask): string|false { }
function krisp_prefix2mask (int $prefix): string { }
function krisp_mask2prefix (string $mask): int { }
function krisp_set_mtime_interval (resource $link, int $sec): bool { }
function krisp_set_debug (resource $link, int $switch = true): bool { }

class KRISP {
	/**
	 * @return KRISP
	 * @alias krisp_open
	 */
	public function __construct (?string $datafile = null) { }
	/**
	 * @return object|false
	 * @alias krisp_search
	 */
	public function search (string $host): object|false { }
	/**
	 * @return object|false
	 * @alias krisp_search_ex
	 */
	public function search_ex (string $host): object|false { }
	/**
	 * @return bool
	 * @alias krisp_close
	 */
	public function close (): bool { }
	/**
	 * @return bool
	 * @alias krisp_set_mtime_interval
	 */
	public function mtimeInterval (int $sec): bool { }
	/**
	 * @return bool
	 * @alias krisp_set_debug
	 */
	public function debug (int $switch = true): bool { }
}
?>
