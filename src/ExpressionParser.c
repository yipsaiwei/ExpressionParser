#include "ExpressionParser.h"

OperatorTableStruct  operatorPrecedenceTable[] = {
  [OPEN_PAREN]        = {1, NULL, NULL, forcePush},  
  [CLOSE_PAREN]       = {1000, NULL, NULL, evaluateExpressionWithinBrackets},  //Call unwind until '('  
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
  Symbolizer  *symbolizer = createSymbolizer(tokenizer);
  Symbol  *symbol = getSymbol(symbolizer);
  ListItem  *popItem;
  while(!(isSymbolNull(symbol))){
    if(isSymbolOperatorType(symbol))
      pushOperator(operandStack, operatorStack, symbol);
    else                                   
      pushToStack(operandStack, (void  *)symbol);
  symbol = getSymbol(symbolizer);
  }
  while(operatorStack->size > 0)
    unwindStack(operatorStack, operandStack);
  freeSymbolizer(symbolizer);
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

void pushOperator(StackStruct *operandStack, StackStruct *operatorStack, Symbol  *operatorToPush){
  ListItem *peekItem = peekTopOfStack(operatorStack);
  while(!isStackEmpty(operatorStack) && comparePrecedence(operatorToPush, getItemSymbol(peekItem)) < 1){
    unwindStack(operatorStack, operandStack);
    peekItem = peekTopOfStack(operatorStack);
  }
  pushToStack(operatorStack, (void  *)operatorToPush);
}  

ListItem  *popOperator(StackStruct *operatorStack, OperationType type){
  ListItem  *popItem = popFromStack(operatorStack);
  if(getItemSymbolId(popItem) != type)
    throwException(UNEXPECTED_OPERATOR, NULL, 0, "ERROR code %x : Not the desired operator!", UNEXPECTED_OPERATOR);
  else
    return  popItem;
}

void  forcePush(StackStruct *operandStack, StackStruct *operatorStack, Symbol *symbol){
  pushToStack(operatorStack, (void  *)symbol);
}

void  evaluateExpressionWithinBrackets(StackStruct *operandStack, StackStruct *operatorStack, Symbol *symbol){
  unwindStackUntil(operandStack, operatorStack, OPEN_PAREN);
  ListItem  *operatorItem = popOperator(operatorStack, OPEN_PAREN);
  linkedListFreeListItem(operatorItem); 
}

void  unwindStackUntil(StackStruct *operandStack, StackStruct *operatorStack, OperationType type){
  ListItem  *peekItem = peekTopOfStack(operatorStack);
  while(!isStackEmpty(operatorStack) && getItemSymbolId(peekItem) != type){
    unwindStack(operatorStack, operandStack);
    peekItem = peekTopOfStack(operatorStack);
  }
  if(isStackEmpty(operatorStack))
    throwException(UNEXPECTED_OPERATOR, NULL, 0, "ERROR code %x : Not the desired operator!", UNEXPECTED_OPERATOR);
}

//check the index of the top of both stacks to determine which one comes just before this operator to check. 
//if no operator/number before = prefix
//if previous is number = infix / suffix
//if previous is an operator :
    //if previous type is infix = prefix
    //if previous type is prefix = prefix
int arityAllowable(StackStruct *operandStack, StackStruct *operatorStack, Operator *operator){
  
}

int returnOperatorPrecedence(Symbol *symbol){
  OperatorTableStruct instruction = operatorPrecedenceTable[symbol->id];
  return  instruction.precedence;
}

/*
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
*/

void  handleInfix(StackStruct *operandStack, StackStruct *operatorStack){
  ListItem *operand2 = popFromStack(operandStack);
  ListItem *operand1 = popFromStack(operandStack);
  ListItem *operator = popFromStack(operatorStack);
  Symbol  *result;
  OperatorTableStruct instruction = operatorPrecedenceTable[getItemSymbolId(operator)];
  result = instruction.arithmeticHandler(getItemSymbol(operand1), getItemSymbol(operand2));
  pushToStack(operandStack, (void *)result);
  linkedListFreeListItem(operand1);                         
  linkedListFreeListItem(operand2); 
  linkedListFreeListItem(operator);
}

void  handlePrefix(StackStruct *operandStack, StackStruct *operatorStack){
  ListItem *operand = popFromStack(operandStack);
  ListItem *operator = popFromStack(operatorStack);
  Symbol  *result;
  OperatorTableStruct instruction = operatorPrecedenceTable[getItemOperatorId(operator)];
  result = instruction.arithmeticHandler(getItemSymbol(operand), NULL);
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

char  *createResultString(void  *result, OperationType type){
  char  *resultStr;
  if(type == INTEGER){
    int intNumber = *((int  *)result);
    int size = countIntegerDigitNumber(intNumber);
    resultStr = malloc(sizeof(char) * (countIntegerDigitNumber(intNumber) + 1));
    itoa(intNumber, resultStr, 10);
    return  resultStr;
  }else{
    double  doubleNumber = *((double  *)result);
    resultStr = malloc(sizeof(char) * (countDoubleDigitNumber(doubleNumber, 6) + 1));
    ftoa(doubleNumber, resultStr, 6);
    return  resultStr;
  }
}

int countIntegerDigitNumber(int number){
  if(number < 0){
    number *= (-1);
    return  (number == 0) ? 1 : (log10(number) + 2);
  }else
    return  (number == 0) ? 1 : (log10(number) + 1);
}

int countDoubleDigitNumber(double number, int afterpoint){
  int intSize;
  if(number < 0){
    number *= (-1);
    intSize = countIntegerDigitNumber((int)number) + 1;
  }else
    intSize = countIntegerDigitNumber((int)number);
  int count;
  for(count = 0; number >= 0 && count < afterpoint; count++){
    number = number - (int)number;
    number *= 10;
  }
  return  (count + intSize + 1);
}

// Converts a floating-point/double number to a string.
void ftoa(double n, char* res, int afterpoint){
  int ipart = (int)n;
  char  *strPtr;
  
  double fpart = n - (double)ipart;
  
  if(fpart < 0)
    fpart *= (-1);
  itoa(ipart, res, 10);
    
  int i = countIntegerDigitNumber(ipart);
  if (afterpoint != 0) {
    res[i] = '.'; 
    fpart = fpart * pow(10, afterpoint);
    strPtr = (res + i + 1);
    itoa((int)fpart, strPtr, 10);
  }
  /*
  if(afterpoint != 0){
    *(res + i) = '.';
    strPtr = res + 1;
    for(int j = 0; j < afterpoint; j++){
      fpart *= 10;
      itoa((int)fpart, strPtr + j, 
    }
  }
  */
}