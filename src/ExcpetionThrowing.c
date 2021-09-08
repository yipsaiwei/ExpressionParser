#include "ExcpetionThrowing.h"

extern  SymbolTableStruct  symbolTable[];

void  symbolThrowException(Symbol *symbol, int errorCode, char *message,...){
	size_t len;
	char *buffer;
  
	va_list va;
	va_start(va, message);
	len=vsnprintf(NULL, 0, message, va);
	buffer=memAlloc(len+1);
	vsprintf(buffer, message, va);
	va_end(va);
  
  throwException(errorCode, symbol, 0, "%s\n", buffer);
  
  free(buffer);
}

void  dumpSymbolErrorMessage(CEXCEPTION_T ex, int lineNo){
  Symbol *symbol = ex->data;
  SymbolTableStruct instruction = symbolTable[symbol->id];
  char  *idChar = instruction.idChar;
  int idCharSize = sizeof(idChar)/sizeof(char);
  char  *errorLine;
  if(ex->data == NULL)
    printf("Error on line%d:%d: %s\n", lineNo, 0, ex->msg);
  else{
    errorLine = errorIndicator(symbol->token->startColumn, idCharSize);
    printf("Error on line %d:%d: %s\n%s\n%s\n", lineNo, symbol->token->startColumn, ex->msg, symbol->token->originalstr, errorLine);
  }
}