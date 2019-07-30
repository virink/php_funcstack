dnl config.m4 for extension funcstack

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary.

dnl If your extension references something external, use 'with':

dnl PHP_ARG_WITH([funcstack],
dnl   [for funcstack support],
dnl   [AS_HELP_STRING([--with-funcstack],
dnl     [Include funcstack support])])

dnl Otherwise use 'enable':

PHP_ARG_ENABLE([funcstack],
  [whether to enable funcstack support],
  [AS_HELP_STRING([--enable-funcstack],
    [Enable funcstack support])],
  [no])

if test "$PHP_FUNCSTACK" != "no"; then
  dnl Write more examples of tests here...

  dnl Remove this code block if the library does not support pkg-config.
  dnl PKG_CHECK_MODULES([LIBFOO], [foo])
  dnl PHP_EVAL_INCLINE($LIBFOO_CFLAGS)
  dnl PHP_EVAL_LIBLINE($LIBFOO_LIBS, FUNCSTACK_SHARED_LIBADD)

  dnl If you need to check for a particular library version using PKG_CHECK_MODULES,
  dnl you can use comparison operators. For example:
  dnl PKG_CHECK_MODULES([LIBFOO], [foo >= 1.2.3])
  dnl PKG_CHECK_MODULES([LIBFOO], [foo < 3.4])
  dnl PKG_CHECK_MODULES([LIBFOO], [foo = 1.2.3])

  dnl Remove this code block if the library supports pkg-config.
  dnl --with-funcstack -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/funcstack.h"  # you most likely want to change this
  dnl if test -r $PHP_FUNCSTACK/$SEARCH_FOR; then # path given as parameter
  dnl   FUNCSTACK_DIR=$PHP_FUNCSTACK
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for funcstack files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       FUNCSTACK_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$FUNCSTACK_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the funcstack distribution])
  dnl fi

  dnl Remove this code block if the library supports pkg-config.
  dnl --with-funcstack -> add include path
  dnl PHP_ADD_INCLUDE($FUNCSTACK_DIR/include)

  dnl Remove this code block if the library supports pkg-config.
  dnl --with-funcstack -> check for lib and symbol presence
  dnl LIBNAME=FUNCSTACK # you may want to change this
  dnl LIBSYMBOL=FUNCSTACK # you most likely want to change this

  dnl If you need to check for a particular library function (e.g. a conditional
  dnl or version-dependent feature) and you are using pkg-config:
  dnl PHP_CHECK_LIBRARY($LIBNAME, $LIBSYMBOL,
  dnl [
  dnl   AC_DEFINE(HAVE_FUNCSTACK_FEATURE, 1, [ ])
  dnl ],[
  dnl   AC_MSG_ERROR([FEATURE not supported by your funcstack library.])
  dnl ], [
  dnl   $LIBFOO_LIBS
  dnl ])

  dnl If you need to check for a particular library function (e.g. a conditional
  dnl or version-dependent feature) and you are not using pkg-config:
  dnl PHP_CHECK_LIBRARY($LIBNAME, $LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $FUNCSTACK_DIR/$PHP_LIBDIR, FUNCSTACK_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_FUNCSTACK_FEATURE, 1, [ ])
  dnl ],[
  dnl   AC_MSG_ERROR([FEATURE not supported by your funcstack library.])
  dnl ],[
  dnl   -L$FUNCSTACK_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(FUNCSTACK_SHARED_LIBADD)

  dnl In case of no dependencies
  AC_DEFINE(HAVE_FUNCSTACK, 1, [ Have funcstack support ])

  PHP_NEW_EXTENSION(funcstack, funcstack.c, $ext_shared)
fi
