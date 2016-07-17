mod_krisp PHP extension
===

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

 * PHP >= 7
  * If you want to PHP <= 5, use [v2 branch](https://github.com/OOPS-ORG-PHP/mod_krisp/tree/v2).
 * [libkrisp](https://github.com/Joungkyun/libkrisp) 3.1 or after.

If you don't want to external library, you can choose [pear_KRISP](https://github.com/OOPS-ORG-PHP/krisp) that is written by pure php cdoe.

## USAGE

See also [wiki](https://github.com/OOPS-ORG-PHP/mod_krisp/wiki) pages.


## COPYRIGHT AND LICENCE

Copyright (C) 2016 by JoungKyun.Kim &lt;https://joungkyun.github.com&gt;

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
