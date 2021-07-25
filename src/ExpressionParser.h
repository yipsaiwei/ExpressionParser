#ifndef EXPRESSIONPARSER_H
#define EXPRESSIONPARSER_H

#include "DoubleLinkedList.h"
#include "Stack.h"
#include "Token.h"
#include "Tokenizer.h"
#include "TokenizerExceptionThrowing.h"
#include "Stack.h"
#include  "Arithmetic.h"

typedef void (*Operation)(StackStruct *operandStack);

typedef enum
{
	INTEGER_NUMBER,
	FLOAT_NUMBER,
}OPERANDTYPE;

typedef enum
{
	UNARY,
	BINARY,
  TERNARY,
}ARITY;

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

typedef struct  OperatorTableStruct  OperatorTableStruct;
struct  OperatorTableStruct{
    int precedence;
    Operation operation;
};

typedef struct  Operator  Operator;
struct  Operator{
  char  *str;
  int precedence;
  ARITY arity;
};

//#define getItemType(item)                   (item->data->type)
//#define getItemDataInteger(item)            *((int  *)((Number *)(*item).data)->value)
//#define getItemDataFloat(item)              *((double  *)((Number *)(*item).data)->value)
#define getItemOperator(item)               *(((Operator  *)(*item).data)->str)
#define getItemDataType(item)               (((Number *)(*item).data)->type)
#define getItemOperatorPrecedence(item)     (((Operator *)(*item).data)->precedence)

#define getItemInteger(item)                ((Integer  *)(*item).data)->value
#define getItemDouble(item)                ((Double  *)(*item).data)->value

int obtainOperatorPrecedence(Token  *token);
int  checkOperator1PrecedenceGreater(Token  *operatorToken1, Token  *operatorToken2);
void  infixAddition(StackStruct *operandStack);
void  infixSubtraction(StackStruct *operandStack);
void  infixMultiplication(StackStruct *operandStack);
void  infixDivision(StackStruct *operandStack);
int isOperandType(void    *number, OPERANDTYPE  type);
Integer *createInteger(int  value);
Double *createDouble(double  value);
Integer  *extractIntegerFromToken(Token *token);
Double  *extractFloatingPointFromToken(Token *token);
Operator  *createOperator();
Operator  *extractOperatorFromToken(Token *token);
void  freeOperator(Operator *operator);

#endif // EXPRESSIONPARSER_H
