# add source code files from bootstrap
include src_vars.mk

#libdip and libhdcr
noinst_LTLIBRARIES = libhdcr.la
libhdcr_la_SOURCES = $(LIB_HDCR_SRC) $(LIB_DIP_SRC)
libhdcr_la_CFLAGS = $(AM_CFLAGS) -I$(top_srcdir)/src/libdip #add libdip header file location

bin_PROGRAMS = hdcr
hdcr_SOURCES = $(HDCR_SRC) $(LIB_DIP_H) $(LIB_HDCR_H)
hdcr_CFLAGS = $(AM_CFLAGS) -I./src/lib -I./src/libdip -I./
hdcr_LDADD = libhdcr.a
