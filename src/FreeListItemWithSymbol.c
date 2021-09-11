#include "FreeListItemWithSymbol.h"
#include "ExpressionParser.h"

void  freeListItemWithSymbol(ListItem *item){
  if(getItemSymbol(item))
    freeSymbol(getItemSymbol(item));
  if(item)
    linkedListFreeListItem(item);
}

void  freeSymbolInLinkedList(void *symbolToFree){
  Symbol  *symbol = (Symbol *)symbolToFree;
  freeSymbol(symbol);
}