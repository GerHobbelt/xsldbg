# in subdir we have files need to test the guess of stylesheet and XML file name
SUBDIRS = subdir

if USE_XSLDBG_AS_THREAD
# set the include path found by configure
# cheet a bit because I need DOCS_PATH defined
INCLUDES= $(all_includes) -DDOCS_PATH=\"$(HTML_DIR)/xsldbg/\" -DVERSION=\"@VERSION@\" \
	$(LIBXML_CFLAGS) $(LIBXSLT_CFLAGS)  -I $(top_builddir)/src -I $(top_builddir)/src/libxsldbg -I $(QIDIR)/include
else
# set the include path found by configure
# cheet a bit because I need DOCS_PATH defined
INCLUDES= $(all_includes) -DDOCS_PATH=\"$(HTML_DIR)/xsldbg/\" -DVERSION=\"@VERSION@\" \
	$(LIBXML_CFLAGS) $(LIBXSLT_CFLAGS) -I $(top_builddir)/src/libxsldbg  -I $(top_builddir)/src
endif

XSLDBG=$(top_builddir)/src/xsldbgtest

EXTRA_DIST= *.xsl *.xml *.data *.pl *.dtd testcoverage.txt

if WITH_TEST10
   TEST10_PERL=../results/test10.pl 
   TEST10_SCRIPT=../results/test10.script
else
   TEST10_PER =
   TEST10_SCRIPT=
endif

TESTS = \
	../results/test1.pl \
	../results/test2.pl \
	../results/test3.pl \
	../results/test4.pl \
	../results/test5.pl \
	../results/test6.pl \
	../results/test7.pl \
	../results/test8.pl \
	../results/test9.pl \
	$(TEST10_PERL)	    \
	../results/test11.pl \
	../results/test12.pl \
	../results/test13.pl 

TEST_SCRIPT = \
	../results/test1.script \
	../results/test2.script \
	../results/test3.script \
	../results/test4.script \
	../results/test5.script \
	../results/test6.script \
	../results/test7.script \
	../results/test8.script	\
	../results/test9.script \
	$(TEST10_SCRIPT) 	\
	../results/test11.script \
	../results/test12.script \
	../results/test13.script
 

# Define the array list tester
noinst_PROGRAMS = listTest searchTest

# Define some tests on arraylist 
listTest_SOURCES = listTest.c 

listTest_DEPENDENCIES  = $(top_builddir)/src/libxsldbg/libxsldbg.la

$(top_builddir)/src/libxsldbg/libxsldbg.la:
	cd $(top_builddir)/src/libxsldbg; $(MAKE

# use of configure defined libraries
listTest_LDADD = ../src/simpleio.o $(top_builddir)/src/libxsldbg/libxsldbg.la $(XSLDBG_TEST_LIBS)

listTest_LDFLAGS = $(XSLDBG_STATIC_FLAGS) $(all_libraries)



# Define some tests on the search dataDase
searchTest_SOURCES = searchTest.c

searchTest_DEPENDENCIES = $(listTest_DEPENDENCIES)

searchTest_LDADD = $(listTest_LDADD)

# the library search path.
searchTest_LDFLAGS = $(listTest_LDFLAGS)

$(XSLDBG):
	@(cd $(top_builddir)/src ; $(MAKE) xsldbgtest)

runtests :  $(XSLDBG) coretests $(TESTS)
	perl runtests.pl

# do the tests of core functionality on xsldbg
coretests: listTest searchTest
	./listTest
	./searchTest

../results/%.script ../results/%.pl : %.data testgenerator.xsl
	$(XSLDBG) -o dump.txt --param baseFileName \'../results/$(*F)\' testgenerator.xsl $(*F).data 

# if the user has Perl then they will have sed

if WITH_PERL


all: $(XSLDBG) coretests runtests


clean:
	rm -f ../results/*.log
	rm -f $(TESTS) 
	rm -f $(TEST_SCRIPT)
	rm -f $(TEST_RESULTS)

very-clean: clean
	rm -f *.*~
	rm -f *~
else

all:

endif
