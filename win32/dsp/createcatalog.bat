Rem Create xml catalog for xsldbg
@echo create xml catalog for xsldbg
md c:\etc
md c:\etc\xml
set XML_CATALOG_FILES="file:///c:\etc\xml\catalog"   
xmlcatalog -v --noout --create "c:\etc\xml\catalog"
xmlcatalog -v --noout --add public "-//xsldbg//DTD search XML V1.0//EN" "file:///c:/Program Files\xsldbg\docs\en\search.dtd"  "c:\etc\xml\catalog"
xmlcatalog -v --noout --add public "-//xsldbg//DTD xsldoc XML V1.0//EN" "file:///c:/Program Files\xsldbg\docs\en\xsldoc.dtd"  "c:\etc\xml\catalog"
