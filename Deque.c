/*	Miles Grimshaw
	A complete implementation of the Deque ADT
	Full specification of functions outlines in Deque.h
*/

#include <stdio.h>
#include <stdlib.h>
#include "Deque.h"

/*	This implementation of the Deque ADT is a non-circular doubly-lined list
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

   		/*	An empty deque will have the following specifications:
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


/* 	Implementation of addD()
	Adds to new node to the tail of Deque
*/
int addD (Deque *d, node* new_node)
{
	//	Check if deck doesn't exist
	if (!(*d))
		return false;
	// struct node* new_node;
	// new_node = malloc(sizeof(struct node));
	if (new_node) {
		// new_node->line = s;

		//	Ensure that deque's head and tail are correctly configured
		(*d)->tail->previous->next = new_node;
		new_node->previous = (*d)->tail->previous;
		(*d)->tail->previous = new_node;
		new_node->next = (*d)->tail;

		return true;
	}
	else
		return false;
}


/* 	Implementation of remD()
	Removes from the head of Deque

*/
void remD (Deque *d, node** new_node)
{
    if (isEmptyD(d))
    	return false;
    else {
    	struct node* to_remove = (*d)->head->next;
    	*new_node = to_remove;
    	// *s = to_delete->line;
    	(*d)->head->next = to_delete->next;
    	to_delete->next->previous = (*d)->head;
    	free(to_delete);
		return true;
	} 
}


// Implementation of isEmptyD()
int isEmptyD (Deque *d)
{
	if (*d) {
		if ((*d)->head->next == (*d)->tail)
			return true;
		else
			return false;
	}
		
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
