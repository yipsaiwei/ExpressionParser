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

#define getItemOperator(item)               *(((Operator  *)(*item).data)->str)
#define getItemDataType(item)               (((Number *)(*item).data)->type)
#define getItemOperatorPrecedence(item)     (((Operator *)(*item).data)->precedence)

#define getItemInteger(item)                ((Integer  *)(*item).data)->value
#define getNumberInteger(number)            ((Integer *)(number))->value
#define getItemDouble(item)                 ((Double  *)(*item).data)->value
#define getNumberDouble(number)             ((Double  *)(number))->value
#define getItemNumber(item)                 ((Number  *)(*item).data)

int obtainOperatorPrecedence(Token  *token);
int  checkOperator1PrecedenceGreater(Token  *operatorToken1, Token  *operatorToken2);
Number  *infixAddition(Number  *operand1, Number *operand2);
Number  *infixSubtraction(Number  *operand1, Number *operand2);
Number  *infixMultiplication(Number  *operand1, Number *operand2);
Number  *infixDivision(Number  *operand1, Number *operand2);
int isOperandType(void    *number, OPERANDTYPE  type);
Integer *createInteger(int  value);
Double *createDouble(double  value);
Integer  *extractIntegerFromToken(Token *token);
Double  *extractFloatingPointFromToken(Token *token);
void  unwindStack(StackStruct *operatorStack, StackStruct *operandStack, Operator *currentOperator);
void  handleInfix(StackStruct *operandStack, StackStruct *operatorStack);

#endif // EXPRESSIONPARSER_H
