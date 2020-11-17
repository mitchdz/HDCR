# add source code files from bootstrap
include src_vars.mk

AM_CFLAGS = -I$(top_srcdir)/src -I$(top_srcdir)/lib

noinst_LTLIBRARIES = libhdcr.la
libhdcr_la_SOURCES = $(LIB_HDCR_SRC)
libhdcr_la_LIBADD = libdip.la

bin_PROGRAMS = hdcr
hdcr_SOURCES = $(HDCR_SRC) $(LIB_DIP_H) $(LIB_HDCR_H)
hdcr_LDADD = libhdcr.la
