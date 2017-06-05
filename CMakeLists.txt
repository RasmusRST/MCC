cmake_minimum_required (VERSION 3.0)
project (mcc)


INCLUDE_DIRECTORIES("include/")

add_library(LexYacc "generated/y.tab.c"
                    "generated/lex.yy.c")
INCLUDE_DIRECTORIES("generated/")

add_executable(mcc src/mcc.c)
target_link_libraries(mcc LexYacc)
