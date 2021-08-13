#include "ExpressionParser.h"

OperatorTableStruct  operatorPrecedenceTable[] = {
  [OPEN_PAREN]        = {1, NULL, NULL},  
  [CLOSE_PAREN]       = {1, NULL, NULL},  
  [OPEN_SQ_BRACKET]   = {1, NULL, NULL},  
  [CLOSE_SQ_BRACKET]  = {1, NULL, NULL},  
  [BITWISE_NOT]       = {2, prefixBitwiseNot, handlePrefix},  
  [LOGICAL_NOT]       = {2, prefixLogicNot, handlePrefix},  
  [MULTIPLY]          = {3, infixMultiply, handleInfix},  
  [DIVIDE]            = {3, infixDivide, handleInfix},  
  [REMAINDER]         = {3, NULL, NULL},  
  [ADD]               = {4, infixAdd, handleInfix},  
  [MINUS]             = {4, infixMinus, handleInfix},  
  
  [BITWISE_AND]       = {8, NULL, NULL},
};

void  shuntingYard(Tokenizer  *tokenizer, StackStruct *operatorStack, StackStruct *operandStack){
  Number  *extractedNumber;
  Operator  *extractedOperator;
  Token *token = getToken(tokenizer);
  ListItem  *popItem;
  while(!(isTokenNULLType(token))){
    if(isTokenOperatorType(token)){   //Check whether it is operator type
      extractedOperator = extractOperatorFromToken(token, tokenizer);
      if(peekTopOfStack(operatorStack) == NULL)
        pushToStack(operatorStack, (void  *)extractedOperator);
      else
        unwindStack(operatorStack, operandStack, extractedOperator);
    }else{                                    //Integer type
      extractedNumber = extractNumberFromToken(token);
      pushToStack(operandStack, (void  *)extractedNumber);
    }
  token = getToken(tokenizer);
  }
  while(operatorStack->size > 0){
    //popItem = popFromStack(operatorStack);
    //OperatorTableStruct instruction = operatorPrecedenceTable[getItemOperatorId(popItem)];
    //instruction.arityHandler(operandStack, operatorStack);
    unwindStack(operatorStack, operandStack, NULL);  
  }
}

Operator  *extractOperatorFromToken(Token *token, Tokenizer *tokenizer){
  Operator  *operator;
  char  *operatorStr;
  Token *nextToken = peekToken(tokenizer);
  OperatorInformationTable  information = operatorInformationTable[*(token->str)];
  if(isNextTokenAOperator(nextToken) && isNextTokenAdjacentToCurrent(token, nextToken))
    operator = information.func(token, tokenizer);   
  else{
    operator = createOperator(token->str, 0, information.type[0]); 
    freeToken(token);
  }
  
  OperatorTableStruct operatorInfo = operatorPrecedenceTable[operator->id];
  operator->precedence = operatorInfo.precedence;
  return  operator;
}

//take the operator and operate on the operands
//3 different scenarios " infix, prefix, suffix
void  unwindStack(StackStruct *operatorStack, StackStruct *operandStack, Operator *currentOperator){
  ListItem *peekItem = peekTopOfStack(operatorStack);
  if(!currentOperator)
    printf("hello world");
  if(peekItem != NULL && (isLastOperatorInStack(operatorStack) || isCurrentOperatorPrecedenceLower(currentOperator, getItemOperator(peekItem)))){
    while(!isStackEmpty(operatorStack) && (isLastOperatorInStack(operatorStack)|| isCurrentOperatorPrecedenceLower(currentOperator, getItemOperator(peekItem)))){
      OperatorTableStruct instruction = operatorPrecedenceTable[getItemOperatorId(peekItem)];
      instruction.arityHandler(operandStack, operatorStack);
      peekItem = peekTopOfStack(operatorStack);
    }  
  }
  if(currentOperator != NULL)
    pushToStack(operatorStack, (void  *)currentOperator);
}

void  handleInfix(StackStruct *operandStack, StackStruct *operatorStack){
  ListItem *operand2 = popFromStack(operandStack);
  ListItem *operand1 = popFromStack(operandStack);
  ListItem *operator = popFromStack(operatorStack);
  Number  *result;
  OperatorTableStruct instruction = operatorPrecedenceTable[getItemOperatorId(operator)];
  result = instruction.arithmeticHandler(getItemNumber(operand1), getItemNumber(operand2));
  pushToStack(operandStack, (void *)result);
  linkedListFreeListItem(operand1);                         
  linkedListFreeListItem(operand2); 
  linkedListFreeListItem(operator);
}

void  handlePrefix(StackStruct *operandStack, StackStruct *operatorStack){
  ListItem *operand = popFromStack(operandStack);
  ListItem *operator = popFromStack(operatorStack);
  Number  *result;
  OperatorTableStruct instruction = operatorPrecedenceTable[getItemOperatorId(operator)];
  result = instruction.arithmeticHandler(getItemNumber(operand), NULL);
  pushToStack(operandStack, (void *)result);
  linkedListFreeListItem(operand); 
  linkedListFreeListItem(operator);  
}

createArithmeticFunction(infixAdd, +);
createArithmeticFunction(infixMinus, -);
createArithmeticFunction(infixMultiply, *);
createArithmeticDivFunction(infixDivide, /);

createInfixLogicFunction(infixModulus, %);
createPrefixLogicFunction(prefixLogicNot, !);
createPrefixLogicFunction(prefixBitwiseNot, ~);

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

Number  *extractNumberFromToken(Token *token){
  Number  *result;
  if(token->type == TOKEN_FLOAT_TYPE)
    result = (Number  *)extractFloatingPointFromToken(token);
  else
    result = (Number  *)extractIntegerFromToken(token);
  return  result;
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