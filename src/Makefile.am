
SUBDIRS = libxsldbg libqtnotifier


# set the include path found by configure
# cheet a bit because I need DOCS_PATH defined
INCLUDES= $(all_includes) -DDOCS_PATH=\"$(HTML_DIR)/xsldbg/\" \
 -DVERSION=\"@VERSION@\" $(LIBXML_CFLAGS) $(LIBXSLT_CFLAGS)

xsldbg_SOURCES = \
	main.c	\
	simpleio.c

EXTRA_DIST = \
	main.c	   \
	simpleio.c \
	readme.txt	


bin_PROGRAMS = xsldbg

xsldbg_DEPENDENCIES  = ./libxsldbg/.libs/libxsldbg.a

./libxsldbg/.libs/libxsldbg.a :
	cd libxsldbg; $(MAKE) 

# use of configure defined libraries
xsldbg_LDADD = $(XSLDBG_LIBS)

# the library search path.
xsldbg_LDFLAGS = $(all_libraries) -L ./libxsldbg/.libs/


clean:
	rm -f *.o

very-clean:clean
	rm -f *.*~
	rm -f *~
	rm -f Makefile.in
	rm -f Makefile

