Rem Create xml catalog for xsldbg
@echo create xml catalog for xsldbg

set XML_CATALOG_FILES="/etc/xml/catalog.xml"
set XSLDBG_CATALOG_FILES_URI="file:///xsldbg/docs/en/catalog.xml"

if EXIST c:\etc\xml\catalog.xml GOTO ADDCATALOG
md c:\etc
md c:\etc\xml  

pause Do you wish to create new catalog %XML_CATALOG_FILES% 
xmlcatalog -v --noout --create %XML_CATALOG_FILES%

:ADDCATALOG

xmlcatalog -v --noout --add nextCatalog %XSLDBG_CATALOG_FILES_URI% "" %XML_CATALOG_FILES%
set XML_CATALOG_FILES=
XSLDBG_CATALOG_FILES_URI=

:EOF

