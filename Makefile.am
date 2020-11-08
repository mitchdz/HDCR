ACLOCAL_AMFLAGS = -I m4 --install

#check-programs: $(check_PROGRAMS)

# add source code files from bootstrap
include src_vars.mk

EXTRA_DIST = \
    LICENSE \
    docs \
    test

# Generate the AUTHORS file from git log
AUTHORS :
	$(AM_V_GEN)git log --format='%aN <%aE>' | grep -v 'users.noreply.github.com' | sort | \
	    uniq -c | sort -nr | sed 's/^\s*//' | cut -d" " -f2- > $@
EXTRA_DIST += AUTHORS
CLEANFILES = AUTHORS


#libdip and libhdcr
noinst_LIBRARIES = libhdcr.a
libhdcr_a_CFLAGS = $(CFLAGS) -I$(LIB_HDCR_H)
libhdcr_a_SOURCES = $(LIB_HDCR_SRC)

noinst_LIBRARIES += libdip.a
libdip_a_CFLAGS = $(CFLAGS) -I$(LIB_DIP_H)
libdip_a_SOURCES = $(LIB_DIP_SRC)


bin_PROGRAMS = hdcr
hdcr_CFLAGS = $(CFLAGS) -I$(HDCR_SRC)
hdcr_SOURCES = $(HDCR_SRC)
hdcr_LIBADD = libdip libhdcr

# Intentionally empty so INTEGRATION and UNIT tests can append to it.
check_PROGRAMS =
check_SCRIPTS =
