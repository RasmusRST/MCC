#include "globals.h"
#include "code.h"

extern FILE *outCode;

extern char *cmdBuffer;
extern int codeBuffer;

/* Emmits a comment. Same structure as printf */
void emitComment(const char* format, ...)
{
	va_list args;
	va_start(args, format);

	//if (Verbose)
	char* str1;
	char* str2;
	str1 = "* %s";
	str2 = "\n";
	char * str3 = (char *)malloc(1 + strlen(str1) + strlen(str2) + strlen(format) + 100); // TODO: find length of args, not guess max 20
	strcpy(str3, str1);
	strcat(str3, format);
	strcat(str3, str2);
	vprintf(str3, args);
	free(str3);

	va_end(args);
}

/* Emmits a code to the output or buffer. Same structure as printf.
   if codeBuffer = 1 the output is written to cmdBuffer (a char pointer).
*/
void emitCode(const char* format, ...)
{
	va_list args;
	va_start(args, format);

	if (!codeBuffer)
	vfprintf(outCode, format, args);  /* print to code file */
	else
	{
		char *temp;
		temp = (char *)malloc(strlen(format) + 100);
		vsprintf(temp, format, args); /* print to intermediate buffer */
		strcat(cmdBuffer, temp);
		free(temp);
	}
	va_end(args);
}