AUTOMAKE_OPTIONS = subdir-objects

include_HEADERS = include/lex.h include/error.h

bin_PROGRAMS = thale
thale_SOURCES = lex.c error.c thale.c

AM_CPPFLAGS = -I$(srcdir)/include
AM_CFLAGS = $(CFLAGS)