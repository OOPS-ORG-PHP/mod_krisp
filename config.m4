dnl $Id: config.m4,v 1.2 2010-06-30 20:07:00 oops Exp $
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

	KRISP_LIBS=$($KRISPCONFIG --libs)
	KRISP_DEFS=$($KRISPCONFIG --defs | sed 's/[ ]*-DHAVE_CONFIG_H[ ]*//g')
	KRISP_SQLITE=$(echo "$KRISP_LIBS" | grep sqlite3)
	KRISP_GEOIP=$(echo "$KRISP_LIBS" | grep GeoIP)

	AC_MSG_CHECKING(check the sqlite version)
	if test -n "$KRISP_SQLITE"; then
		AC_DEFINE(HAVE_LIBSQLITE3, 1,[ ])
		AC_MSG_RESULT([sqlite3])
	else
		AC_MSG_RESULT([sqlite2])
	fi

	AC_MSG_CHECKING(check the geoip support)
	if test -n "$KRISP_GEOIP"; then
		AC_DEFINE(HAVE_LIBGEOIP, 1,[ ])
		AC_MSG_RESULT([yes])
	else
		AC_MSG_RESULT([no])
	fi

	if test -n "$KRISP_DEFS"; then
		KRISP_PARAMETER="$KRISP_PARAMETER $($KRISPCONFIG --defs | sed 's/-DHAVE_CONFIG_H//g')"
	fi

	KRISP_SHARED_LIBADD="$KRISP_LIBS"
	PHP_SUBST(KRISP_SHARED_LIBADD)
	PHP_SUBST(KRISP_PARAMETER)

	dnl if php version is under 4.2.x, use PHP_EXTENSION
	dnl bug over php 4.2.x, use PHP_NEW_EXTENSION
	dnl PHP_EXTENSION(krisp, $ext_shared)
	PHP_NEW_EXTENSION(krisp, php_krisp.c, $ext_shared,, \\$(KRISP_PARAMETER))
fi
