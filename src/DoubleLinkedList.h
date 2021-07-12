#ifndef DOUBLELINKEDLIST_H
#define DOUBLELINKEDLIST_H

#include  "ListItem.h"
#include  "MemAlloc.h"

typedef	struct	DoubleLinkedList	DoubleLinkedList;
struct	DoubleLinkedList{
	ListItem		*head;
	ListItem		*tail;
	int			count;
};


int	linkedListAddItemToHead(ListItem  *itemToAdd , DoubleLinkedList *list );
int linkedListAddItemToTail(ListItem  *itemToAdd , DoubleLinkedList *list );	
ListItem *linkedListRemoveItemFromHead(DoubleLinkedList *list );
ListItem *linkedListRemoveItemFromTail(DoubleLinkedList *list );
ListItem  *linkedListCreateListItem(void  *data);
void  linkedListFreeListItem(ListItem *itemToFree);
void  linkedListFreeList(DoubleLinkedList *list, void (*freeFunction)(void *) );
DoubleLinkedList  *linkedListCreateList(void);
#endif // DOUBLELINKEDLIST_H
