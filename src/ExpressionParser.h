#ifndef EXPRESSIONPARSER_H
#define EXPRESSIONPARSER_H

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
typedef Number  *(*ArithmeticOperation)(Number  *number1, Number  *number2);
typedef void    (*ArityHandler)(StackStruct *operandStack, StackStruct *operatorStack);

typedef struct  OperatorTableStruct  OperatorTableStruct;
struct  OperatorTableStruct{
    int precedence;
    ArithmeticOperation arithmeticHandler;
    ArityHandler  arityHandler;
    int arity;
};

extern  OperatorInformationTable operatorInformationTable[];
//extern  struct  Operator;

#define getItemOperator(item)               ((Operator  *)(*item).data)
#define getItemOperatorStr(item)            *(((Operator  *)(*item).data)->str)
#define getItemDataType(item)               (((Number *)(*item).data)->type)
#define getItemOperatorPrecedence(item)     (((Operator *)(*item).data)->precedence)
#define getItemOperatorId(item)             (((Operator *)(*item).data)->id)

#define getItemInteger(item)                ((Integer  *)(*item).data)->value
#define getNumberInteger(number)            ((Integer *)(number))->value
#define getItemDouble(item)                 ((Double  *)(*item).data)->value
#define getNumberDouble(number)             ((Double  *)(number))->value
#define getItemNumber(item)                 ((Number  *)(*item).data)

#define isCurrentOperatorPrecedenceLower(currentOp, nextOp)           \
  nextOp->precedence <= currentOp->precedence
  
#define isTokenOperatorType(token)        token->type == TOKEN_OPERATOR_TYPE
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
void  unwindStack(StackStruct *operatorStack, StackStruct *operandStack, Operator *currentOperator);

Number  *infixAdd(Number  *number1, Number  *number2);
Number  *infixMinus(Number  *number1, Number  *number2);
Number  *infixMultiply(Number  *number1, Number  *number2);
Number  *infixDivide(Number  *number1, Number  *number2);
Number  *infixModulus(Number  *number1, Number  *number2);
Number  *prefixLogicNot(Number  *number1, Number *number2);
Number  *prefixBitwiseNot(Number  *number1, Number  *number2);
#endif // EXPRESSIONPARSER_H
