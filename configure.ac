#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
# Minimum version of autoconf required
AC_PREREQ(2.64)

m4_define([lib_version_major], [1])
m4_define([lib_version_minor], [0])
m4_define([lib_version_patch], [0])
m4_define([lib_version],
          [lib_version_major.lib_version_minor.lib_version_patch])

AC_INIT([libflowtuple], [lib_version], [mweiman@merit.edu])
AC_CONFIG_FILES([Makefile
                 lib/libflowtuple/Makefile])
AC_CONFIG_MACRO_DIRS([m4])

AC_CANONICAL_HOST
AM_INIT_AUTOMAKE([1.11 foreign])
AM_SILENT_RULES([yes])

LT_INIT([disable-static])

LIB_VERSION_INFO="lib_version_major:lib_version_minor:lib_version_patch"

AC_PROG_CC_C99
AC_PROG_INSTALL

AC_SUBST(LIB_VERSION_INFO)

AC_SEARCH_LIBS([wandio_create], [wandio trace], ,[AC_MSG_ERROR(
               [libwandio required (included with libtrace since 3.0.15)]
               )])

AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

AC_FUNC_MALLOC

AC_OUTPUT
