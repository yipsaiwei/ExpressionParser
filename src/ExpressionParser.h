#ifndef EXPRESSIONPARSER_H
#define EXPRESSIONPARSER_H

#include  <math.h>
#include  <stdlib.h>
#include "OperatorTable.h"
#include "DoubleLinkedList.h"
#include "Stack.h"
#include "Token.h"
#include "Tokenizer.h"
#include "TokenizerExceptionThrowing.h"
#include "Stack.h"
#include  "Arithmetic.h"
#include  "Arity.h"

//typedef void (*Operation)(StackStruct *operandStack);
typedef Symbol  *(*ArithmeticOperation)(Symbol  *number1, Symbol  *number2); 
typedef void    (*operatorStoringOperation)(StackStruct *operandStack, StackStruct *operatorStack, Symbol  *symbol, OperationType previousId);

typedef struct  SymbolTableStruct  SymbolTableStruct;
struct  SymbolTableStruct{
    int precedence;
    ARITY arity;
    ArithmeticOperation arithmeticHandler;
    operatorStoringOperation   storeHandler;       
};

typedef void    (*arityHandleOperator)(StackStruct *operandStack, StackStruct *operatorStack);  
typedef struct  ArityHandler  ArityHandler;
struct  ArityHandler{
    arityHandleOperator  arityHandler;       
};

extern  OperatorInformationTable operatorInformationTable[];

#define UNEXPECTED_OPERATOR                   0x10

#define getItemSymbol(item)                   ((Symbol  *)(*item).data)

#define getSymbolInteger(symbol)              ((IntegerToken *)(*symbol).token)->value
#define getSymbolDouble(symbol)               ((FloatToken *)(*symbol).token)->value
#define getSymbolType(symbol)                 symbol->type
#define getSymbolId(symbol)                   symbol->id
#define getSymbolArity(symbol)                symbol->arity
#define getItemSymbolType(item)               (((Symbol *)(*item).data)->type)
#define getItemSymbolId(item)                 (((Symbol *)(*item).data)->id)
#define getItemSymbolInteger(item)            ((IntegerToken  *)((Symbol  *)(*item).data)->token)->value
#define getItemSymbolDouble(item)             ((FloatToken  *)((Symbol  *)(*item).data)->token)->value

#define comparePrecedence(currentSym, nextSym) returnOperatorPrecedence(currentSym) < returnOperatorPrecedence(nextSym)

#define isCurrentType(type, checkType)        type == checkType
  
#define isTokenNULLType(token)            token->type == TOKEN_NULL_TYPE
#define isLastOperatorInStack(stack)      stack->size == 1

void  shuntingYard(Tokenizer  *tokenizer, StackStruct *operatorStack, StackStruct *operandStack);
void  handleInfix(StackStruct *operandStack, StackStruct *operatorStack);
void  handlePrefix(StackStruct *operandStack, StackStruct *operatorStack);
void  handleSuffix(StackStruct *operandStack, StackStruct *operatorStack);
void  unwindStack(StackStruct *operatorStack, StackStruct *operandStack);
void  unwindStackUntil(StackStruct *operandStack, StackStruct *operatorStack, OperationType type);
void  operateOperatorInOperatorStack(StackStruct  *operandStack, StackStruct  *operatorStack);
void  evaluateExpressionWithinBrackets(StackStruct *operandStack, StackStruct *operatorStack, Symbol *symbol, OperationType previousId);
void pushOperator(StackStruct *operandStack, StackStruct *operatorStack, Symbol  *operatorToPush);
void  forcePush(StackStruct *operandStack, StackStruct *operatorStack, Symbol *symbol, OperationType previousId);
void  pushSymbolToStack(StackStruct *operatorStack, StackStruct *operandStack, Symbol *symbol);
void  symbolizerUpdateLastSymbol(Symbol *symbol, Symbolizer *symbolizer);
Symbol  *symbolizerUpdateLastSymbolAndGetNewSymbol(Symbolizer  *symbolizer, Symbol  *symbol);

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
Symbol  *infixBitwiseOr(Symbol  *number1, Symbol  *number2);
Symbol  *infixBitwiseAnd(Symbol  *number1, Symbol  *number2);
ARITY returnArityOfAnId(OperationType type);

Symbol  *prefixPlus(Symbol  *number1, Symbol  *number2);
Symbol  *prefixMinus(Symbol  *number1, Symbol  *number2);
Symbol  *prefixInc(Symbol  *number1, Symbol  *number2);

int returnOperatorPrecedence(Symbol *symbol);

int verifyArityAllowable(OperationType  previousType, OperationType currentType);
int arityAllowable(OperationType  previousType, OperationType currentType);
void  pushAccordingToPrecedence(StackStruct *operandStack, StackStruct *operatorStack, Symbol  *symbol, OperationType previousId);
void  handleAddOrSub(StackStruct *operandStack, StackStruct *operatorStack, Symbol *symbol, OperationType previousId);
void  handlePreIncOrPostInc(StackStruct *operandStack, StackStruct *operatorStack, Symbol *symbol, OperationType previousId);

char  *createResultString(void  *result, OperationType type);
int countDoubleDigitNumber(double number, int afterpoint);
int countIntegerDigitNumber(int number);
void ftoa(double n, char* res, int afterpoint);
#endif // EXPRESSIONPARSER_H
