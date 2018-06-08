#this file is only used to build kxsldbg

set( libxsldbg_src
    arraylist.cpp
    breakpoint.cpp
    breakpoint_cmds.cpp
    callstack.cpp
    debug.cpp
    debugXSL.cpp
    file_cmds.cpp
    files.cpp
    nodeview_cmds.cpp
    option_cmds.cpp
    options.cpp
    options_common.cpp    
    os_cmds.cpp
    param_cmds.cpp
    search.cpp
    search_cmds.cpp
    template_cmds.cpp
    trace_cmds.cpp
    utils.cpp
    variable_cmds.cpp
    xsldbg.cpp
    xsldbgthread.cpp
    xsldbgsettingsmodel.cpp
    xsldbgmsg.cpp
    ../simpleio.cpp
)

# unix sources
set (libxsldbg_unix_src
      files_unix.cpp
      help_unix.cpp
      options_unix.cpp
) 

# win32 sources
set (libxsldbg_win_src
    files_win32.cpp
    help_win32.cpp
    options_win32.cpp 
)

add_library(xsldbg STATIC ${libxsldbg_src} ${libxsldbg_unix_src})