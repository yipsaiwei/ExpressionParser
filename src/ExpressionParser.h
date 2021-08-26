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

typedef enum
{
	INTEGER_NUMBER,
	FLOAT_NUMBER,
}OPERANDTYPE;

/*
typedef enum{
  NULLARY, 
	UNARY,
	BINARY,
  TERNARY,
}ARITY;
*/

typedef struct  Number  Number;
struct  Number{
  OPERANDTYPE  type;
};

typedef struct  Integer Integer;
struct  Integer{
  OPERANDTYPE type;
  int value;
};

typedef struct  Double  Double;
struct  Double{
  OPERANDTYPE type;
  double value;
};

//typedef void (*Operation)(StackStruct *operandStack);
typedef Symbol  *(*ArithmeticOperation)(Symbol  *number1, Symbol  *number2);
typedef void    (*postHandleOperator)(StackStruct *operandStack, StackStruct *operatorStack);
typedef void    (*preHandleOperator)(StackStruct *operandStack, StackStruct *operatorStack, Symbol  *operator);

typedef struct  OperatorTableStruct  OperatorTableStruct;
struct  OperatorTableStruct{
    int precedence;
    ArithmeticOperation arithmeticHandler;
    postHandleOperator  arityHandler;
    preHandleOperator   arityAssign;
    int arity;
};

extern  OperatorInformationTable operatorInformationTable[];
//extern  struct  Operator;

#define UNEXPECTED_OPERATOR                   0x10
#define getItemOperator(item)                 ((Operator  *)(*item).data)
#define getItemOperatorStr(item)              *(((Operator  *)(*item).data)->str)
#define getItemDataType(item)                 (((Number *)(*item).data)->type)
#define getItemOperatorPrecedence(item)       (((Operator *)(*item).data)->precedence)
#define getItemOperatorId(item)               (((Operator *)(*item).data)->id)

#define getItemInteger(item)                  ((Integer  *)(*item).data)->value
#define getNumberInteger(number)              ((Integer *)(number))->value
#define getItemDouble(item)                   ((Double  *)(*item).data)->value
#define getNumberDouble(number)               ((Double  *)(number))->value
#define getItemSymbol(item)                   ((Symbol  *)(*item).data)

#define getSymbolInteger(symbol)              ((IntegerToken *)(*symbol).token)->value
#define getSymbolDouble(symbol)               ((FloatToken *)(*symbol).token)->value
#define getItemSymbolType(item)               (((Symbol *)(*item).data)->type)
#define getItemSymbolId(item)                 (((Symbol *)(*item).data)->id)
#define getItemSymbolInteger(item)            ((IntegerToken  *)((Symbol  *)(*item).data)->token)->value
#define getItemSymbolDouble(item)             ((FloatToken  *)((Symbol  *)(*item).data)->token)->value

#define comparePrecedence(currentSym, nextSym) returnOperatorPrecedence(currentSym) < returnOperatorPrecedence(nextSym)
  
//#define isTokenOperatorType(token)        token->type == TOKEN_OPERATOR_TYPE
#define isTokenNULLType(token)            token->type == TOKEN_NULL_TYPE
#define isLastOperatorInStack(stack)      stack->size == 1

int obtainOperatorPrecedence(Token  *token);
int  checkOperator1PrecedenceGreater(Token  *operatorToken1, Token  *operatorToken2);
int isOperandType(void    *number, OPERANDTYPE  type);
Integer *createInteger(int  value);
Double *createDouble(double  value);
Operator  *extractOperatorFromToken(Token *token, Tokenizer *tokenizer);
Number  *extractNumberFromToken(Token *token);
Integer  *extractIntegerFromToken(Token *token);
Double  *extractFloatingPointFromToken(Token *token);
void  shuntingYard(Tokenizer  *tokenizer, StackStruct *operatorStack, StackStruct *operandStack);
void  handleInfix(StackStruct *operandStack, StackStruct *operatorStack);
void  handlePrefix(StackStruct *operandStack, StackStruct *operatorStack);
void  unwindStack(StackStruct *operatorStack, StackStruct *operandStack);
void  unwindStackUntil(StackStruct *operandStack, StackStruct *operatorStack, OperationType type);
void  operateOperatorInOperatorStack(StackStruct  *operandStack, StackStruct  *operatorStack);
void  evaluateExpressionWithinBrackets(StackStruct *operandStack, StackStruct *operatorStack, Symbol *symbol);
void pushOperator(StackStruct *operandStack, StackStruct *operatorStack, Symbol  *operatorToPush);
void  forcePush(StackStruct *operandStack, StackStruct *operatorStack, Symbol *symbol);

Symbol  *infixAdd(Symbol  *number1, Symbol  *number2);
Symbol  *infixMinus(Symbol  *number1, Symbol  *number2);
Symbol  *infixMultiply(Symbol  *number1, Symbol  *number2);
Symbol  *infixDivide(Symbol  *number1, Symbol  *number2);
Symbol  *infixModulus(Symbol  *number1, Symbol  *number2);
Symbol  *prefixLogicNot(Symbol  *number1, Symbol *number2);
Symbol  *prefixBitwiseNot(Symbol  *number1, Symbol  *number2);

int returnOperatorPrecedence(Symbol *symbol);

char  *createResultString(void  *result, OperationType type);
int countDoubleDigitNumber(double number, int afterpoint);
int countIntegerDigitNumber(int number);
void ftoa(double n, char* res, int afterpoint);
#endif // EXPRESSIONPARSER_H
