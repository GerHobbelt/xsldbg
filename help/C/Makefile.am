EXTRADIST = xsldbg-C.omf.in

all: xsldbg.xml xsldbg-C.omf

xsldbg.xml : ../../docs/en/GNOME/gnome.docbook 
	cp ../../docs/en/GNOME/gnome.docbook  index.docbook

OMF_DIR = $(DISTDIR)/$(GNOME_DIR)/share/omf/xsldbg/ 
XML_DIR = $(DISTDIR)/$(GNOME_DIR)/share/gnome/help/xsldbg/C/
     
install-data-local: 
	if test -d $(GNOME_DIR); then if test ! -d $(OMF_DIR); then mkdir -p $(OMF_DIR); fi; cp xsldbg-C.omf $(OMF_DIR); if test ! -d $(XML_DIR); then mkdir -p $(XML_DIR); fi; cp index.docbook $(XML_DIR); fi;

clean: 
	rm ../../docs/en/index.docbook

