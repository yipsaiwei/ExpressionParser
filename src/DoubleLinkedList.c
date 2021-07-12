#include "DoubleLinkedList.h"
#include <stdio.h>


/* This function is to add items to the head of double linked list.
    Parameter: 1. Pointer points to items
               2. Pointer points to double linked list
    return the number of items in the double linked list
*/
int	linkedListAddItemToHead(ListItem* Item, DoubleLinkedList* accList){
  int count;
  if(accList->head==NULL)
  {
    accList->head=Item;
    accList->tail=Item;
    accList->count=1;
    count=accList->count;
    return count;
	}
	else
	{
		Item->next=accList->head;
		accList->head->prev=Item;
		accList->head=Item;
		Item->prev=NULL;
		accList->count++;
		count=accList->count;
		return count;		
	}
}

/* This function is to add items to the tail of double linked list.
    Parameter: 1. Pointer points to items
               2. Pointer points to double linked list
    return the number of items in the double linked list
*/
int linkedListAddItemToTail(ListItem* Item , DoubleLinkedList* accList){
	int count;
	if(accList->head==NULL)
	{
		accList->head=Item;
		accList->tail=Item;
		accList->count=1;
		count=accList->count;
		return count;
	}
	else
	{
		Item->prev=accList->tail;
		accList->tail->next=Item;
		Item->next=NULL;
		accList->tail=Item;
		accList->count++;
		count=accList->count;
		return count;	
	}
}

/* This function is to remove items from the head of given double linked list.
    Parameter: Pointer points to double linked list
    return the number of items in the double linked list
*/
ListItem* linkedListRemoveItemFromHead(DoubleLinkedList* accList)
{
	ListItem	*returnedItem;
	if(accList->head==NULL)
		return NULL;
	else if (accList->head==accList->tail)
	{
		returnedItem=accList->head;
		accList->head=NULL;
		accList->tail=NULL;
		accList->count--;
		return returnedItem;
	}
	else
	{
		returnedItem=accList->head;
		accList->head->next->prev=NULL;
		accList->head=accList->head->next;
		returnedItem->next=NULL;
		returnedItem->prev=NULL;
		accList->count--;
		return	returnedItem;
	}
}

/* This function is to remove items from the tail of given double linked list.
    Parameter: Pointer points to double linked list
    return the number of items in the double linked list
*/
ListItem* linkedListRemoveItemFromTail(DoubleLinkedList* accList)
{
	ListItem	*returnedItem;
	if(accList->head==NULL)
		return NULL;
	else if (accList->head==accList->tail)
	{
		returnedItem=accList->tail;
		accList->head=NULL;
		accList->tail=NULL;
		accList->count--;
		return returnedItem;
	}
	else
	{
		returnedItem=accList->tail;
		accList->tail->prev->next=NULL;
		accList->tail=accList->tail->prev;
		returnedItem->next=NULL;
		returnedItem->prev=NULL;
		accList->count--;
		return	returnedItem;
	}
}

DoubleLinkedList  *linkedListCreateList(){
  DoubleLinkedList  *list = memAlloc(sizeof(DoubleLinkedList));
  list->head = NULL;
  list->tail = NULL;
  list->count = 0;
  return  list;
}

ListItem  *linkedListCreateListItem(void  *data){
  ListItem  *item = memAlloc(sizeof(DoubleLinkedList));   
  item->next = NULL;
  item->prev = NULL;
  item->data = data;
  return  item;
}

void  linkedListFreeListItem(ListItem *itemToFree){
  if(itemToFree)
    memFree(itemToFree);
}

void  linkedListFreeList(DoubleLinkedList *list, void (*freeFunction)(void *) ){
  ListItem  *listptr;
  while(list->head != NULL){
    listptr = list->head;
    list->head = list->head->next;
    freeFunction(listptr->data);
    linkedListFreeListItem(listptr);
    }
   if(list)
     memFree(list);
}