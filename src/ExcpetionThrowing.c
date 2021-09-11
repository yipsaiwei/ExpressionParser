#include "ExcpetionThrowing.h"

ExceptionTable  exceptionTable[] = {
  //[ERROR_INVALID_INFIX] = symbolThrowInfixException,
  [ERROR_INVALID_PREFIX] = symbolThrowPrefixException,
  [ERROR_INVALID_SUFFIX] = symbolThrowSuffixException,
  [ERROR_INVALID_NUMBER] = symbolThrowNumberException,
};

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
  SymbolExceptionInfo *symbolInfo = malloc(sizeof(SymbolExceptionInfo));
  symbolInfo->symbol = symbol;
  symbolInfo->symbolizer = symbolizer;
  char  *currentStr = getCurrentString(symbol);
  char  *previousStr = getPreviousString(symbolizer);
  printf("In symbolThrowInfixException function %s  %s\n", currentStr, previousStr);
  if(getPreviousString(symbolizer) == NULL)
    throwException(errorCode, symbolInfo, 0, "Invalid Infix '%s' detected after NULL(%s)! Only numbers or suffix are allowed.", currentStr, previousStr);
  else
    throwException(errorCode, symbolInfo, 0, "Invalid Infix '%s' after '%s'. Only numbers or suffix are allowed.", currentStr, previousStr);
}

void  symbolThrowPrefixException(Symbol  *symbol, int  errorCode, char *previousStr, char  *currentStr){
  throwException(errorCode, symbol, 0, "Invalid type '%s' after '%s'. Only prefix or infix at this position are allowed.", currentStr, previousStr);
}

void  symbolThrowSuffixException(Symbol  *symbol, int  errorCode, char *previousStr, char  *currentStr){
  throwException(errorCode, symbol, 0, "Invalid type '%s' after '%s'. Only infix at this position are allowed.", currentStr, previousStr);
}

void  symbolThrowNumberException(Symbol  *symbol, int  errorCode, char *previousStr, char  *currentStr){
  throwException(errorCode, symbol, 0, "Invalid number '%s' after '%s'. Only infix at this position are allowed.", currentStr, previousStr);
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

void  dumpSymbolErrorMessageV2(CEXCEPTION_T ex, int lineNo){
  Symbol *symbol = ((SymbolExceptionInfo *)ex->data)->symbol;
  if(symbol == NULL || isSymbolNull(symbol)){
    printf("Error on line%d:%d: %s\n%s\n", lineNo, symbol->token->startColumn, ex->msg, symbol->token->originalstr);
  }else{
    char  *idChar = symbol->token->str;
    int idCharSize = strlen(idChar);
    char  *errorLine;
    errorLine = errorIndicator(symbol->token->startColumn, idCharSize);
    printf("Error on line %d:%d: %s\n%s\n%s\n", lineNo, symbol->token->startColumn, ex->msg, symbol->token->originalstr, errorLine);
    free(errorLine);
  }
  freeSymbol(symbol);
  freeSymbolizer(((SymbolExceptionInfo *)ex->data)->symbolizer);
  if(ex->data)
    free(((SymbolExceptionInfo *)ex->data));
}