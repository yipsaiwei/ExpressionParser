#include "ExcpetionThrowing.h"

extern  SymbolTableStruct  symbolTable[];

/*
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
*/

void  symbolThrowInfixException(Symbol  *symbol, int  errorCode, Symbolizer *symbolizer){
  char  *currentStr = returnSymbolCharGivenId(symbol->id);
  if(symbolizer->lastSymbolId == _NULL){
    freeSymbolizer(symbolizer);
    throwException(errorCode, symbol, 0, "Invalid Infix %s after NULL! Only numbers or suffix are allowed.", currentStr);
  }else{
    char  *previousStr = returnSymbolCharGivenId(symbolizer->lastSymbolId);
    freeSymbolizer(symbolizer);
    throwException(errorCode, symbol, 0, "Invalid Infix '%s' before '%s'. Only numbers or suffix are allowed.", currentStr, previousStr);
  }
}

void  dumpSymbolErrorMessage(CEXCEPTION_T ex, int lineNo){
  Symbol *symbol = ex->data;
  if(symbol == NULL || isSymbolNull(symbol)){
    printf("Error on line%d:%d: %s\n%s\n", lineNo, symbol->token->startColumn, ex->msg, symbol->token->originalstr);
  }else{
    SymbolTableStruct instruction = symbolTable[symbol->id];
    char  *idChar = instruction.idChar;
    int idCharSize = strlen(idChar);
    char  *errorLine;
    errorLine = errorIndicator(symbol->token->startColumn, idCharSize);
    printf("Error on line %d:%d: %s\n%s\n%s\n", lineNo, symbol->token->startColumn, ex->msg, symbol->token->originalstr, errorLine);
    free(errorLine);
  }
  freeSymbol(symbol);
}