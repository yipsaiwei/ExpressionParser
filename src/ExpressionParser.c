#include "ExpressionParser.h"

OperatorTableStruct  operatorPrecedenceTable[] = {
  [OPEN_PAREN]        = {1, NULL, NULL, forcePush},  
  [CLOSE_PAREN]       = {1000, NULL, NULL, NULL},  //Call unwind until '('
  [OPEN_SQ_BRACKET]   = {1000, NULL, NULL, NULL},  
  [CLOSE_SQ_BRACKET]  = {1000, NULL, NULL, NULL},  
  [BITWISE_NOT]       = {2000, prefixBitwiseNot, handlePrefix, NULL},  
  [LOGICAL_NOT]       = {2000, prefixLogicNot, handlePrefix, NULL},  
  [MULTIPLY]          = {3000, infixMultiply, handleInfix, NULL},  
  [DIVIDE]            = {3000, infixDivide, handleInfix, NULL},  
  [REMAINDER]         = {3000, NULL, NULL, NULL},  
  [ADD]               = {4000, infixAdd, handleInfix, NULL},  
  [MINUS]             = {4000, infixMinus, handleInfix, NULL},  
  
  [BITWISE_AND]       = {8000, NULL, NULL},
};

void  shuntingYard(Tokenizer  *tokenizer, StackStruct *operatorStack, StackStruct *operandStack){
  Number  *extractedNumber;
  Operator  *extractedOperator;
  Token *token = getToken(tokenizer);
  ListItem  *popItem;
  while(!(isTokenNULLType(token))){
    if(isTokenOperatorType(token)){
      extractedOperator = extractOperatorFromToken(token, tokenizer);
      while(pushOperator(operatorStack, operandStack, extractedOperator));
    }else{                                    //Integer type
      extractedNumber = extractNumberFromToken(token);
      pushToStack(operandStack, (void  *)extractedNumber);
    }
  token = getToken(tokenizer);
  }
  while(operatorStack->size > 0)
    operateOperatorInOperatorStack(operandStack, operatorStack);
}

//take the operator and operate on the operands
//3 different scenarios " infix, prefix, suffix
//Precedence checking in shuntingYard
//Push operator to the stack
void  unwindStack(StackStruct *operatorStack, StackStruct *operandStack){
  ListItem *peekItem = peekTopOfStack(operatorStack);
  operateOperatorInOperatorStack(operandStack, operatorStack);
  peekItem = peekTopOfStack(operatorStack);
}

void  operateOperatorInOperatorStack(StackStruct  *operandStack, StackStruct  *operatorStack){
    ListItem  *popItem = peekTopOfStack(operatorStack);
    OperatorTableStruct instruction = operatorPrecedenceTable[getItemOperatorId(popItem)];
    instruction.arityHandler(operandStack, operatorStack);  
}

int pushOperator(StackStruct *operandStack, StackStruct *operatorStack, Operator  *operatorToPush){
  ListItem *peekItem = peekTopOfStack(operatorStack);
  if(!isStackEmpty(operatorStack) && comparePrecedence(operatorToPush, getItemOperator(peekItem)) < 1){ 
    unwindStack(operatorStack, operandStack);
    return  0;
  }else{
    pushToStack(operatorStack, (void  *)operatorToPush);
    return  1;
  }
}  

ListItem  *popOperator(StackStruct *operatorStack, OperationType type){
  ListItem  *popItem = popFromStack(operatorStack);
  if(getItemOperatorId(popItem) != type)
    throwException(UNEXPECTED_OPERATOR, NULL, 0, "ERROR code %x : Not the desired operator!", UNEXPECTED_OPERATOR);
  else
    return  popItem;
}

void  forcePush(StackStruct *operandStack, StackStruct *operatorStack, Operator *operator){
  pushToStack(operatorStack, (void  *)operator);
}

void  evaluateExpressionWithinBrackets(StackStruct *operandStack, StackStruct *operatorStack){
  unwindStackUntil(operandStack, operatorStack, OPEN_PAREN);
  ListItem  *operatorItem = popOperator(operatorStack, OPEN_PAREN);
  linkedListFreeListItem(operatorItem); 
}

void  unwindStackUntil(StackStruct *operandStack, StackStruct *operatorStack, OperationType type){
  ListItem  *peekItem = peekTopOfStack(operatorStack);
  while(!isStackEmpty(operatorStack) && getItemOperatorId(peekItem) != type){
    unwindStack(operatorStack, operandStack);
    peekItem = peekTopOfStack(operatorStack);
  }
}

/*
void  operateExpressionBetweenBracket(StackStruct  *operandStack, StackStruct  *operatorStack, Tokenizer  *tokenizer){
  ListItem  *peekItem, *popItem;
  Token *token = getToken(tokenizer);
  Operator  *extractedOperator = extractOperatorFromToken(token, tokenizer);
  Number  *extractedNumber;
  pushToStack(operatorStack, (void  *)extractedOperator);
  token = getToken(tokenizer);
  while(!(isTokenNULLType(token)) && *(token->str) != ')'){
    if(isTokenOperatorType(token) && *(token->str) == '('){
      pushBackToken(tokenizer, token);
      operateExpressionBetweenBracket(operandStack, operatorStack, tokenizer);
    }else if(isTokenOperatorType(token)){
      extractedOperator = extractOperatorFromToken(token, tokenizer);
      pushToStack(operatorStack, (void  *)extractedOperator);
    }else{
      extractedNumber = extractNumberFromToken(token);
      pushToStack(operandStack, (void  *)extractedNumber);      
    }
    token = getToken(tokenizer);
  }
  peekItem = peekTopOfStack(operatorStack);
  while(getItemOperatorStr(peekItem) != '('){
    OperatorTableStruct instruction = operatorPrecedenceTable[getItemOperatorId(peekItem)];
    instruction.arityHandler(operandStack, operatorStack); 
    peekItem = peekTopOfStack(operatorStack);
  }
  popItem = popFromStack(operatorStack);
}
*/

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