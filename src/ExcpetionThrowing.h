#ifndef EXCPETIONTHROWING_H
#define EXCPETIONTHROWING_H

#include <stdarg.h>
#include "Exception.h"
#include "CException.h"
#include  "OperatorTable.h"
#include "ExpressionParser.h"
//#include "Token.h"
#include "Tokenizer.h"

#define ERROR_INVALID_INFIX               0x111
#define ERROR_INVALID_INFIX_AFTER_NONE    0x112
#define ERROR_INVALID_PREFIX              0x222
#define ERROR_INVALID_SUFFIX              0x333
#define ERROR_INVALID_NUMBER              0x444

typedef void  (*ExceptionFuncPtr)(Symbol  *symbol, int  errorCode, char *previousStr, char  *currentStr); 

typedef struct  ExceptionTable  ExceptionTable;
struct  ExceptionTable{
  ExceptionFuncPtr  exceptionPtr;
};

//void  symbolThrowException(Symbol *symbol, int errorCode, char *message,...);
void  dumpSymbolErrorMessage(CEXCEPTION_T ex, int lineNo);
void  symbolThrowInfixException(Symbol  *symbol, int  errorCode, char *previousStr, char  *currentStr);
void  symbolThrowPrefixException(Symbol  *symbol, int  errorCode, char *previousStr, char  *currentStr);
void  symbolThrowSuffixException(Symbol  *symbol, int  errorCode, char *previousStr, char  *currentStr);
void  symbolThrowNumberException(Symbol  *symbol, int  errorCode, char *previousStr, char  *currentStr);
#endif // EXCPETIONTHROWING_H
