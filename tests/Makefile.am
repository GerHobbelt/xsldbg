# set the include path found by configure
# cheet a bit because I need HELP_DOC_PATH defined
INCLUDES= $(all_includes)  -DVERSION=\"@VERSION@\" \
	$(LIBXML_CFLAGS) $(LIBXSLT_CFLAGS)

XSLDBG=$(top_builddir)/src/xsldbg

EXTRA_DIST= *.xsl *.xml *.data *.pl

TESTS = \
	../results/test1.pl \
	../results/test2.pl \
	../results/test3.pl \
	../results/test4.pl \
	../results/test5.pl \
	../results/test6.pl \
	../results/test7.pl \
	../results/test8.pl 
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
bin_PROGRAMS = listTest

# Define some tests on arraylist 
listTest_SOURCES = listTest.c 

# use of configure defined libraries
listTest_LDADD = $(XSLDBG_LIBS) ../src/options.o

# the library search path.
listTest_LDFLAGS = $(all_libraries) 

$(XSLDBG):
	@(cd $(top_builddir)/src ; $(MAKE) xsldbg)

../results/%.script ../results/%.pl : %.data testgenerator.xsl 
	$(XSLDBG) --param baseFileName \'../results/$(*F)\' testgenerator.xsl $(*F).data 

runtests : $(TESTS) $(TEST_RESULTS) $(XSLDBG) 

../results/%.results : $(TESTS) %.xsl %.xml xsldbgmatch.pl $(XSLDBG) 	
	@echo "Running test $(*F)"
	@echo "Running test $(*F)" > $@
	$(XSLDBG) --shell  $(*F).xsl  $(*F).xml <  ../results/$(*F).script &> ../results/$(*F).log
	perl ../results/$(*F).pl  &>  $@ < ../results/$(*F).log
	@echo "Test $(*F) succeeded" 
	@echo "Test $(*F) succeeded">> $@

if WITH_PERL

all: $(XSLDBG) listTest runtests
	./listTest

clean:
	rm -f ../results/*.script
	rm -f ../results/*.pl
	rm -f ../results/*.log
else

all:

endif