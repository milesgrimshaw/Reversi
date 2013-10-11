// getLine.c                                     

// Read a line of text using the file pointer *fp and returns a pointer to a
// null-terminated string that contains the text read, include the newline (if
// any) that ends the line.  Storage for the line is allocated with malloc()
// and realloc().  If the end of the file is reached before any characters are
// read, then the NULL pointer is returned.

#include <stdlib.h>
#include "getLine.h"

char *getLine(FILE *fp)
{
    char *line;                 // Line being read
    int size;                   // #chars allocated
    int c, i;

    size = sizeof(double);                      // Minimum allocation
    line = malloc (size);
    for (i = 0;  (c = getc(fp)) != EOF; )  {
	if (i == size-1) {
	    size *= 2;                          // Double allocation
	    line = realloc (line, size);
	}
	line[i++] = c;
	if (c == '\n')                          // Break on newline
	    break;
    }

    if (c == EOF && i == 0)  {                  // Check for immediate EOF
	free (line);
	return NULL;
    }

    line[i++] = '\0';                           // Terminate line
    line = realloc (line, i);                   // Trim excess storage

    return (line);
}
