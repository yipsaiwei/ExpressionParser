#include "ExpressionParser.h"

OperatorTableStruct  operatorPrecedenceTable[127] = {
  ['('] = {1, NULL},  // (
  [')'] = {1, NULL},  // )
  ['['] = {1, NULL},  // [
  [']'] = {1, NULL},  // ]
  ['.'] = {1, NULL},  // .
  ['~'] = {2, NULL},  // ~
  ['!'] = {2, NULL},  // !
  ['*'] = {3, NULL},  // *
  ['/'] = {3, NULL},  // /
  ['%'] = {3, NULL},  // %
  ['+'] = {4, NULL},  // +
  ['-'] = {4, NULL},  // -
};

/*
void  shuntingYard(Tokenizer  *tokenizer, StackStruct *operatorStack, StackStruct *operandStack){
  Token *token = getToken(tokenizer);
  if(token->type == TOKEN_OPERATOR_TYPE){
    if(peekTopOfStack == NULL)
      pushToStack(operatorStack, (void  *)token);
    else
      unwindStack();
  }else if (token->type == TOKEN_INTEGER_TYPE)
}
*/

int  checkOperator1PrecedenceGreater(Token  *operatorToken1, Token  *operatorToken2){
  return  (getPrecedence(operatorToken1) < getPrecedence(operatorToken2));
}

//take the operator and operate on the operands
//3 different scenarios " infix, prefix, suffix

/*
void  unwindStack(StackStruct *operatorStack, StackStruct *operandStack){
  
}
*/

//Not yet consider floating type token
void  infixAddition(StackStruct *operandStack){
  ListItem *operand2 = popFromStack(operandStack);
  ListItem *operand1 = popFromStack(operandStack);
  Number  *resultNumber;
  if(getItemDataType(operand2) == INTEGER_NUMBER && getItemDataType(operand1) == INTEGER_NUMBER){
    int *result = malloc(sizeof(int));
    *result = getItemDataInteger(operand2) + getItemDataInteger(operand1); 
    resultNumber = createNumber((void *)result, INTEGER_NUMBER);
  }else{
    double  *result = malloc(sizeof(double));
    *result = getItemDataFloat(operand2) + getItemDataFloat(operand1); 
    resultNumber = createNumber((void *)result, FLOAT_NUMBER);
  }
  linkedListFreeListItem(operand1);                         
  linkedListFreeListItem(operand2);
  pushToStack(operandStack, (void *)resultNumber);
}

//Not yet consider floating type token
/*
void  infixSubtraction(StackStruct *operandStack){
  ListItem *operand2 = popFromStack(operandStack);
  ListItem *operand1 = popFromStack(operandStack);
  
  int result = getItemDataInteger(operand1) - getItemDataInteger(operand2);
  char  *numberStr = malloc(sizeof(1 + countDigit(result)));
  itoa(result, numberStr, 10);
  
  Tokenizer *resultTokenizer = createTokenizer(numberStr);
  Token *resultToken = getToken(resultTokenizer);
  
  linkedListFreeListItem(operand1);
  linkedListFreeListItem(operand2);
  freeTokenizer(resultTokenizer);
  
  pushToStack(operandStack, (void *)resultToken);
}
*/

Number  *createNumber(void  *value, NUMBERTYPE  type){
  Number  *newNumber = malloc(sizeof(Number));
  newNumber->value = value;
  newNumber->type = type;
  return  newNumber;
}

Operator  *createOperator(){
  Operator  *newOperator = malloc(sizeof(Operator));
  return  newOperator;
}

Number  *extractNumberFromToken(Token *token){
  Number  *newNumber;
  if(token->type == TOKEN_INTEGER_TYPE){
    int *newInteger = malloc(sizeof(int));
    IntegerToken  *tokenPointer = (IntegerToken *)token;
    *newInteger = tokenPointer->value;
    newNumber = createNumber((void  *)newInteger, INTEGER_NUMBER);
  } else {
    double *newFloat = malloc(sizeof(double));
    FloatToken  *tokenPointer = (FloatToken *)token;
    *newFloat = tokenPointer->value;
    newNumber = createNumber((void  *)newFloat, FLOAT_NUMBER);
  }
  return  newNumber;
}

Operator  *extractOperatorFromToken(Token *token){
  Operator  *newOperator = createOperator();
  newOperator->str = token->str;
  return  newOperator;
}

void  freeNumber(Number *number){
  if(number->value)
    free(number->value);
  if(number)
    free(number);
}

void  freeOperator(Operator *operator){
  if(operator)
    free(operator);
}
