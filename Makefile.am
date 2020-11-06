INCLUDE_DIRS = -I$(srcdir)/src -I$(top_srcdir)/src/lib \
			   -I$(top_srcdir)/libdip/src

ACLOCAL_AMFLAGS = -I m4 --install

check-programs: $(check_PROGRAMS)

# add source code files from bootstrap
include src_vars.mk


EXTRA_DIST = \
    LICENSE \
    docs \
    test/integration/scripts \
    misc/p11-kit \
    tools

# Generate the AUTHORS file from git log
AUTHORS :
	$(AM_V_GEN)git log --format='%aN <%aE>' | grep -v 'users.noreply.github.com' | sort | \
	    uniq -c | sort -nr | sed 's/^\s*//' | cut -d" " -f2- > $@
EXTRA_DIST += AUTHORS
CLEANFILES = AUTHORS


# Intentionally empty so INTEGRATION and UNIT tests can append to it.
check_PROGRAMS =
check_SCRIPTS =
