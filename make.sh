#!/bin/sh
# This script runs simple yacc/flex compilations
# instructions
set -x; # echo on
yacc -d yacc/mcc.y -o generated/y.tab.c
flex -o generated/lex.yy.c lex/mcc.l
gcc generated/lex.yy.c generated/y.tab.c src/util.c src/analyze.c src/symtab.c src/cgen.c src/code.c src/mcc.c -IInclude -Igenerated -o build/mcc_GNU
#done