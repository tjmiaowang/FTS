cmake_minimum_required(VERSION 2.8)
project(editline)

set(editline_SRCS
    editline/readline.h
    chared.c
    chared.h
    chartype.c
    chartype.h
    common.c
    common.h
    el.c
    el.h
    eln.c
    emacs.c
    emacs.h
    fcns.c
    fcns.h
    fgetln.c
    filecomplete.c
    filecomplete.h
    help.c
    help.h
    hist.c
    histedit.h
    hist.h
    history.c
    historyn.c
    keymacro.c
    keymacro.h
    map.c
    map.h
    parse.c
    parse.h
    prompt.c
    prompt.h
    read.c
    read.h
    readline.c
    refresh.c
    refresh.h
    search.c
    search.h
    sig.c
    sig.h
    strlcat.c
    strlcpy.c
    sys.h
    terminal.c
    terminal.h
    tokenizer.c
    tokenizern.c
    tty.c
    tty.h
    unvis.c
    vi.c
    vi.h
    vis.c
    vis.h
    wcsdup.c
)

# FindCurses fails when ncurses lives in /usr/lib32 as is the case on
# old multiarch Ubuntu and Debian boxes (where the 32bit libs live in
# /lib32 and /usr/lib32). In newer Ubuntu and Debian versions the
# libraries live in [/usr]/lib/i386-linux-gnu which is something that
# cmake can better cope with.
#find_package(Curses REQUIRED)
#find_library(CURSES_CURSES_LIBRARY ncurses)
set(CURSES_CURSES_LIBRARY ncurses)

include_directories(
    .
    ${CURSES_INCLUDE_DIR}
)

add_library(editline_lib STATIC ${editline_SRCS})
target_link_libraries(editline_lib ${CURSES_CURSES_LIBRARY})

# disable warnings that are enabled by default on some versions of gcc
set_target_properties(editline_lib PROPERTIES COMPILE_FLAGS "-Wno-unused-result")

