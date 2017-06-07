#include "globals.h"
#include "code.h"

extern FILE *outCode;

extern char cmdBuffer[200];
extern int codeBuffer;

void emitComment(char * c)
{
	//if (Verbose)
	printf("* %s\n", c);
}

void emitCode(char * c)
{
	if (!codeBuffer)
	fprintf(outCode, "%s", c); /* print to code file */
	else
	sprintf(cmdBuffer, "%s", c); /* print to intermediate buffer */
}