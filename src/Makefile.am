
SUBDIRS=libxsldbg libqtnotifier


# set the include path found by configure
# cheet a bit because I need DOCS_PATH defined
INCLUDES= $(all_includes) -DDOCS_PATH=\"$(HTML_DIR)/xsldbg/\" \
 -DVERSION=\"@VERSION@\" $(LIBXML_CFLAGS) $(LIBXSLT_CFLAGS)

xsldbg_SOURCES = \
	main.c	\
	simpleio.c

EXTRA_DIST = \
	main.c	   	\
	simpleio.c 	\
	readme.txt 		

bin_PROGRAMS = xsldbg


xsldbg_DEPENDENCIES  = $(top_builddir)/src/libxsldbg/libxsldbg.la

$(top_builddir)/src/libxsldbg/libxsldbg.la:
	cd $(top_builddir)/src/libxsldbg; $(MAKE) 

# use of configure defined libraries
xsldbg_LDADD = $(top_builddir)/src/libxsldbg/libxsldbg.la $(XSLDBG_TEST_LIBS)

# the library search path.
xsldbg_LDFLAGS = $(all_libraries) 


xsldbgtest_SOURCES = \
	main.c	\
	simpleio.c

noinst_PROGRAMS = xsldbgtest

# use of configure defined libraries 
xsldbgtest_LDADD = $(top_builddir)/src/libxsldbg/libxsldbg.la $(XSLDBG_TEST_LIBS)

# the library search path.
xsldbgtest_LDFLAGS = $(XSLDBG_STATIC_FLAGS) $(all_libraries)


clean:
	rm -f *.o

very-clean:clean
	rm -f *.*~
	rm -f *~
	rm -f Makefile.in
	rm -f Makefile

