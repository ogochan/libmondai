#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
AC_PREREQ(2.61)
AC_INIT([libmondai],[1.4.2])
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([src])
AC_CONFIG_HEADERS([config.h])
AM_MAINTAINER_MODE

MAJOR_VERSION=`echo AC_PACKAGE_VERSION|sed 's/\(.*\)\.\(.*\)\.\(.*\)/\1/'|sed 's/\([0-9][0-9]*\).*/\1/'`
MINOR_VERSION=`echo AC_PACKAGE_VERSION|sed 's/\(.*\)\.\(.*\)\.\(.*\)/\2/'|sed 's/\([0-9][0-9]*\).*/\1/'`
MICRO_VERSION=`echo AC_PACKAGE_VERSION|sed 's/\(.*\)\.\(.*\)\.\(.*\)/\3/'|sed 's/\([0-9][0-9]*\).*/\1/'`

LTVERSION=`expr $MAJOR_VERSION + $MINOR_VERSION`:$MICRO_VERSION:$MINOR_VERSION

AC_DEFINE_UNQUOTED(PACKAGE,"$PACKAGE","")
AC_DEFINE_UNQUOTED(VERSION,"$VERSION","")

AC_SUBST(MAJOR_VERSION)
AC_SUBST(MINOR_VERSION)
AC_SUBST(MICRO_VERSION)
AC_SUBST(PACKAGE)
AC_SUBST(VERSION)
AC_SUBST(LTVERSION)

# Checks for programs.
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB

AM_PROG_LIBTOOL

# Checks for libraries.

# Checks for header files.
AC_HEADER_DIRENT
AC_HEADER_STDC
AC_CHECK_HEADERS([fcntl.h float.h stdint.h stdlib.h string.h unistd.h wchar.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_BIGENDIAN
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_STRUCT_TM
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_MKTIME
AC_FUNC_MMAP
AC_FUNC_VPRINTF
AC_CHECK_FUNCS([localtime_r memset mkdir munmap strchr strdup strrchr strtol])

dnl ***************************************************************************
dnl Find pkg-config
dnl ***************************************************************************
AC_PATH_PROG(PKG_CONFIG, pkg-config, no)
if test x$PKG_CONFIG = xno ; then
  AC_MSG_ERROR([*** pkg-config not found. See http://pkgconfig.sourceforge.net])
fi

dnl ***************************************************************************
dnl i18n feture
dnl ***************************************************************************

USE_I18N="yes"
LIBMONDAI_I18N=0
AC_ARG_ENABLE(i18n,
    [  --disable-i18n          disable link with iconv [[enable]]],
    [USE_I18N=$enable_i18n])
if test "x$USE_I18N" = "xyes"; then
    AC_DEFINE(WITH_I18N,1,"")
    LIBMONDAI_I18N=1  
fi
AC_SUBST(LIBMONDAI_I18N)

dnl ***************************************************************************
dnl GLib
dnl ***************************************************************************
PKG_CHECK_MODULES(GLIB, glib-2.0 >= 2.0.0)
AC_SUBST(GLIB_CFLAGS)
AC_SUBST(GLIB_LIBS)

dnl ***************************************************************************
dnl libxml
dnl ***************************************************************************
USE_LIBXML="no"
AC_ARG_WITH(xml,
    [  --with-xml=<encode>     Use libxml with encode [[no]]],
    [USE_LIBXML=$withval], [USE_LIBXML="no"])
if test "x$USE_LIBXML" != "xno"; then
    if test "x$USE_LIBXML" = "xyes"; then
        USE_LIBXML="euc-jp"
    fi

    XML_REQUIRED_VERSION=1.0.0
    LIBXML_PACKAGES="libxml"
    AC_MSG_CHECKING(for libxml libraries >= $XML_REQUIRED_VERSION) 
      if $PKG_CONFIG --atleast-version $XML_REQUIRED_VERSION libxml ; then
          XML_CFLAGS=`$PKG_CONFIG --cflags $LIBXML_PACKAGES`
          XML_LIBS=`$PKG_CONFIG --libs $LIBXML_PACKAGES`

          AC_MSG_RESULT(yes)
      else
          AC_MSG_ERROR([
*** libxml $XML_REQUIRED_VERSION or better is required. The latest version of 
*** libxml is always available from http://xmlsoft.org/.
          ])
      fi
  AC_DEFINE(USE_XML,1,"")
  AC_DEFINE_UNQUOTED(LIBXML_CODE,"$USE_LIBXML","")
fi

dnl ***************************************************************************
dnl SOAP
dnl ***************************************************************************
USE_SOAP="yes"
XML2_REQUIRED_VERSION=2.4.0
AC_ARG_ENABLE(soap,
    [  --disable-soap          disable soap [[enable]]],
    [USE_SOAP=$enable_soap])
if test "x$USE_SOAP" != "xno"; then
    AC_DEFINE(USE_SOAP,1,"")
    XML2_REQUIRED_VERSION=2.6.0
fi
AM_CONDITIONAL(SOAP, test x$USE_SOAP != "xno")
dnl ***************************************************************************
dnl libxml2
dnl ***************************************************************************
USE_LIBXML2="yes"
AC_ARG_ENABLE(xml2,
    [  --disable-xml2          disable xml with libxml2 [[enable]]],
    [USE_LIBXML2=$enable_xml2])
if test "x$USE_LIBXML2" != "xno"; then
    LIBXML_PACKAGES="libxml-2.0"
    AC_MSG_CHECKING(for libxml2 libraries >= $XML2_REQUIRED_VERSION) 
      if $PKG_CONFIG --atleast-version $XML2_REQUIRED_VERSION libxml-2.0 ; then
          XML_CFLAGS=`$PKG_CONFIG --cflags $LIBXML_PACKAGES`
          XML_LIBS=`$PKG_CONFIG --libs $LIBXML_PACKAGES`

          AC_MSG_RESULT(yes)
      else
          AC_MSG_ERROR([
*** libxml $XML2_REQUIRED_VERSION or newer is required. The latest version of libxml
*** is always available from http://www.xmlsoft.org/.
*** Please try --disable-soap option if you are installing an old version. 
      ])
     fi
   AC_SUBST(XML_CFLAGS)
   AC_SUBST(XML_LIBS)
   AC_DEFINE(USE_XML,2,"")
   AC_DEFINE(USE_XML2,1,"")
fi

dnl *************************************************************************** 
dnl for Ruby
dnl *************************************************************************** 

enable_ruby="no"
AC_ARG_WITH(ruby, 
[  --with-ruby=PATH        path to ruby],[
    if test "x$withval" = "xyes"; then
      AC_PATH_PROG(RUBY, ruby, no)
    else
      AC_MSG_CHECKING(for ruby)
      RUBY="$withval"
      AC_MSG_RESULT($RUBY)
    fi
    if test "x$RUBY" != "xno"; then
      AC_MSG_CHECKING(for ruby version >= 1.8.0)
      if $RUBY -e 'exit(RUBY_VERSION >= "1.8.0" ? 0 : 1)'; then
        AC_MSG_RESULT(yes)
      else
        AC_MSG_RESULT(no)
        AC_MSG_ERROR(Ruby version is too old)
      fi
      archdir=`$RUBY -r rbconfig -e 'print Config::CONFIG[["archdir"]]'`
      if test -f $archdir/ruby.h; then
        AC_DEFINE(HAVE_RUBY,1,"")
        enable_ruby="yes"
        RUBY_CFLAGS="-I${archdir}"
        RUBY_SHARED=`$RUBY -r rbconfig -e 'print Config::CONFIG[["ENABLE_SHARED"]]'`
        if test "$RUBY_SHARED" = "yes"; then
          RUBY_LIBS=`$RUBY -r rbconfig -e 'print "-L" + Config::CONFIG[["libdir"]] + " " + Config::CONFIG[["LIBRUBYARG"]].gsub(/-L\./, "-L" + Config::CONFIG[["libdir"]]) + " " + Config::CONFIG[["LIBS"]]'`
          RUBY_EXT_LIBS="$RUBY_LIBS"
        else
          RUBY_LIBS=`$RUBY -r rbconfig -e 'print Config::CONFIG[["libdir"]] + "/" + Config::CONFIG[["LIBRUBY_A"]] + " " + Config::CONFIG[["LIBS"]]'`
          RUBY_EXT_LIBS=`$RUBY -r rbconfig -e 'Config::CONFIG[["LIBS"]]'`
        fi
        rubylibdir=`$RUBY -r rbconfig -e 'print Config::CONFIG[["rubylibdir"]]'`
        rubyarchdir=`$RUBY -r rbconfig -e 'print Config::CONFIG[["archdir"]]'`
      else
        AC_MSG_ERROR(ruby.h not found)
      fi
    fi
    AC_SUBST(RUBY) 
    AC_SUBST(RUBY_CFLAGS) 
    AC_SUBST(RUBY_SHARED)
    AC_SUBST(RUBY_LIBS)
    AC_SUBST(RUBY_EXT_LIBS)
    AC_SUBST(rubylibdir) 
    AC_SUBST(rubyarchdir) 
],[
    AC_MSG_CHECKING(for ruby)
    AC_MSG_RESULT(no)
])

dnl ***************************************************************************

AC_CONFIG_FILES([
Makefile
src/Makefile
tools/Makefile
src/libmondai.h
docs/Makefile
libmondai-config
libmondai-config.1
libmondai.pc
])
AC_CONFIG_COMMANDS([default],[chmod +x libmondai-config],[])
AC_OUTPUT

dnl ***************************************************************************
dnl output result
dnl ***************************************************************************

echo "------------------------------------------------------"
echo "libmondai Configuration:"
echo ""
echo "  VERSION:               $VERSION"
echo "  LTVERSION:             $LTVERSION"
echo "  i18n Support:          $USE_I18N"
echo "  libxml Support:        $USE_LIBXML"
echo "  libxml2 Support:       $USE_LIBXML2"
echo "  SOAP Support:          $USE_SOAP"
echo ""
