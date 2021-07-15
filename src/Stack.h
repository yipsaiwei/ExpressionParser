#ifndef STACK_H
#define STACK_H

#include "MemAlloc.h"
#include "ListItem.h"
#include "DoubleLinkedList.h"

#define ERROR_STACK_IS_EMPTY  0x01

typedef struct  StackStruct StackStruct;
struct  StackStruct{
  int size;
  DoubleLinkedList  *list;
};

StackStruct *createStack();
void  freeStack(StackStruct *stack, void (*freeFunction)(void *));
void  pushToStack(StackStruct *stack, void  *data);
ListItem  *popFromStack(StackStruct  *stack);
#endif // STACK_H
