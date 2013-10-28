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
    // Line being read
    char *line;                 
    // #chars allocated
    int size;                   
    int c, i;

    // Minimum allocation  
    size = sizeof(double);                      
    line = malloc (size);
    for (i = 0;  (c = getc(fp)) != EOF; )  {
	if (i == size-1) {
	    // Double allocation
        size *= 2;                          
	    line = realloc (line, size);
	}
	line[i++] = c;
	// Break on newline
    if (c == '\n')                          
	    break;
    }

    // Check for immediate EOF
    if (c == EOF && i == 0)  {
	free (line);
	return NULL;
    }

    // Terminate line
    line[i++] = '\0';                           
    // Trim excess storage
    line = realloc (line, i);                   

    return (line);
}
