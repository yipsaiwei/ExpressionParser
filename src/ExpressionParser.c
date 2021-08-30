#include "ExpressionParser.h"

//SymbolTableStruct   symbolTable
SymbolTableStruct  operatorPrecedenceTable[] = {
  [OPEN_PAREN]        = {10000, PREFIX, NULL,             NULL,         forcePush},  
  [CLOSE_PAREN]       = {1000 , SUFFIX, NULL,             NULL,         evaluateExpressionWithinBrackets},  //Call unwind until '('  
  [OPEN_SQ_BRACKET]   = {1000 , PREFIX, NULL,             NULL,         forcePush},  
  [CLOSE_SQ_BRACKET]  = {1000 , SUFFIX, NULL,             NULL,         evaluateExpressionWithinBrackets},  
  [BITWISE_NOT]       = {2000 , PREFIX, prefixBitwiseNot, handlePrefix, NULL},  
  [LOGICAL_NOT]       = {2000 , PREFIX, prefixLogicNot,   handlePrefix, NULL},  
  [MULTIPLY]          = {3000 , INFIX , infixMultiply,    handleInfix,  NULL},  
  [DIVIDE]            = {3000 , INFIX , infixDivide,      handleInfix,  NULL},  
  [REMAINDER]         = {3000 , INFIX , infixModulus,     handleInfix,  NULL},  //pushAccordingToPrecedence
  [ADD]               = {4000 , INFIX , infixAdd,         handleInfix,  NULL},  //handleAddOrSub
  [PRE_INC]           = {1000 , PREFIX, prefixInc,        handlePrefix, NULL},  //handleAddOrSub
  [POST_INC]          = {1000 , SUFFIX, infixAdd,         NULL,         NULL},  //handleAddOrSub
  [MINUS]             = {4000 , INFIX , infixMinus,       handleInfix,  NULL},    
  [BITWISE_AND]       = {8000 , INFIX , infixBitwiseAnd,  handleInfix,  NULL},
  [PLUS_SIGN]         = {2000 , PREFIX, prefixPlus,       handlePrefix, NULL}, 
  [MINUS_SIGN]        = {2000 , PREFIX, prefixMinus,      handlePrefix, NULL}, 
  [INTEGER]           = {0,     NUMBER, NULL,             NULL,         NULL},//0, NUMBER, NULL, NULL, pushToOperandStack
};

void  shuntingYard(Tokenizer  *tokenizer, StackStruct *operatorStack, StackStruct *operandStack){
  Number  *extractedNumber;
  Symbolizer  *symbolizer = createSymbolizer(tokenizer);
  Symbol  *symbol = symbolizerGetSymbol(symbolizer);
  ListItem  *popItem;
  while(!(isSymbolNull(symbol))){
    pushSymbolToStack(operatorStack, operandStack, symbol);
    symbolizerUpdateLastSymbol(symbol, symbolizer);
    symbol = symbolizerGetSymbol(symbolizer);
  }
  while(operatorStack->size > 0)
    unwindStack(operatorStack, operandStack);
  freeSymbolizer(symbolizer);
}

Symbol  *symbolizerGetSymbol(Symbolizer  *symbolizer){
  Symbol  *symbol = getSymbol(symbolizer);
  symbol->arity = returnArityOfAnId(symbol->id);
  //symbolizerUpdateLastSymbol(symbol, symbolizer);  
  return  symbol;
}

void  symbolizerUpdateLastSymbol(Symbol *symbol, Symbolizer *symbolizer){
  symbolizer->lastSymbolId = symbol->id;
  //symbolizer->lastSymbolArity = symbol->arity;  
}

void  pushSymbolToStack(StackStruct *operatorStack, StackStruct *operandStack, Symbol *symbol){
  if(isSymbolOperatorType(symbol))
    pushOperator(operandStack, operatorStack, symbol);
  else                                   
    pushToStack(operandStack, (void  *)symbol);  
}

//typedef void    (*preHandleOperator)(StackStruct *operandStack, StackStruct *operatorStack, Symbol  *operator);
void  handleAddOrSub(Symbolizer *symbolizer, Symbol *symbol){
  
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
  SymbolTableStruct instruction = operatorPrecedenceTable[getItemSymbolId(popItem)];
  instruction.arityHandler(operandStack, operatorStack);  
}

void  pushAccordingToPrecedence(StackStruct *operandStack, StackStruct *operatorStack, Symbol  *symbol){
  
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
    
//verifyArityAllowable
int arityAllowable(OperationType  previousType, OperationType currentType){
  switch(returnArityOfAnId(currentType)){
    case  INFIX:
      if(isIdArity(previousType, SUFFIX) || isIdArity(previousType, NUMBER))
        return  1;
      else
        return  0;
      break;
    case  PREFIX:
      if(isIdArity(previousType, INFIX) || isIdArity(previousType, PREFIX) || isIdArity(previousType, NONE))
        return  1;
      else
        return  0;
      break;
    case  SUFFIX:
      if(isIdArity(previousType, NUMBER))
        return  1;
      else
        return  0;
     break;
    case  NUMBER:
      if(isIdArity(previousType, NONE) || isIdArity(previousType, PREFIX) || isIdArity(previousType, INFIX))
        return  1;
      else
        return  0;
    break;
    default:
      return  0;
  }  
}

int verifyArityAllowable(OperationType  previousType, OperationType currentType){
  switch(returnArityOfAnId(currentType)){
    case  INFIX:
      if(isIdArity(previousType, SUFFIX) || isIdArity(previousType, NUMBER))
        return  1;
      else
        return  0;
      break;
    case  PREFIX:
      if(isIdArity(previousType, INFIX) || isIdArity(previousType, PREFIX) || isIdArity(previousType, NONE))
        return  1;
      else
        return  0;
      break;
    case  SUFFIX:
      if(isIdArity(previousType, NUMBER))
        return  1;
      else
        return  0;
     break;
    case  NUMBER:
      if(isIdArity(previousType, NONE) || isIdArity(previousType, PREFIX) || isIdArity(previousType, INFIX))
        return  1;
      else
        return  0;
    break;
    default:
      return  0;
  }
}

int returnOperatorPrecedence(Symbol *symbol){
  SymbolTableStruct instruction = operatorPrecedenceTable[symbol->id];
  return  instruction.precedence;
}

void  handleInfix(StackStruct *operandStack, StackStruct *operatorStack){
  ListItem *operand2 = popFromStack(operandStack);
  ListItem *operand1 = popFromStack(operandStack);
  ListItem *operator = popFromStack(operatorStack);
  Symbol  *result;
  SymbolTableStruct instruction = operatorPrecedenceTable[getItemSymbolId(operator)];
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
  SymbolTableStruct instruction = operatorPrecedenceTable[getItemSymbolId(operator)];
  result = instruction.arithmeticHandler(getItemSymbol(operand), NULL);
  pushToStack(operandStack, (void *)result);
  linkedListFreeListItem(operand); 
  linkedListFreeListItem(operator);  
}

ARITY returnArityOfAnId(OperationType id){
  if(id == INTEGER || id == DOUBLE)
    return  NUMBER;
  else{
  SymbolTableStruct instruction = operatorPrecedenceTable[id];  
  return  instruction.arity;
  }
}

createInfixArithmeticFunction(infixAdd, +);
createInfixArithmeticFunction(infixMinus, -);
createInfixArithmeticFunction(infixMultiply, *);
createArithmeticDivFunction(infixDivide, /);

createPrefixArithmeticFunction(prefixPlus, +);
createPrefixArithmeticFunction(prefixMinus, -);

createInfixLogicFunction(infixModulus, %);
createInfixLogicFunction(infixBitwiseAnd, &);
createPrefixLogicFunction(prefixLogicNot, !);
createPrefixLogicFunction(prefixInc, ++);
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
}