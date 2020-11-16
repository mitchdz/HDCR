# creates Makefile.in in libdip/ and src/
SUBDIRS = libdip src

ACLOCAL_AMFLAGS = -I m4 --install

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


# Intentionally empty so INTEGRATION and UNIT tests can append to it.
check_PROGRAMS =
check_SCRIPTS =
