

#include<stdio.h>
#include "y.tab.h"

typedef struct yy_buffer_state * YY_BUFFER_STATE;
extern int yyparse();
extern YY_BUFFER_STATE yy_scan_string(char * str);
extern void yy_delete_buffer(YY_BUFFER_STATE buffer);

int main(){
	printf("MATLAB to C compiler (MCC) 0.1\n");
	printf("Copyright (C) 2017 Rasmus Steffensen\n");
	printf("This is free software; There is NO warranty;\n");
	printf("not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n\n");
    char string[] = "sdf=45;";
    YY_BUFFER_STATE buffer = yy_scan_string(string);
    yyparse();
    yy_delete_buffer(buffer);
    return 0;
}