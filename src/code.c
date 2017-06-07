#include "globals.h"
#include "code.h"

extern FILE *outCode;

extern char *cmdBuffer;
extern int codeBuffer;

void emitComment(char * c)
{
	//if (Verbose)
	printf("* %s\n", c);
}

void emitCode(const char* format, ...)
{
	va_list args;
	va_start(args, format);

	if (!codeBuffer)
	vfprintf(outCode, format, args);  /* print to code file */
	else
	vsprintf(cmdBuffer, format, args); /* print to intermediate buffer */

	va_end(args);
}