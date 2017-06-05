#!/bin/sh
# This script runs simple yacc/flex compilations
# instructions
set -x; # echo on
yacc -d yacc/mcc.y -o generated/y.tab.c
flex -o generated/lex.yy.c lex/mcc.l
#gcc lex.yy.c y.tab.c ../src/mcc.c -Ibuild -o mcc
#done