if USE_XSLDBG_AS_THREAD
# set the include path found by configure
# cheet a bit because I need DOCS_PATH defined
INCLUDES= $(all_includes) -DDOCS_PATH=\"$(HTML_DIR)/xsldbg/\" -DVERSION=\"@VERSION@\" \
	$(LIBXML_CFLAGS) $(LIBXSLT_CFLAGS) -I $(top_builddir)/src/libxsldbg -I $(QIDIR)/include
else
# set the include path found by configure
# cheet a bit because I need DOCS_PATH defined
INCLUDES= $(all_includes) -DDOCS_PATH=\"$(HTML_DIR)/xsldbg/\" -DVERSION=\"@VERSION@\" \
	$(LIBXML_CFLAGS) $(LIBXSLT_CFLAGS) -I $(top_builddir)/src/libxsldbg
endif

XSLDBG=$(top_builddir)/src/xsldbg

EXTRA_DIST= *.xsl *.xml *.data *.pl testcmd.dtd testcoverage.txt

TESTS = \
	../results/test1.pl \
	../results/test2.pl \
	../results/test3.pl \
	../results/test4.pl \
	../results/test5.pl \
	../results/test6.pl \
	../results/test7.pl \
	../results/test8.pl 

TEST_SCRIPT = \
	../results/test1.script \
	../results/test2.script \
	../results/test3.script \
	../results/test4.script \
	../results/test5.script \
	../results/test6.script \
	../results/test7.script \
	../results/test8.script
 
TEST_RESULTS = \
	../results/test1.results	\
	../results/test2.results  	\
	../results/test3.results	\
	../results/test4.results	\
	../results/test5.results	\
	../results/test6.results	\
	../results/test7.results	\
	../results/test8.results

# Define the array list tester
noinst_PROGRAMS = listTest searchTest

# Define some tests on arraylist 
listTest_SOURCES = listTest.c 

# use of configure defined libraries
listTest_LDADD = $(XSLDBG_LIBS) -lxsldbg ../src/simpleio.o

if USE_XSLDBG_AS_THREAD
# the library search path.
listTest_LDFLAGS = $(all_libraries) -L$(top_builddir)/src/libxsldbg/.libs -L$(QTDIR)/lib
else
listTest_LDFLAGS = $(all_libraries) -L$(top_builddir)/src/libxsldbg/.libs
endif

# Define some tests on the search dataDase
searchTest_SOURCES = searchTest.c

searchTest_LDADD = $(listTest_LDADD)

# the library search path.
searchTest_LDFLAGS = $(listTest_LDFLAGS)

$(XSLDBG):
	@(cd $(top_builddir)/src ; $(MAKE) xsldbg)

runtests :  $(XSLDBG) coretests $(TESTS) $(TEST_RESULTS)

# do the tests of core functionality on xsldbg
coretests: listTest searchTest
	$(XSLDBG_EXPORTS) ./listTest
	$(XSLDBG_EXPORTS) ./searchTest

../results/%.script ../results/%.pl : %.data testgenerator.xsl
	$(XSLDBG_EXPORTS) $(XSLDBG) --param baseFileName \'../results/$(*F)\' testgenerator.xsl $(*F).data 

../results/%.results : $(TESTS) %.xsl %.xml xsldbgmatch.pl $(XSLDBG)
	@echo "Running test $(*F)"
	@echo "Running test $(*F)" > $@
	$(XSLDBG_EXPORTS) $(XSLDBG) --shell  $(*F).xsl  $(*F).xml <  ../results/$(*F).script &> ../results/$(*F).log
	perl ../results/$(*F).pl  &>  $@ < ../results/$(*F).log
	@echo "Test $(*F) succeeded"
	@echo "Test $(*F) succeeded">> $@

if WITH_PERL


all: $(XSLDBG) coretests runtests


clean:
	rm -f ../results/*.script
	rm -f ../results/*.pl
	rm -f ../results/*.log

very-clean: clean
	rm -f *.*~
	rm -f *~
else

all:

endif