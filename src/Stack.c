#include "Stack.h"
#include "Exception.h"
#include "CException.h"

StackStruct *createStack(){
  StackStruct *stack = memAlloc(sizeof(StackStruct));
  if(!stack)
    return  NULL;
  stack->size = 0;
  stack->list = NULL;
  return  stack;
}

void  freeStack(StackStruct *stack, void (*freeFunction)(void *)){
  if(stack->list)
    linkedListFreeList(stack->list, freeFunction);
  if(stack)
    memFree(stack);
}

void  pushToStack(StackStruct *stack, void  *data){
  if(stack->list == NULL)
    stack->list = linkedListCreateList();
  ListItem  *item = linkedListCreateListItem(data);
  linkedListAddItemToHead(item, stack->list);
  stack->size++;
}

ListItem  *popFromStack(StackStruct  *stack){
  if(stack->size == 0)
    throwException(ERROR_STACK_IS_EMPTY, NULL, 0, "ERROR code %x : The stack is empty!", ERROR_STACK_IS_EMPTY);
  ListItem  *itemPop = linkedListRemoveItemFromHead(stack->list);
  stack->size--;
  return  itemPop;
}

ListItem  *peekTopOfStack(StackStruct *stack){
  if(stack->list == NULL)
    return  NULL;
  else
    return  (stack->list->head);
}

int isStackEmpty(StackStruct  *stack){
  return  stack->size == 0;
}
