#this file is only used to build kxsldbg

set (libqtnotifier_src 
  qtnotifier2.cpp
  xsldbgdebuggerbase.cpp
  xsldbgevent.cpp
  xsldbgnotifier.cpp
  xsldbgthreadimpl.cpp
)

# unix sources
set (libqtnotifier_unix_src
  # xsldbgthread.cpp
)

# win32 sources
set (libqtnotifier_win32_src
 #  xsldbgthread_win32.cpp
)

include_directories( ${LIBXML2_INCLUDE_DIR} )
include_directories( ${LIBXSLT_INCLUDE_DIR} )

# build for Unix only at moment
add_library(qtnotifier STATIC ${libqtnotifier_src} ${libqtnotifier_unix_src})

