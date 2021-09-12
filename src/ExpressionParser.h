#ifndef EXPRESSIONPARSER_H
#define EXPRESSIONPARSER_H

#include  <math.h>
#include  <stdlib.h>
#include  <stdio.h>
#include "OperatorTable.h"
#include "DoubleLinkedList.h"
#include "Stack.h"
#include "Token.h"
#include "Tokenizer.h"
#include "TokenizerExceptionThrowing.h"
#include "Stack.h"
#include  "Arithmetic.h"
#include  "Arity.h"
#include "ExcpetionThrowing.h"
#include "FreeListItemWithSymbol.h"

//typedef void (*Operation)(StackStruct *operandStack);
typedef Symbol  *(*ArithmeticOperation)(Symbol  *number1, Symbol  *number2); 
typedef void    (*operatorStoringOperation)(StackStruct *operandStack, StackStruct *operatorStack, Symbol  *symbol, Symbolizer  *symbolizer);

typedef struct  SymbolTableStruct  SymbolTableStruct;
struct  SymbolTableStruct{
    int precedence;
    ARITY arity;
    ArithmeticOperation arithmeticHandler;
    operatorStoringOperation   storeHandler; 
    char  *idChar;    
};

typedef int    (*arityHandleOperator)(StackStruct *operandStack, StackStruct *operatorStack);  
typedef struct  ArityHandler  ArityHandler;
struct  ArityHandler{
    arityHandleOperator  arityHandler;       
};

extern  OperatorInformationTable operatorInformationTable[];

#define UNEXPECTED_OPERATOR                         0x10

#define getItemSymbol(item)                         ((Symbol  *)(*item).data)

#define getSymbolInteger(symbol)                    ((IntegerToken *)(*symbol).token)->value
#define getSymbolDouble(symbol)                     ((FloatToken *)(*symbol).token)->value
#define getSymbolType(symbol)                       symbol->type
#define getSymbolId(symbol)                         symbol->id
#define getSymbolArity(symbol)                      symbol->arity
#define getItemSymbolType(item)                     (((Symbol *)(*item).data)->type)
#define getItemSymbolId(item)                       (((Symbol *)(*item).data)->id)
#define getItemSymbolInteger(item)                  ((IntegerToken  *)((Symbol  *)(*item).data)->token)->value
#define getItemSymbolDouble(item)                   ((FloatToken  *)((Symbol  *)(*item).data)->token)->value

#define comparePrecedence(currentSym, nextSym)      returnOperatorPrecedence(currentSym->id) < returnOperatorPrecedence(nextSym->id)

#define areTwoIdPrecedenceSame(id1, id2)            returnOperatorPrecedence(id1) == returnOperatorPrecedence(id2)
#define isCurrentType(type, checkType)        type == checkType
  
#define isTokenNULLType(token)            token->type == TOKEN_NULL_TYPE
#define isLastOperatorInStack(stack)      stack->size == 1

void  shuntingYard(Tokenizer  *tokenizer, StackStruct *operatorStack, StackStruct *operandStack);
int   handleInfix(StackStruct *operandStack, StackStruct *operatorStack);
int   handlePrefix(StackStruct *operandStack, StackStruct *operatorStack);
int   handleSuffix(StackStruct *operandStack, StackStruct *operatorStack);
int   unwindStack(StackStruct *operatorStack, StackStruct *operandStack);
int   unwindStackUntil(StackStruct *operandStack, StackStruct *operatorStack, OperationType type);
void  unwindStackForAnArityInSequence(StackStruct *operandStack, StackStruct *operatorStack, ARITY  arity);
int   operateOperatorInOperatorStack(StackStruct  *operandStack, StackStruct  *operatorStack);
void  evaluateExpressionWithinBrackets(StackStruct *operandStack, StackStruct *operatorStack, Symbol *symbol, Symbolizer  *symbolizer);
int   pushOperator(StackStruct *operandStack, StackStruct *operatorStack, Symbol  *operatorToPush);
void  forcePush(StackStruct *operandStack, StackStruct *operatorStack, Symbol *symbol, Symbolizer  *symbolizer);
void  pushSymbolToStack(StackStruct *operatorStack, StackStruct *operandStack, Symbol *symbol);
void  symbolizerUpdateLastSymbol(Symbol *symbol, Symbolizer *symbolizer);
Symbol  *symbolizerUpdateLastSymbolAndGetNewSymbol(Symbolizer  *symbolizer, Symbol  *symbol);
void  checkAndThrowException(Symbol *symbol, Symbolizer *symbolizer, StackStruct  *operatorStack);

Symbol  *infixAdd(Symbol  *number1, Symbol  *number2);
Symbol  *infixMinus(Symbol  *number1, Symbol  *number2);
Symbol  *infixMultiply(Symbol  *number1, Symbol  *number2);
Symbol  *infixEqual(Symbol  *number1, Symbol  *number2);
Symbol  *infixDivide(Symbol  *number1, Symbol  *number2);
Symbol  *infixModulus(Symbol  *number1, Symbol  *number2);
Symbol  *infixShiftLeft(Symbol  *number1, Symbol  *number2);
Symbol  *infixShiftRight(Symbol  *number1, Symbol  *number2);
Symbol  *infixLesser(Symbol  *number1, Symbol  *number2);
Symbol  *infixLesserEq(Symbol  *number1, Symbol  *number2);
Symbol  *infixGreater(Symbol  *number1, Symbol  *number2);
Symbol  *infixGreaterEq(Symbol  *number1, Symbol  *number2);
Symbol  *infixLogicalOr(Symbol  *number1, Symbol  *number2);
Symbol  *infixLogicalAnd(Symbol  *number1, Symbol  *number2);
Symbol  *prefixLogicNot(Symbol  *number1, Symbol *number2);
Symbol  *prefixBitwiseNot(Symbol  *number1, Symbol  *number2);
Symbol  *suffixInc(Symbol  *number1, Symbol  *number2);
Symbol  *suffixDec(Symbol  *number1, Symbol  *number2);
Symbol  *infixBitwiseOr(Symbol  *number1, Symbol  *number2);
Symbol  *infixBitwiseAnd(Symbol  *number1, Symbol  *number2);
ARITY returnArityOfAnId(OperationType type);

Symbol  *prefixPlus(Symbol  *number1, Symbol  *number2);
Symbol  *prefixMinus(Symbol  *number1, Symbol  *number2);
Symbol  *prefixInc(Symbol  *number1, Symbol  *number2);
Symbol  *prefixDec(Symbol  *number1, Symbol  *number2);

int returnOperatorPrecedence(OperationType  type);
char  *returnSymbolCharGivenId(Symbol *symbol);

int verifyArityAllowable(Symbolizer  *symbolizer, Symbol *symbol, OperationType typeToCheck);
int arityAllowable(OperationType  previousType, OperationType currentType);
void  pushAccordingToPrecedence(StackStruct *operandStack, StackStruct *operatorStack, Symbol  *symbol, Symbolizer  *symbolizer);
void  handleAddOrSub(StackStruct *operandStack, StackStruct *operatorStack, Symbol *symbol, Symbolizer  *symbolizer);
void  handlePrefixSuffixInc(StackStruct *operandStack, StackStruct *operatorStack, Symbol *symbol, Symbolizer  *symbolizer);
void  handlePrefixSuffixDec(StackStruct *operandStack, StackStruct *operatorStack, Symbol *symbol, Symbolizer  *symbolizer);
void  handleRightToLeftAssociativity(StackStruct *operandStack, StackStruct *operatorStack, Symbol  *symbol, Symbolizer *symbolizer);
void  handleNumber(StackStruct *operandStack, StackStruct *operatorStack, Symbol *symbol, Symbolizer  *symbolizer);
void  executeStoreHandler(StackStruct  *operandStack, StackStruct  *operatorStack, Symbol  *symbol, Symbolizer *symbolizer);

void  handleException(Symbol  *symbol, Symbolizer *symbolizer, int  errorCode);

char  *createResultString(void  *result, OperationType type);
int countDoubleDigitNumber(double number, int afterpoint);
int countIntegerDigitNumber(int number);
void ftoa(double n, char* res, int afterpoint);
#endif // EXPRESSIONPARSER_H
