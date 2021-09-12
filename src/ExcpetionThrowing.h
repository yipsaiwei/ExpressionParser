#ifndef EXCPETIONTHROWING_H
#define EXCPETIONTHROWING_H

#include  <stdlib.h>
#include <stdarg.h>
#include "Exception.h"
#include "CException.h"
#include  "OperatorTable.h"
#include "ExpressionParser.h"
//#include "Token.h"
#include "Tokenizer.h"
#include  "MemAlloc.h"

#define ERROR_INVALID_INFIX               0x111
#define ERROR_INVALID_INFIX_AFTER_NONE    0x112
#define ERROR_INVALID_PREFIX              0x222
#define ERROR_INVALID_SUFFIX              0x333
#define ERROR_INVALID_NUMBER              0x444
#define ERROR_MISSING_OPEN_PAREN          0x555
#define ERROR_MISSING_CLOSING_PAREN       0x556
#define ERROR_EXCESS_OPERAND              0x666

#define getCurrentString(symbol)          symbol->token->str
#define getPreviousString(symbolizer)     symbolizer->lastSymbol->token->str
typedef void  (*ExceptionFuncPtr)(Symbol  *symbol, int  errorCode, Symbolizer *symbolizer); 

typedef struct  ExceptionTable  ExceptionTable;
struct  ExceptionTable{
  ExceptionFuncPtr  exceptionPtr;
};

typedef struct  SymbolExceptionInfo SymbolExceptionInfo;
struct  SymbolExceptionInfo{
  Symbol  *symbol;
  Symbolizer  *symbolizer;
};

//void  symbolThrowException(Symbol *symbol, int errorCode, char *message,...);
void  dumpSymbolErrorMessage(CEXCEPTION_T ex, int lineNo);
//void  dumpSymbolErrorMessageV2(CEXCEPTION_T ex, int lineNo);
void  symbolThrowInfixException(Symbol  *symbol, int  errorCode, Symbolizer *symbolizer);
void  symbolThrowPrefixException(Symbol  *symbol, int  errorCode, Symbolizer *symbolizer);
void  symbolThrowSuffixException(Symbol  *symbol, int  errorCode, Symbolizer *symbolizer);
void  symbolThrowNumberException(Symbol  *symbol, int  errorCode, Symbolizer *symbolizer);
void  symbolThrowMissingParenException(Symbol  *symbol, int  errorCode, Symbolizer *symbolizer);
#endif // EXCPETIONTHROWING_H
