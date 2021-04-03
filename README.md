mod_krisp PHP extension
===
[![PHP v3.01](https://img.shields.io/badge/license-php_v3.01-blue.svg)](http://www.php.net/license)
![GitHub download](https://img.shields.io/github/downloads/OOPS-ORG-PHP/mod_krisp/total.svg)
[![GitHub last release](https://img.shields.io/github/release/OOPS-ORG-PHP/mod_krisp.svg)](https://github.com/OOPS-ORG-PHP/mod_krisp/releases)
[![GitHub closed issues](https://img.shields.io/github/issues-closed-raw/OOPS-ORG-PHP/mod_krisp.svg)](https://github.com/OOPS-ORG-PHP/mod_krisp/issues?q=is%3Aissue+is%3Aclosed)
[![GitHub closed pull requests](https://img.shields.io/github/issues-pr-closed-raw/OOPS-ORG-PHP/mod_krisp.svg)](https://github.com/OOPS-ORG-PHP/mod_krisp/pulls?q=is%3Apr+is%3Aclosed)


## DESCRIPTION

This is php extension that is [libkrisp](https://github.com/Joungkyun/libkrisp) PHP frontend.
See also [libkrisp](https://github.com/Joungkyun/libkrisp) project.

***mod_krisp*** extension provides network calcluration and searching ISP/country information
for given ip address.

## INSTALLATION

To install this extension type the following:

```bash
[root@host mod_krisp]$ phpize
[root@host mod_krisp]$ ./configure
[root@host mod_krisp]$ make && make install
```

## DEPENDENCIES

This module requires these libraries:

 * PHP <= 5.4
  * If you want to PHP >= 7, use [master branch](https://github.com/OOPS-ORG-PHP/mod_krisp).
 * [libkrisp](https://github.com/Joungkyun/libkrisp) 3.1 or after.

If you don't want to external library, you can choose [pear_KRISP](https://github.com/OOPS-ORG-PHP/krisp) that is written by pure php cdoe.

## USAGE

See also [wiki](https://github.com/OOPS-ORG-PHP/mod_krisp/wiki) pages.


## COPYRIGHT AND LICENCE

Copyright 2021. JoungKyun.Kim all rights reserved.

This source file is subject to version 3.01 of the PHP license, that is bundled with this package in the file LICENSE, and is available through the world-wide-web at the following url:

    [http://www.php.net/license/3_01.txt](http://www.php.net/license/3_01.txt)

If you did not receive a copy of the PHP license and are unable to obtain it through the world-wide-web, please send a note to license@php.net so we can mail you a copy immediately.

Also, legacy versions of GPL also change to PHP V3.01.
