/* This file is a part of the MATLAB to C++ compiler
   Developped by Rasmus Steffensen, May 2017.
*/

%{
#include "globals.h"
#include "util.h"
#include "y.tab.h"

extern int isatty(int);

%}

%option nounistd

%%
("if"|"IF"|"If")		return IF;
("end"|"end"|"end")		return END;

"atan"					return ATAN;
"atan2"					return ATAN2;
"cos"					return COS;
"sin"					return SIN;
"sqrt"					return SQRT;
"sign"					return SIGN;

("+"|"-")?[0-9]+("."[0-9]+)?((E|e|exp)("+"|"-")?[0-9]+)	  return SCIENTIFICVAL;
[0-9]+                  yylval.ival=atoi(yytext); return NUMBER;
[0-9]+("."[0-9]+)       yylval.dval=atoi(yytext); return DECIMAL;
[a-zA-Z"_"][a-zA-Z0-9"_"]*    yylval.name=strdup(yytext); return NAME;

"\n"                    ; /* ignore EOL */
"\r\n"                  ; /* windows line endings */
" "						; /* ignore white spaces */
"..."					; /* ignore MATLAB next lines */
.               		{return yytext[0];} // match all single characters
%%
