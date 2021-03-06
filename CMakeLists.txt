cmake_minimum_required (VERSION 3.0)
project (mcc)


INCLUDE_DIRECTORIES("include/")

add_library(LexYacc "generated/y.tab.c"
                    "generated/lex.yy.c")
INCLUDE_DIRECTORIES("generated/")

SET(SOURCES
  "src/util.c"
  "src/analyze.c"
  "src/symtab.c"
  "src/cgen.c"
  "src/code.c"
)

SET(HEADERS
  "include/globals.h"
  "include/util.h"
  "include/analyze.h"
  "include/symtab.h"
  "include/cgen.h"
  "include/code.h"  
)

SET(PARSER
  "lex/mcc.l"
  "yacc/mcc.y"
)

source_group("Utilities" FILES ${SOURCES})

source_group("Parser" FILES ${PARSER})

add_executable(mcc "src/mcc.c" ${SOURCES} ${HEADERS} ${PARSER})

target_link_libraries(mcc LexYacc)
