XSLDBG=$(top_builddir)/src/xsldbg

TESTS = ../results/test1.pl ../results/test2.pl
TEST_RESULTS =../results/test1.results  ../results/test2.results

EXTRA_DIST= *.xsl *.xml *.data *.pl

$(XSLDBG):
	@(cd $(top_builddir)/src ; $(MAKE) xsldbg)

../results/%.script ../results/%.pl : %.data testgenerator.xsl 
	$(XSLDBG) --param baseFileName \'../results/$(*F)\' testgenerator.xsl $(*F).data 

runtests : $(TESTS) $(TEST_RESULTS) $(XSLDBG) 

../results/%.results : $(TESTS) %.xsl %.xml xsldbgmatch.pl $(XSLDBG) 	
	@echo "Running test $(*F)"
	@echo "Running test $(*F)" > $@
	$(XSLDBG) --shell  $(*F).xsl  $(*F).xml <  ../results/$(*F).script &> ../results/$(*F).log
	perl ../results/$(*F).pl  >>  $@ < ../results/$(*F).log
	@echo "Test $(*F) suceeded" 
	@echo "Test $(*F) suceeded" >> $@

if WITH_PERL

all: $(XSLDBG) runtests

clean:
	rm -f ../results/*.script
	rm -f ../results/*.pl
	rm -f ../results/*.log
else

all:

endif