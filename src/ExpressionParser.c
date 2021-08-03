#include "ExpressionParser.h"

OperatorTableStruct  operatorPrecedenceTable[] = {
  ['('] = {1, NULL, NULL},  
  [')'] = {1, NULL, NULL},  
  ['['] = {1, NULL, NULL},  
  [']'] = {1, NULL, NULL},  
  ['~'] = {2, NULL, NULL},  
  ['!'] = {2, NULL, NULL},  
  ['*'] = {3, infixMultiplication, handleInfix},  
  ['/'] = {3, infixDivision, handleInfix},  
  ['%'] = {3, NULL, NULL},  
  ['+'] = {4, infixAddition, handleInfix},  
  ['-'] = {4, infixSubtraction, handleInfix},  
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
    
  }else{
    extractedNumber = extractNumberFromToken(token);
    pushToStack(operandStack, (void  *)extractedNumber);
  }
}
*/

//take the operator and operate on the operands
//3 different scenarios " infix, prefix, suffix
/*
void  unwindStack(StackStruct *operatorStack, StackStruct *operandStack, Operator *currentOperator){
  ListItem *peekItem = peekTopOfStack(operatorStack);
  ListItem  *popItem;
  switch(currentOperator->arity){
    case  BINARY:
      if(getItemOperatorPrecedence(peekItem) <= currentOperator->precedence){
        while(!isStackEmpty(operatorStack) && (getItemOperatorPrecedence(peekItem) <= currentOperator->precedence)){
          popItem = popFromStack(operatorStack);
          OperatorTableStruct instruction = operatorPrecedenceTable[getItemOperator(popItem)];
          instruction.operation(operandStack);
          linkedListFreeListItem(popItem);
          peekItem = peekTopOfStack(operatorStack);
        }
        pushToStack(operatorStack, (void  *)currentOperator);
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
*/

void  handleInfix(StackStruct *operandStack, StackStruct *operatorStack){
  ListItem *operand2 = popFromStack(operandStack);
  ListItem *operand1 = popFromStack(operandStack);
  ListItem *operator = popFromStack(operatorStack);
  Number  *result;
  OperatorTableStruct instruction = operatorPrecedenceTable[getItemOperator(operator)];
  result = instruction.arithmeticHandler(getItemNumber(operand1), getItemNumber(operand2));
  pushToStack(operandStack, (void *)result);
  linkedListFreeListItem(operand1);                         
  linkedListFreeListItem(operand2); 
  linkedListFreeListItem(operator);
}

Number  *infixAddition(Number  *operand1, Number *operand2){
  Number  *result;
  arithmeticCalculation(operand1, operand2, +);
  return  result;
}

Number  *infixSubtraction(Number  *operand1, Number *operand2){
  Number  *result;
  arithmeticCalculation(operand1, operand2, -);
  return  result;
}

Number  *infixMultiplication(Number  *operand1, Number *operand2){
  Number  *result;
  arithmeticCalculation(operand1, operand2, *);
  return  result;
}

Number  *infixDivision(Number  *operand1, Number *operand2){
  Number  *result;
  arithmeticDivisionCalculation(operand1, operand2, /);
  return  result;
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

char  *duplicateString(char *str, int length){
  char  *resultstr = memAlloc((length+1)*sizeof(char));
  strncpy(resultstr, str, length);
  resultstr[length] = '\0';
  return  resultstr;
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