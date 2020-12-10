# Usage:
# make run          <- run program
# make debug        <- debug program
# make submission   <- auto generate submission folder

################################ VARIABLES #####################################
PROGNAME = hdcr
INPUT_FILE = input/address.png
OUTPUT_FILE = out/address_threshold.png

################################ COMPILER ######################################
LINKER=gcc
LDFLAGS =-Llibdip
LDLIBS =-ldip -lpng -lz -fopenmp -lm
CC =gcc
CFLAGS = -U__STRICT_ANSI__ -fopenmp -I$(LIBDIR) -I$(SRCDIR) -I$(SRCDIR)/lib -Ilibdip/src
CFLAGS += -DDEBUG -g -Wall
################################ DIRECTORIES ###################################
SUBMISSIONDIR =submission
OUTDIR =out
SRCDIR =src
OBJDIR =.obj
HDCR_OBJDIR =.hdcr_obj
BINDIR =.bin
LIBDIR =/usr/lib
LIBSDIR =.libs

################################ FILES #########################################
SOURCES := $(wildcard $(SRCDIR)/*.c)
INCLUDES := $(wildcard $(SRCDIR)/*.h)
OBJECTS := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

LIB_HDCR_SOURCES = $(wildcard $(SRCDIR)/lib/*.c)
LIB_HDCR_INCLUDES := $(wildcard $(SRCDIR)/lib/*.h)
LIB_HDCR_OBJECTS := $(LIB_HDCR_SOURCES:$(SRCDIR)/lib/%.c=$(HDCR_OBJDIR)/%.o)

############################DEFAULT DIRECTIVE ##################################
all: create_dirs libdip $(BINDIR)/$(PROGNAME)

$(BINDIR)/$(PROGNAME): $(OBJECTS) libdip libhdcr
	g++ -c src/lib/cppthresh.cpp -o .obj/cppthresh.opp $(CFLAGS) $(shell pkg-config opencv4 --cflags --libs) -std=c++11
	g++ -c src/lib/cppOpenCV2Helper.cpp -o .obj/cppOpenCV2Helper.opp $(CFLAGS) $(shell pkg-config --cflags --libs opencv4) -std=c++11
	g++ -o $@ .obj/cppthresh.opp .obj/cppOpenCV2Helper.opp $(OBJECTS) $(CFLAGS) -L.libs $(LDFLAGS) -lhdcr $(LDLIBS) $(shell pkg-config opencv4 --cflags --libs) -std=c++11

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c $(SRCDIR)/%.h
	$(CC) $(CFLAGS) -c $< -o $@ $(LDLIBS)
################################ TARGETS #######################################
.PHONY: libhdcr
libhdcr: $(LIB_HDCR_OBJECTS)
	ar rc $(LIBSDIR)/$@.a $^

$(LIB_HDCR_OBJECTS): $(HDCR_OBJDIR)/%.o : $(SRCDIR)/lib/%.c $(SRCDIR)/lib/%.h
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: libdip
libdip:
	$(MAKE) -C libdip

.PHONY: run
run: all
	./$(BINDIR)/$(PROGNAME) -i demo/Circuit2-1.png -o demo/Circuit2-1_out.png -D

.PHONY: benchmark
benchmark: all
	./$(BINDIR)/$(PROGNAME) -i demo/Circuit2-1.png -o demo/Circuit2-1_out.png -b

.PHONY: create_dirs
create_dirs:
	mkdir -p $(OBJDIR)
	mkdir -p $(BINDIR)
	mkdir -p $(LIBSDIR)
	mkdir -p $(HDCR_OBJDIR)
	mkdir -p out

.PHONY: clean
clean:
	rm -rf $(OBJDIR) $(BINDIR) $(SUBMISSIONDIR) $(OUTDIR) $(HDCR_OBJDIR)
