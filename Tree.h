// Tree.h
//
// Interface for tree module for Reversi game

#include <stdio.h>

// Tree data structure
typedef struct node* Tree;

// Return an empty tree
Tree createT (void);

// Make a node with board b and depth h
Tree makeNode (char* b, int h);

// Get and return the board at the node* t
char* getBoard (Tree t);

// Get the depth of the node* t
int getDepth (Tree t);

// Get the score of the node* t
int getScore (Tree t);

// Get the child (node*) of the node* t
Tree getChild (Tree t);

// Get the next child (node*) of the node* t
Tree getNext(Tree t);

// Set the child at the node p
void setChild (Tree p, Tree c);

// Set the sibling at the node p
void setSib (Tree p, Tree c);

// Set the score at the node t
void setScore (Tree t, int s);

////////////////////////////////////////////////////////////////////////////////////////////

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
