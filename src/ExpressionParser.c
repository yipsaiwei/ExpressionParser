#include "ExpressionParser.h"

//SymbolTableStruct   symbolTable
SymbolTableStruct  operatorPrecedenceTable[] = {
  [OPEN_PAREN]        = {10000, PREFIX, NULL,             forcePush},  
  [CLOSE_PAREN]       = {1000 , SUFFIX, NULL,             evaluateExpressionWithinBrackets},  //Call unwind until '('  
  [OPEN_SQ_BRACKET]   = {1000 , PREFIX, NULL,             forcePush},  
  [CLOSE_SQ_BRACKET]  = {1000 , SUFFIX, NULL,             evaluateExpressionWithinBrackets}, 
  [INC]               = {1000 , PREFIX, prefixInc,        handlePreIncOrPostInc},  
  [POST_INC]          = {1000 , SUFFIX, suffixInc,        handlePreIncOrPostInc},   
  [BITWISE_NOT]       = {2000 , PREFIX, prefixBitwiseNot, pushAccordingToPrecedence},  
  [LOGICAL_NOT]       = {2000 , PREFIX, prefixLogicNot,   pushAccordingToPrecedence},  
  [PLUS_SIGN]         = {2000 , PREFIX, prefixPlus,       pushAccordingToPrecedence}, 
  [MINUS_SIGN]        = {2000 , PREFIX, prefixMinus,      pushAccordingToPrecedence}, 
  [MULTIPLY]          = {3000 , INFIX , infixMultiply,    pushAccordingToPrecedence},  
  [DIVIDE]            = {3000 , INFIX , infixDivide,      pushAccordingToPrecedence},  
  [REMAINDER]         = {3000 , INFIX , infixModulus,     pushAccordingToPrecedence},  
  [ADD]               = {4000 , INFIX , infixAdd,         handleAddOrSub},  
  [MINUS]             = {4000 , INFIX , infixMinus,       handleAddOrSub},    
  [SHIFT_LEFT]        = {5000 , INFIX , infixShiftLeft,   pushAccordingToPrecedence}, 
  [LESSER]            = {6000 , INFIX , infixLesser,      pushAccordingToPrecedence}, 
  [LESSER_EQ]         = {6000 , INFIX , infixLesserEq,    pushAccordingToPrecedence}, 
  [GREATER]           = {6000 , INFIX , infixGreater,     pushAccordingToPrecedence}, 
  [GREATER_EQ]        = {6000 , INFIX , infixGreaterEq,   pushAccordingToPrecedence}, 
  [EQUAL]             = {7000 , INFIX , infixEqual,       pushAccordingToPrecedence}, 
  [BITWISE_AND]       = {8000 , INFIX , infixBitwiseAnd,  pushAccordingToPrecedence},
  [BITWISE_OR]        = {8000 , INFIX , infixBitwiseOr,   pushAccordingToPrecedence},
  [LOGICAL_AND]       = {12000, INFIX , infixLogicalAnd,  pushAccordingToPrecedence},
  [LOGICAL_OR]        = {12000, INFIX , infixLogicalOr,   pushAccordingToPrecedence},
  [INTEGER]           = {0,     NUMBER, NULL,             forcePush},
  [DOUBLE]            = {0,     NUMBER, NULL,             forcePush},
};

ArityHandler  arityHandler[] = {
  [PREFIX] = handlePrefix,
  [INFIX]  = handleInfix,
  [SUFFIX] = handleSuffix,
  [NUMBER] = NULL,
};

void  shuntingYard(Tokenizer  *tokenizer, StackStruct *operatorStack, StackStruct *operandStack){
  SymbolTableStruct instruction;
  Symbolizer  *symbolizer = createSymbolizer(tokenizer);
  Symbol  *symbol = symbolizerUpdateLastSymbolAndGetNewSymbol(symbolizer, NULL);
  ListItem  *popItem;
  while(!(isSymbolNull(symbol))){
    instruction = operatorPrecedenceTable[symbol->id];
    instruction.storeHandler(operandStack, operatorStack, symbol, symbolizer->lastSymbolId);
    symbol = symbolizerUpdateLastSymbolAndGetNewSymbol(symbolizer, symbol);
  }
  while(operatorStack->size > 0)
    unwindStack(operatorStack, operandStack);
  freeSymbolizer(symbolizer);
}

Symbol  *symbolizerUpdateLastSymbolAndGetNewSymbol(Symbolizer  *symbolizer, Symbol *symbol){
  symbolizerUpdateLastSymbol(symbol, symbolizer); 
  symbol = getSymbol(symbolizer);
  symbol->arity = returnArityOfAnId(symbol->id); 
  return  symbol;
}

void  symbolizerUpdateLastSymbol(Symbol *symbol, Symbolizer *symbolizer){
  if(symbol == NULL)
    symbolizer->lastSymbolId = _NULL;
  else
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
void  handleAddOrSub(StackStruct *operandStack, StackStruct *operatorStack, Symbol *symbol, OperationType previousId){
  if(!arityAllowable(previousId, symbol->id) || previousId == _NULL){
    verifyArityAllowable(previousId, PLUS_SIGN);
    if(symbol->id == ADD)
      symbol->id = PLUS_SIGN;
    else
      symbol->id = MINUS_SIGN;
  }
  pushAccordingToPrecedence(operandStack, operatorStack, symbol, previousId);
}

void  handlePreIncOrPostInc(StackStruct *operandStack, StackStruct *operatorStack, Symbol *symbol, OperationType previousId){
  if(!arityAllowable(previousId, symbol->id)){
    verifyArityAllowable(previousId, SUFFIX);
    if(symbol->id == INC)
      symbol->id = POST_INC;    
  }
  pushAccordingToPrecedence(operandStack, operatorStack, symbol, previousId);
}

void  pushAccordingToPrecedence(StackStruct *operandStack, StackStruct *operatorStack, Symbol  *symbol, OperationType previousId){
  verifyArityAllowable(previousId, symbol->id);
  pushOperator(operandStack, operatorStack, symbol);
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
  ArityHandler  arityFuncPtr = arityHandler[returnArityOfAnId(getItemSymbolId(popItem))];
  arityFuncPtr.arityHandler(operandStack, operatorStack);  
}


void  forcePush(StackStruct *operandStack, StackStruct *operatorStack, Symbol *symbol, OperationType  previousType){
  if(symbol->type == OPERATOR)
    pushToStack(operatorStack, (void  *)symbol);
  else
    pushToStack(operandStack, (void  *)symbol);
}

void  evaluateExpressionWithinBrackets(StackStruct *operandStack, StackStruct *operatorStack, Symbol *symbol, OperationType  previousType){
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
      if(isIdArity(previousType, SUFFIX) || isIdArity(previousType, NUMBER))    //3++ +3 OR 2+3
        return  1;
      else
        throwException(UNEXPECTED_OPERATOR, NULL, 0, "ERROR code %x : Not the desired operator!", UNEXPECTED_OPERATOR);
      break;
    case  PREFIX:
      if(isIdArity(previousType, INFIX) || isIdArity(previousType, PREFIX) || isIdArity(previousType, NONE))  //2+ ++3 OR -++3 OR -3
        return  1;
      else
        throwException(UNEXPECTED_OPERATOR, NULL, 0, "ERROR code %x : Not the desired operator!", UNEXPECTED_OPERATOR);
      break;
    case  SUFFIX:
      if(isIdArity(previousType, NUMBER))    //3++
        return  1;
      else
        throwException(UNEXPECTED_OPERATOR, NULL, 0, "ERROR code %x : Not the desired operator!", UNEXPECTED_OPERATOR);
     break;
    case  NUMBER:
      if(isIdArity(previousType, NONE) || isIdArity(previousType, PREFIX) || isIdArity(previousType, INFIX))  //3 OR -3 OR 2+3
        return  1;
      else
        throwException(UNEXPECTED_OPERATOR, NULL, 0, "ERROR code %x : Not the desired operator!", UNEXPECTED_OPERATOR);
    break;
    default:
      throwException(UNEXPECTED_OPERATOR, NULL, 0, "ERROR code %x : Not the desired operator!", UNEXPECTED_OPERATOR);
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

void  handleSuffix(StackStruct *operandStack, StackStruct *operatorStack){
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
createInfixArithmeticFunction(infixEqual, ==);
createArithmeticDivFunction(infixDivide, /);

createPrefixArithmeticFunction(prefixPlus, +);
createPrefixArithmeticFunction(prefixMinus, -);

//createSuffixArithmeticFunction(suffixInc, ++);

createInfixLogicFunction(infixModulus, %);
createInfixLogicFunction(infixLesser, <);
createInfixLogicFunction(infixLesserEq, <=);
createInfixLogicFunction(infixGreater, >);
createInfixLogicFunction(infixGreaterEq, >=);
createInfixLogicFunction(infixShiftLeft, <<);
createInfixLogicFunction(infixShiftRight, >>);
createInfixLogicFunction(infixBitwiseAnd, &);
createInfixLogicFunction(infixLogicalAnd, &&);
createInfixLogicFunction(infixBitwiseOr, |);
createInfixLogicFunction(infixLogicalOr, ||);
createPrefixLogicFunction(prefixLogicNot, !);
createPrefixLogicFunction(prefixInc, ++);
createPrefixLogicFunction(prefixBitwiseNot, ~);

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

Symbol  *suffixInc(Symbol  *number1, Symbol  *number2){
  Symbol  *result;
  Token *resultToken;
  char  *resultStr;
  if(isSymbolInteger(number1)){
    int resultNum = getSymbolInteger(number1);
    resultNum++;
    resultStr = createResultString((void  *)&resultNum, INTEGER);
    resultToken = (Token  *)createIntToken(resultNum, number1->token->startColumn, number1->token->originalstr, resultStr, TOKEN_INTEGER_TYPE);
    result = createSymbol(resultToken,  OPERAND, INTEGER);
  }else{
    double  resultNum = getSymbolDouble(number1);
    resultNum++;
    resultStr = createResultString((void  *)&resultNum, DOUBLE);
    resultToken = (Token  *)createFloatToken(resultNum, number2->token->startColumn, number2->token->originalstr, resultStr, TOKEN_FLOAT_TYPE);
    result = createSymbol(resultToken, OPERAND, DOUBLE);
  } 
  return  result;    
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