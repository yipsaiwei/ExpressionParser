#include "ExcpetionThrowing.h"

void  symbolThrowInfixException(Symbol  *symbol, int  errorCode, Symbolizer *symbolizer){
  SymbolExceptionInfo *symbolInfo = malloc(sizeof(SymbolExceptionInfo));
  symbolInfo->symbol = symbol;
  symbolInfo->symbolizer = symbolizer;
  printf("In symbolThrowInfixException function %s  %s\n", getCurrentString(symbol), getPreviousString(symbolizer));
  if(getPreviousString(symbolizer) == NULL)
    throwException(errorCode, symbolInfo, 0, "Invalid Infix '%s' detected after NULL(%s)! Only numbers or suffix are allowed.", getCurrentString(symbol), getPreviousString(symbolizer));
  else
    throwException(errorCode, symbolInfo, 0, "Invalid Infix '%s' after '%s'. Only numbers or suffix are allowed.", getCurrentString(symbol), getPreviousString(symbolizer));
}

void  symbolThrowPrefixException(Symbol  *symbol, int  errorCode, Symbolizer *symbolizer){
  SymbolExceptionInfo *symbolInfo = malloc(sizeof(SymbolExceptionInfo));
  symbolInfo->symbol = symbol;
  symbolInfo->symbolizer = symbolizer;
  throwException(errorCode, symbolInfo, 0, "Invalid prefix '%s' after '%s'. Prefix cannot appear after suffix.", getCurrentString(symbol), getPreviousString(symbolizer));
}

void  symbolThrowSuffixException(Symbol  *symbol, int  errorCode, Symbolizer *symbolizer){
  SymbolExceptionInfo *symbolInfo = malloc(sizeof(SymbolExceptionInfo));
  symbolInfo->symbol = symbol;
  symbolInfo->symbolizer = symbolizer;
  throwException(errorCode, symbolInfo, 0, "Invalid suffix '%s' after '%s'. Suffix can only appear after a number.", getCurrentString(symbol), getPreviousString(symbolizer));
}

void  symbolThrowNumberException(Symbol  *symbol, int  errorCode, Symbolizer *symbolizer){
  SymbolExceptionInfo *symbolInfo = malloc(sizeof(SymbolExceptionInfo));
  symbolInfo->symbol = symbol;
  symbolInfo->symbolizer = symbolizer;
  throwException(errorCode, symbolInfo, 0, "Invalid number '%s' after '%s'. Number cannot appear after a suffix.", getCurrentString(symbol), getPreviousString(symbolizer));
}

void  symbolThrowMissingParenException(Symbol  *symbol, int  errorCode, Symbolizer *symbolizer){
  SymbolExceptionInfo *symbolInfo = malloc(sizeof(SymbolExceptionInfo));
  symbolInfo->symbol = symbol;
  symbolInfo->symbolizer = symbolizer;  
  if(errorCode == ERROR_MISSING_OPEN_PAREN)
    throwException(errorCode, symbolInfo, 0, "Incomplete parenthesis!(Open parenthesis is missing).");
  else
    throwException(errorCode, symbolInfo, 0, "Incomplete parenthesis!(Closing parenthesis is missing).");
}

void  dumpSymbolErrorMessage(CEXCEPTION_T ex, int lineNo){
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
  Symbolizer  *symbolizer = ((SymbolExceptionInfo *)ex->data)->symbolizer;
  freeSymbolizer(symbolizer);
  if(ex->data){
    SymbolExceptionInfo *infoptr = (SymbolExceptionInfo *)ex->data;
    free(infoptr);
  }
}