// Deque.h
//
// Define the abstract data type for a Deque of string pointers.  The strings
// themselves are NOT stored.

// Define true and false

#include <stdbool.h>

// A variable of type Deque is a pointer to the struct used to implement the
// deque operations declared below.  Note that the declaration of that struct
// appears only in the file that implements these operations.  Thus the calling
// program does not know what fields have been defined and cannot access them;
// and the variable must be a pointer since the size of the struct is unknown.

typedef struct deque *Deque;

// Set *D to a new object of type Deque.
// Returns true if successful, false otherwise.
int createD (Deque *d);

// Adds new node to the tail of Deque
int addD (Deque *d, Tree new_node);

// Removes the node at the head of Deque
void remD (Deque *d, Tree* new_node);

int isEmptyD (Deque *d);

int destroyD (Deque* d);

////////////////////////////////////////////////////////////////////////////////////////////
