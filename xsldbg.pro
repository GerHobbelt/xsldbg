TEMPLATE = subdirs
SUBDIRS = src docs tests

unix {
    SUBDIRS += \
        man1 

	xsldbg_shortcut {	
		SUBDIRS += \
			icons
	}
}

DISTFILES +=\
     ChangeLog \
     README.md \
     COPYING \
     xsldbg.spec \
     xsldbg_suse.custom_spec \
     xsldbg_centos.custom_spec \
     xsldbg_PLD_Linux.custom_spec 

