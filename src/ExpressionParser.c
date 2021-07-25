#include "ExpressionParser.h"

OperatorTableStruct  operatorPrecedenceTable[127] = {
  ['('] = {1, NULL},  // (
  [')'] = {1, NULL},  // )
  ['['] = {1, NULL},  // [
  [']'] = {1, NULL},  // ]
  ['.'] = {1, NULL},  // .
  ['~'] = {2, NULL},  // ~
  ['!'] = {2, NULL},  // !
  ['*'] = {3, infixMultiplication},  // *
  ['/'] = {3, infixDivision},  // /
  ['%'] = {3, NULL},  // %
  ['+'] = {4, infixAddition},  // +
  ['-'] = {4, infixSubtraction},  // -
};

/*
void  shuntingYard(Tokenizer  *tokenizer, StackStruct *operatorStack, StackStruct *operandStack){
  Token *token = getToken(tokenizer);
  Number  *extractedNumber;
  Operator  *extractedOperator;
  if(token->type == TOKEN_OPERATOR_TYPE){
    if(peekTopOfStack == NULL){
      extractedOperator = extractOperatorFromToken(token);
      pushToStack(operatorStack, (void  *)extractedOperator);
    }else
      unwindStack(operatorStack, operandStack, currentOperator);
    
  }else if (token->type == TOKEN_INTEGER_TYPE || token->type == TOKEN_FLOAT_TYPE){
    extractedNumber = extractNumberFromToken(token);
    pushToStack(operandStack, (void  *)extractedNumber);
  }
}
*/

//take the operator and operate on the operands
//3 different scenarios " infix, prefix, suffix
void  unwindStack(StackStruct *operatorStack, StackStruct *operandStack, Operator *currentOperator){
  ListItem *peekItem = popFromStack(operandStack);
  ListItem  *popItem;
  switch(currentOperator->arity){
    case  BINARY:
      if(getItemOperatorPrecedence(peekItem) >= currentOperator->precedence){
        while(getItemOperatorPrecedence(peekItem) >= currentOperator->precedence){
          popItem = popFromStack(operatorStack);
          OperatorTableStruct instruction = operatorPrecedenceTable[getItemOperator(popItem)];
          instruction.operation(operandStack);
          linkedListFreeListItem(popItem);
        }
      }else
        pushToStack(operatorStack, (void  *)currentOperator);
    break;
    case  UNARY:
    ;
    break;
    
    case  TERNARY:
    ;
    break;
    default:
    ;
  }
}

void  infixAddition(StackStruct *operandStack){
  ListItem *operand2 = popFromStack(operandStack);
  ListItem *operand1 = popFromStack(operandStack);
  Number  *result;
  arithmeticCalculation(operand1, operand2, +);
  pushToStack(operandStack, (void *)result);
  linkedListFreeListItem(operand1);                         
  linkedListFreeListItem(operand2);
}

void  infixSubtraction(StackStruct *operandStack){
  ListItem *operand2 = popFromStack(operandStack);
  ListItem *operand1 = popFromStack(operandStack);
  Number  *result;
  arithmeticCalculation(operand1, operand2, -);
  pushToStack(operandStack, (void *)result);
  linkedListFreeListItem(operand1);                         
  linkedListFreeListItem(operand2);
}

void  infixMultiplication(StackStruct *operandStack){
  ListItem *operand2 = popFromStack(operandStack);
  ListItem *operand1 = popFromStack(operandStack);
  Number  *result;
  arithmeticCalculation(operand1, operand2, *);
  pushToStack(operandStack, (void *)result);
  linkedListFreeListItem(operand1);                         
  linkedListFreeListItem(operand2);
}

void  infixDivision(StackStruct *operandStack){
  ListItem *operand2 = popFromStack(operandStack);
  ListItem *operand1 = popFromStack(operandStack);
  Number  *result;
  arithmeticDivisionCalculation(operand1, operand2);
  pushToStack(operandStack, (void *)result);
  linkedListFreeListItem(operand1);                         
  linkedListFreeListItem(operand2);
}

Integer *createInteger(int  value){
  Integer *newInteger = malloc(sizeof(Integer));
  newInteger->type = INTEGER_NUMBER;
  newInteger->value = value;
  return  newInteger;
}
Double *createDouble(double  value){
  Double *newDouble = malloc(sizeof(Double));
  newDouble->type = FLOAT_NUMBER;
  newDouble->value = value;
  return  newDouble;
}

int isOperandType(void    *number, OPERANDTYPE  type){
  return  (((Number *)number)->type == type);
}

Operator  *createOperator(){
  Operator  *newOperator = malloc(sizeof(Operator));
  return  newOperator;
}

Integer  *extractIntegerFromToken(Token *token){
  Integer *newInteger = malloc(sizeof(Integer));
  IntegerToken  *tokenPointer = (IntegerToken *)token;
  newInteger->type = INTEGER_NUMBER;
  newInteger->value = tokenPointer->value;
  return  newInteger;
}

Double  *extractFloatingPointFromToken(Token *token){
  Double *newFloat = malloc(sizeof(Double));
  FloatToken  *tokenPointer = (FloatToken *)token;
  newFloat->type = FLOAT_NUMBER;
  newFloat->value = tokenPointer->value;
  return  newFloat;
}

Operator  *extractOperatorFromToken(Token *token){
  Operator  *newOperator = createOperator();
  newOperator->str = token->str;              //???
  return  newOperator;
}

void  freeOperator(Operator *operator){
  if(operator)
    free(operator);
}
