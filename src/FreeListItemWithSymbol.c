#include "FreeListItemWithSymbol.h"
#include "ExpressionParser.h"

//To ensure that symbol inside list item will be freed as well
void  freeListItemWithSymbol(ListItem *item){
  if(getItemSymbol(item))
    freeSymbol(getItemSymbol(item));
  if(item)
    linkedListFreeListItem(item);
}

//Used in freeStack() function where a funcptr will point to here
void  freeSymbolInLinkedList(void *symbolToFree){
  Symbol  *symbol = (Symbol *)symbolToFree;
  freeSymbol(symbol);
}