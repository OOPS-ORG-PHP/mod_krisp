dnl $Id$
dnl config.m4 for extension krisp

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

PHP_ARG_WITH(krisp, for krisp support,
[  --with-krisp            Include krisp support])

if test "$PHP_KRISP" != "no"; then
	AC_DEFINE(HAVE_LIBKRISP,1,[ ])

	KRISP_PARAMETER="$CPPLAGS"
	PHP_SUBST(LDFLAGS)
	PHP_SUBST(CPPFLAGS)

	AC_MSG_CHECKING([check the PHP mininum version for krisp (>=7.0.0)])
	PHP_CHKVER=`
		awk '/^#define PHP_VERSION_ID/ { print $NF; }' $phpincludedir/main/php_version.h 2> /dev/null
	`
	PHP_CURVER=`
		awk '/^#define PHP_VERSION /  { print gensub(/\"/, "", "g", $NF); }' \
			$phpincludedir/main/php_version.h 2> /dev/null
	`
	AC_MSG_RESULT([$PHP_CURVER])

	if test -z $PHP_CHKVER || test $PHP_CHKVER -lt 70000 ; then
		AC_MSG_ERROR([The krisp extension is unsupported PHP $PHP_CURVER. Use PHP 7.0.0 or after!])
	fi


	AC_MSG_CHECKING(check the krisp-config path)
	KRISPCONFIG=
	if test -f "$PHP_KRISP"; then
		KRISPCONFIG=$PHP_KRISP
	else
		for i in /bin /sbin /usr/bin /usr/sbin /usr/local/bin /usr/local/sbin \
				/opt/bin /opt/sbin /usr/local/libkrisp/bin /usr/local/libkrisp/sbin \
				/usr/local/bin/krisp/bin /usr/local/bin/krisp/sbin
		do
			if test -f "$i/krisp-config"; then
				KRISPCONFIG="$i/krisp-config"
				break
			fi
		done
	fi

	if test -n "$KRISPCONFIG" ; then
		AC_MSG_RESULT([$KRISPCONFIG])
	else
		AC_MSG_ERROR([can't find. specify --with-krisp=/path/krisp-config])
	fi

	LIBKRISP_MINIMAL_VERSION="3.1.0"
	LIBKRISP_VERSION=$($KRISPCONFIG --version)
	LIBKRISP_MAJOR=$(echo $LIBKRISP_VERSION | sed 's/\..*//g')
	LIBKRISP_MINOR=$(echo $LIBKRISP_VERSION | sed 's/^[[0-9]]\+\.\([[0-9]]\+\)\.[[0-9]]\+$/\1/g')
	LIBKRISP_PATCH=$(echo $LIBKRISP_VERSION | sed 's/^.*\.\([[0-9]]\+\)$/\1/g')

	AC_MSG_CHECKING(check the libkrisp version)
	if test -n "$LIBKRISP_VERSION"; then
		if test $LIBKRISP_MAJOR -lt 3 ; then
			AC_MSG_ERROR([Current is $LIBKRISP_VERSION. Requires over $LIBKRISP_MINIMAL_VERSION])
		fi
		if test $LIBKRISP_MINOR -lt 1 ; then
			AC_MSG_ERROR([Current is $LIBKRISP_VERSION. Requires over $LIBKRISP_MINIMAL_VERSION])
		fi
		AC_MSG_RESULT([$LIBKRISP_VERSION])
	else
		AC_MSG_ERROR([Unknown libkrisp version. Requires over $LIBKRISP_MINIMAL_VERSION])
	fi

	KRISP_LIBS=$($KRISPCONFIG --link-ld)
	KRISP_DEFS=$($KRISPCONFIG --cppflags | sed 's/[ ]*-DHAVE_CONFIG_H[ ]*//g')
	KRISP_SQLITE=$(echo "$KRISP_LIBS" | grep sqlite3)

	AC_MSG_CHECKING(check the sqlite version)
	if test -n "$KRISP_SQLITE"; then
		AC_DEFINE(HAVE_LIBSQLITE3, 1,[ ])
		AC_MSG_RESULT([sqlite3])
	else
		AC_MSG_RESULT([sqlite2])
	fi

	if test -n "$KRISP_DEFS"; then
		KRISP_PARAMETER="$KRISP_PARAMETER $($KRISPCONFIG --cppflags | sed 's/-DHAVE_CONFIG_H//g')"
	fi

	KRISP_SHARED_LIBADD="$KRISP_LIBS"
	PHP_SUBST(KRISP_SHARED_LIBADD)
	PHP_SUBST(KRISP_PARAMETER)

	dnl if php version is under 4.2.x, use PHP_EXTENSION
	dnl bug over php 4.2.x, use PHP_NEW_EXTENSION
	dnl PHP_EXTENSION(krisp, $ext_shared)
	PHP_NEW_EXTENSION(krisp, php_krisp.c, $ext_shared,, \\$(KRISP_PARAMETER))
fi
