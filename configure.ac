
AC_INIT([ThaleCompiler], [0.0/1.0], [https://example.com])
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_MACRO_DIRS([m4])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])

AC_ARG_ENABLE([debug],
    [AS_HELP_STRING([--enable-debug], [Enable debug build (default is no)])],
    [enable_debug=$enableval],
    [enable_debug=no])

AC_PROG_CC

COMMON_WARNINGS="-Wall -Wextra -Wshadow -Wundef -Wwrite-strings -Wredundant-decls -Wmissing-declarations -Wconversion -Wstrict-overflow=2 -Wfatal-errors -pedantic -Wvla -Wstrict-prototypes"

AS_IF([test "x$enable_debug" = "xyes"],
    [
        # Debug mode: No optimization, all warnings, debug symbols
        CFLAGS="-std=c99 -O0 -g3 $COMMON_WARNINGS -Wdouble-promotion -Wdisabled-optimization -fno-builtin-memcpy -fstack-protector-all"
        LDFLAGS="-g"
    ],
    [
        CFLAGS="-std=c99 -O2 -flto"
        LDFLAGS="-fPIC -s"
    ])

case `$CC --version` in
    *"clang"*)
        AS_IF([test "x$enable_debug" = "xyes"],
            [CFLAGS="$CFLAGS"])
        ;;
    *"gcc"*)
        AS_IF([test "x$enable_debug" = "xyes"],
            [CFLAGS="$CFLAGS -Wduplicated-cond -Wduplicated-branches"])
        ;;
esac

AC_SUBST([CFLAGS])

AC_CONFIG_FILES([Makefile src/Makefile tests/Makefile])
AC_OUTPUT
