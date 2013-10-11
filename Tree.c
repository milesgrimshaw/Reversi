// bst.c                                            Stan Eisenstat (04/17/13)
//
// Implementation of binary search trees

#define _GNU_SOURCE                     // Define strdup()
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
//#include <malloc.h>
#include "Tree.h"

// Tree data structure
struct node {
    // Board stored at the node
    char* B;        
    // Depth of the node
    int H;
    // Score of the board at the node
    int S;
    // Sibling of the node
    Tree Sib;
    // First child of the node
    Tree Child;
    // Previous and Next for us in the queue
    struct node* previous;
    struct node* next;
};

// Return an empty tree
Tree createT (void)
{
    return NULL;
}


// Return a one-element tree that contains the board b
Tree makeNode (char* b, int h)
{
    Tree t = malloc (sizeof(*t));
    t->B = strdup(b);
    t->H = h;
    t->S = NULL;
    t->Sib = NULL;
    t->Child = NULL;
    t->previous = NULL;
    t->next = NULL;
    return t;
}

char* getBoard (Tree t) {
    return t->B;
}

// Return height of T
int getDepth (Tree t)
{
    if (t==NULL)
        return -1;
    else
        return t->H;
}

// Return depth of T
int getScore (Tree t)
{
    if (t==NULL)
        return -1;
    else
        return t->S;
}

Tree getChild (Tree t) {
    if (t->Child == NULL)
        return NULL;
    else
        return t->Child;
}

Tree getNext(Tree t) {
    if (t->Sib == NULL)
        return NULL;
    else
        return t->Sib;
}

void setChild (Tree p, Tree c) {
    p->Child = c;
}

void setSib (Tree p, Tree c) {
    p->Sib = c;
}

void setScore (Tree t, int s) {
    t->S = s;
}



////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////

/*  This implementation of the Deque ADT is a non-circular doubly-lined list
*/

// Node struct is defined in the Tree structure


struct deque {
   struct node* head;
   struct node* tail;
};

// Implementation of createD()
int createD (Deque *d)
{   
    struct deque new_deque;
    *d = malloc(sizeof(struct deque));
    
   if (*d) {
        new_deque.head = malloc(sizeof(struct node));
        new_deque.tail = malloc(sizeof(struct node));

        /*  An empty deque will have the following specifications:
            Head and Tail will inwardly point to each other
            The end points will point to NULL
        */
        if (new_deque.head && new_deque.tail) {
            new_deque.head->previous = NULL;
            new_deque.head->next = new_deque.tail;
            new_deque.tail->next = NULL;
            new_deque.tail->previous = new_deque.head;
        }
        else
            return false;
        **d = new_deque;
        return true;
   }
   else
        return false;
}


/*  Implementation of addD()
    Adds to new node to the tail of Deque
*/
int addD (Deque *d, Tree new_node)
{
    //  Check if deck doesn't exist
    if (!(*d))
        return false;
    // struct node* new_node;
    // new_node = malloc(sizeof(struct node));
    if (new_node) {
        // new_node->line = s;

        //  Ensure that deque's head and tail are correctly configured
        (*d)->tail->previous->next = new_node;
        new_node->previous = (*d)->tail->previous;
        (*d)->tail->previous = new_node;
        new_node->next = (*d)->tail;

        return true;
    }
    else
        return false;
}


/*  Implementation of remD()
    Removes from the head of Deque

*/
void remD (Deque *d, Tree* new_node)
{
    struct node* to_remove = (*d)->head->next;
    *new_node = to_remove;
    (*d)->head->next = to_remove->next;
    to_remove->next->previous = (*d)->head;
}


// Implementation of isEmptyD()
int isEmptyD (Deque *d)
{
    if ((*d)->head->next == (*d)->tail)
        return true;
    else
        return false;
}


/////////////////////////////////////////////////////////////////

// Implementation of destroyD()
int destroyD (Deque* d)
{
    if (*d) {
        while (!isEmptyD(d)) {
            struct node* to_delete = (*d)->head->next;
            (*d)->head->next = (*d)->head->next->next;
            free(to_delete);
        }
        free((*d)->head);
        free((*d)->tail);
        free(*d);
        *d = NULL;
        return true;
    }
    else
        return false;
}
