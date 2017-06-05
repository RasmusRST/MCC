%{
#include <stdio.h>
#include "y.tab.h"
%}

%option nounistd

%%
("+"|"-")?[0-9]+("."[0-9]+)?((E|e|exp)("+"|"-")?[0-9]+)			return SCIENTIFICVAL;
[0-9]+                  yylval.ival=atoi(yytext); return NUMBER;
[0-9]+("."[0-9]+)       yylval.dval=atoi(yytext); return DECIMAL;
[a-zA-Z][a-zA-Z0-9]*    yylval.name=strdup(yytext); return NAME;
\n                      /* ignore EOL */
.               		{return yytext[0];} // match all single characters
%%
