AC_PREREQ([2.69])
AC_INIT([FULL-PACKAGE-NAME], [VERSION], [BUG-REPORT-ADDRESS])
AM_INIT_AUTOMAKE([foreign])
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])

m4_ifdef([AM_PROG_AR], [AM_PROG_AR])
LT_INIT
AC_CONFIG_MACRO_DIR([m4])

# propagate configure arguments to distcheck
AC_SUBST([DISTCHECK_CONFIGURE_FLAGS],[$ac_configure_args])

AC_PROG_CC
LT_INIT
AM_INIT_AUTOMAKE([foreign subdir-objects])

PKG_INSTALLDIR()

AX_CODE_COVERAGE


# Checks for header files.
AC_CHECK_HEADERS([float.h limits.h stdint.h stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_INT32_T
AC_TYPE_SIZE_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_FUNC_MALLOC
AC_CHECK_FUNCS([pow])

AC_CONFIG_FILES([Makefile])

AC_OUTPUT
