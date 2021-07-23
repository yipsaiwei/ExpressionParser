#ifndef EXPRESSIONPARSER_H
#define EXPRESSIONPARSER_H

#include "Token.h"
#include "Tokenizer.h"
#include "TokenizerExceptionThrowing.h"
#include "Stack.h"

typedef void (*Operation)(StackStruct *operandStack);

typedef enum
{
	INTEGER_NUMBER,
	FLOAT_NUMBER,
}NUMBERTYPE;

typedef enum
{
	UNARY,
	BINARY,
  TERNARY,
}ARITY;

typedef struct  Number  Number;
struct  Number{
  void  *value;
  NUMBERTYPE  type;
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

#define getPrecedence(token)                (operatorPrecedenceTable[*((*token).str)].precedence)
#define getItemType(item)                   (item->data->type)
#define getItemDataInteger(item)            *((int  *)((Number *)(*item).data)->value)
#define getItemDataFloat(item)              *((double  *)((Number *)(*item).data)->value)
#define getItemOperator(item)               *(((Operator  *)(*popItem).data)->str)
#define getItemDataType(item)               (((Number *)(*item).data)->type)
#define getItemOperatorPrecedence(item)     (((Operator *)(*item).data)->precedence)

int obtainOperatorPrecedence(Token  *token);
int  checkOperator1PrecedenceGreater(Token  *operatorToken1, Token  *operatorToken2);
void  infixAddition(StackStruct *operandStack);
void  infixSubtraction(StackStruct *operandStack);
void  infixMultiplication(StackStruct *operandStack);
void  infixDivision(StackStruct *operandStack);
Number  *extractNumberFromToken(Token *token);
Number  *createNumber(void  *value, NUMBERTYPE  type);
void  freeNumber(Number *number);
Operator  *createOperator();
Operator  *extractOperatorFromToken(Token *token);
void  freeOperator(Operator *operator);

#endif // EXPRESSIONPARSER_H
