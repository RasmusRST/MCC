cmake_minimum_required (VERSION 3.0)
project (mcc)


INCLUDE_DIRECTORIES("include/")

add_library(LexYacc "generated/y.tab.c"
                    "generated/lex.yy.c")
INCLUDE_DIRECTORIES("generated/")

SET(SOURCES
  "src/util.c"
)

SET(HEADERS
  "include/globals.h"
  "include/util.h"
)

source_group("Utilities" FILES ${SOURCES})

add_executable(mcc "src/mcc.c" ${SOURCES} ${HEADERS})

target_link_libraries(mcc LexYacc)
