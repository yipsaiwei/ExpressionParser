#include "ExpressionParser.h"
#include  <math.h>
#include  <stdlib.h>

//SymbolTableStruct   symbolTable
SymbolTableStruct  symbolTable[] = {
  [OPEN_PAREN]        = {10000, PREFIX, NULL,             forcePush                        },  
  [CLOSE_PAREN]       = {1000 , SUFFIX, NULL,             evaluateExpressionWithinBrackets },  //Call unwind until '('  
  [OPEN_SQ_BRACKET]   = {1000 , PREFIX, NULL,             forcePush                        },  
  [CLOSE_SQ_BRACKET]  = {1000 , SUFFIX, NULL,             evaluateExpressionWithinBrackets }, 
  [POST_INC]          = {1000 , SUFFIX, suffixInc,        handlePrefixSuffixInc            },   
  [POST_DEC]          = {1000 , SUFFIX, suffixDec,        handlePrefixSuffixDec            },   
  [INC]               = {2000 , PREFIX, prefixInc,        handlePrefixSuffixInc            },       //Right to left
  [DEC]               = {2000 , PREFIX, prefixDec,        handlePrefixSuffixDec            },       //Right to left
  [BITWISE_NOT]       = {2000 , PREFIX, prefixBitwiseNot, pushAccordingToPrecedence        },   //Right to left  (Change second func so that it will not take double
  [LOGICAL_NOT]       = {2000 , PREFIX, prefixLogicNot,   pushAccordingToPrecedence        },   //Right to left
  [PLUS_SIGN]         = {2000 , PREFIX, prefixPlus,       pushAccordingToPrecedence        },   //Right to left
  [MINUS_SIGN]        = {2000 , PREFIX, prefixMinus,      pushAccordingToPrecedence        },   //Right to left
  [MULTIPLY]          = {3000 , INFIX , infixMultiply,    pushAccordingToPrecedence        },  
  [DIVIDE]            = {3000 , INFIX , infixDivide,      pushAccordingToPrecedence        },  
  [REMAINDER]         = {3000 , INFIX , infixModulus,     pushAccordingToPrecedence        },  
  [ADD]               = {4000 , INFIX , infixAdd,         handleAddOrSub                   },  
  [MINUS]             = {4000 , INFIX , infixMinus,       handleAddOrSub                   },    
  [SHIFT_LEFT]        = {5000 , INFIX , infixShiftLeft,   pushAccordingToPrecedence        }, 
  [SHIFT_RIGHT]       = {5000 , INFIX , infixShiftRight,  pushAccordingToPrecedence        }, 
  [LESSER]            = {6000 , INFIX , infixLesser,      pushAccordingToPrecedence        }, 
  [LESSER_EQ]         = {6000 , INFIX , infixLesserEq,    pushAccordingToPrecedence        }, 
  [GREATER]           = {6000 , INFIX , infixGreater,     pushAccordingToPrecedence        }, 
  [GREATER_EQ]        = {6000 , INFIX , infixGreaterEq,   pushAccordingToPrecedence        }, 
  [EQUAL]             = {7000 , INFIX , infixEqual,       pushAccordingToPrecedence        }, 
  [BITWISE_AND]       = {8000 , INFIX , infixBitwiseAnd,  pushAccordingToPrecedence        },
  [BITWISE_XOR]       = {9000 , INFIX , infixBitwiseXor,  pushAccordingToPrecedence        },
  [BITWISE_OR]        = {8000 , INFIX , infixBitwiseOr,   pushAccordingToPrecedence        },
  [LOGICAL_AND]       = {12000, INFIX , infixLogicalAnd,  pushAccordingToPrecedence        },
  [LOGICAL_OR]        = {12000, INFIX , infixLogicalOr,   pushAccordingToPrecedence        },
  [INTEGER]           = {0,     NUMBER, NULL,             handleNumber                     },
  [DOUBLE]            = {0,     NUMBER, NULL,             handleNumber                     },
  [_NULL]             = {0,     NONE,   NULL,             NULL                             },
};

extern  ExceptionTable  exceptionTable[];

ArityHandler  arityHandler[] = {
  [PREFIX] = handlePrefix,
  [INFIX]  = handleInfix,
  [SUFFIX] = handleSuffix,
  [NUMBER] = NULL,
};

void  shuntingYard(Tokenizer  *tokenizer, StackStruct *operatorStack, StackStruct *operandStack){
  Symbolizer  *symbolizer = createSymbolizer(tokenizer);
  Symbol  *symbol = getSymbol(symbolizer);
  ListItem  *popItem;
  while(!(isSymbolNull(symbol))){           //Get the symbol(until expression end) and store it into their stacks 
    executeStoreHandler(operandStack, operatorStack, symbol, symbolizer);
    symbol = symbolizerUpdateLastSymbolAndGetNewSymbol(symbolizer, symbol);
  }
  while(!isStackEmpty(operatorStack)){    //Unwind until end of stack, throw exception if the operator failed to be unwind
    if(!unwindStack(operatorStack, operandStack))
      checkAndThrowException(symbol, symbolizer, operatorStack);
  }
  freeSymbolizer(symbolizer);
}

//Access the store handler function for different types of operator through function pointer
void  executeStoreHandler(StackStruct  *operandStack, StackStruct  *operatorStack, Symbol  *symbol, Symbolizer *symbolizer){
  SymbolTableStruct instruction = symbolTable[symbol->id];
  instruction.storeHandler(operandStack, operatorStack, symbol, symbolizer);  
}

//Update last symbol pointed by symbolizer, return new symbol obtained
Symbol  *symbolizerUpdateLastSymbolAndGetNewSymbol(Symbolizer  *symbolizer, Symbol *symbol){
  symbolizerUpdateLastSymbol(symbol, symbolizer); 
  symbol = getSymbol(symbolizer);
  symbol->arity = returnArityOfAnId(symbol->id); 
  return  symbol;
}

//Update last symbol by clonning the last symbol and will be pointed by symbolizer
void  symbolizerUpdateLastSymbol(Symbol *symbol, Symbolizer *symbolizer){
  if(symbol == NULL)
    symbolizer->lastSymbol = NULL;
  else{
    if(symbolizer->lastSymbol)
      freeSymbol(symbolizer->lastSymbol);
    symbolizer->lastSymbol = cloneSymbol(symbol);
  }
}

//Check type of symbol, then push the symbol into their respective stack
void  pushSymbolToStack(StackStruct *operatorStack, StackStruct *operandStack, Symbol *symbol){
  if(isSymbolOperatorType(symbol))
    pushOperator(operandStack, operatorStack, symbol);
  else                                   
    pushToStack(operandStack, (void  *)symbol);  
}

//Detect whether the plus sign is infix or prefix or invalid based on the last symbol
void  handleAddOrSub(StackStruct *operandStack, StackStruct *operatorStack, Symbol *symbol, Symbolizer  *symbolizer){
  if(!arityAllowable(symbolizer->lastSymbol->id, symbol->id) || isPreviousSymbolId(symbolizer, _NULL)){
    verifyArityAllowable(symbolizer, symbol, PLUS_SIGN);
    if(symbol->id == ADD)
      symbol->id = PLUS_SIGN;
    else
      symbol->id = MINUS_SIGN;
  }
  pushAccordingToPrecedence(operandStack, operatorStack, symbol, symbolizer);
}

//Detect whether the increment is suffix or prefix or invalid based on the last symbol
void  handlePrefixSuffixInc(StackStruct *operandStack, StackStruct *operatorStack, Symbol *symbol, Symbolizer  *symbolizer){
  if(!arityAllowable(symbolizer->lastSymbol->id, symbol->id)){
    if(!arityAllowable(symbolizer->lastSymbol->id, POST_INC))
      verifyArityAllowable(symbolizer, symbol, symbol->id);
    symbol->id = POST_INC; 
  }
  pushAccordingToPrecedence(operandStack, operatorStack, symbol, symbolizer);
}

//Detect whether the decrement is suffix or prefix or invalid based on the last symbol
void  handlePrefixSuffixDec(StackStruct *operandStack, StackStruct *operatorStack, Symbol *symbol, Symbolizer  *symbolizer){
  if(!arityAllowable(symbolizer->lastSymbol->id, symbol->id)){
    if(!arityAllowable(symbolizer->lastSymbol->id, POST_DEC))
      verifyArityAllowable(symbolizer, symbol, symbol->id);
    symbol->id = POST_DEC; 
  }
  pushAccordingToPrecedence(operandStack, operatorStack, symbol, symbolizer);
}

//Check whether the number position is allowed based on the last symbol
void  handleNumber(StackStruct *operandStack, StackStruct *operatorStack, Symbol *symbol, Symbolizer  *symbolizer){
  verifyArityAllowable(symbolizer, symbol, symbol->id);
  ListItem  *peekItem = peekTopOfStack(operatorStack);
  forcePush(operandStack, operatorStack, symbol, symbolizer);
}

void  pushAccordingToPrecedence(StackStruct *operandStack, StackStruct *operatorStack, Symbol  *symbol, Symbolizer  *symbolizer){
  verifyArityAllowable(symbolizer, symbol, symbol->id);
  ListItem  *peekItem = peekTopOfStack(operatorStack);
  if(isIdArity(symbol->id, PREFIX) && (!isStackEmpty(operatorStack) && areTwoIdPrecedenceSame(getItemSymbolId(peekItem), symbol->id)))
    handleRightToLeftAssociativity(operandStack, operatorStack, symbol, symbolizer);
  else{
    if(!pushOperator(operandStack, operatorStack, symbol))
      checkAndThrowException(symbol, symbolizer, operatorStack);
  }
}

//Check exception error and throw it
void  checkAndThrowException(Symbol *symbol, Symbolizer *symbolizer, StackStruct  *operatorStack){
  ListItem  *peekItem = peekTopOfStack(operatorStack);
  if(getItemSymbolId(peekItem) == OPEN_PAREN){
    freeSymbol(symbol);
    peekItem = popFromStack(operatorStack);
    symbol = cloneSymbol(getItemSymbol(peekItem));
    freeListItemWithSymbol(peekItem);
    symbolThrowMissingParenException(symbol, ERROR_MISSING_CLOSING_PAREN, symbolizer);
  }
}

//For prefix, need to handle from right to left
void  handleRightToLeftAssociativity(StackStruct *operandStack, StackStruct *operatorStack, Symbol  *symbol, Symbolizer *symbolizer){
  ListItem  *peekItem = peekTopOfStack(operatorStack);
  if(isIdArity(symbol->id, PREFIX) && areTwoIdPrecedenceSame(getItemSymbolId(peekItem), symbol->id)){
    pushToStack(operatorStack, symbol);
  }
}

//Push operator, if there are lower precedence operator inside the operatorStack, unwind it
int pushOperator(StackStruct *operandStack, StackStruct *operatorStack, Symbol  *operatorToPush){
  ListItem *peekItem = peekTopOfStack(operatorStack);
  while(!isStackEmpty(operatorStack) && comparePrecedence(operatorToPush, getItemSymbol(peekItem)) < 1){
    if(!unwindStack(operatorStack, operandStack))
      return  0;
    peekItem = peekTopOfStack(operatorStack);
  }
  pushToStack(operatorStack, (void  *)operatorToPush);
  return  1;
}

ListItem  *popOperator(StackStruct *operatorStack, OperationType type){
  ListItem  *popItem = popFromStack(operatorStack);
  if(getItemSymbolId(popItem) != type){
    return  NULL;
  }
  else
    return  popItem;
}


//take the operator and operate on the operands
//3 different scenarios " infix, prefix, suffix
//Precedence checking in shuntingYard
//Push operator to the stack
int  unwindStack(StackStruct *operatorStack, StackStruct *operandStack){
  return  operateOperatorInOperatorStack(operandStack, operatorStack);
}

int  operateOperatorInOperatorStack(StackStruct  *operandStack, StackStruct  *operatorStack){
  ListItem  *popItem = peekTopOfStack(operatorStack);
  ArityHandler  arityFuncPtr = arityHandler[returnArityOfAnId(getItemSymbolId(popItem))];
  if(arityFuncPtr.arityHandler(operandStack, operatorStack))  
    return  1;
  else
    return  0;
}


void  forcePush(StackStruct *operandStack, StackStruct *operatorStack, Symbol *symbol, Symbolizer  *symbolizer){
  if(symbol->type == OPERATOR)
    pushToStack(operatorStack, (void  *)symbol);
  else
    pushToStack(operandStack, (void  *)symbol);
}

void  evaluateExpressionWithinBrackets(StackStruct *operandStack, StackStruct *operatorStack, Symbol *symbol, Symbolizer  *symbolizer){
  if(!arityAllowable(symbolizer->lastSymbol->id, symbol->id)){
   if(isNotPreviousSymbolId(symbolizer, CLOSE_PAREN) && isNotPreviousArity(symbolizer, SUFFIX))
    verifyArityAllowable(symbolizer, symbol, symbol->id);
  }
  if(!unwindStackUntil(operandStack, operatorStack, OPEN_PAREN))
    symbolThrowMissingParenException(symbol, ERROR_MISSING_OPEN_PAREN, symbolizer);
  ListItem  *operatorItem = popOperator(operatorStack, OPEN_PAREN);
  if(operatorItem == NULL)
    symbolThrowMissingParenException(symbol, ERROR_MISSING_OPEN_PAREN, symbolizer);
  freeListItemWithSymbol(operatorItem); 
}

int  unwindStackUntil(StackStruct *operandStack, StackStruct *operatorStack, OperationType type){
  ListItem  *peekItem = peekTopOfStack(operatorStack);
  while(!isStackEmpty(operatorStack) && getItemSymbolId(peekItem) != type){
    unwindStack(operatorStack, operandStack);
    peekItem = peekTopOfStack(operatorStack);
  }
  if(isStackEmpty(operatorStack))
    return  0;
  else
    return  1;
}

void  unwindStackForAnArityInSequence(StackStruct *operandStack, StackStruct *operatorStack, ARITY  arity){
  ListItem  *peekItem = peekTopOfStack(operatorStack);
  while(!isStackEmpty(operatorStack) && returnArityOfAnId(getItemSymbolId(peekItem)) == arity && getItemSymbolId(peekItem) != OPEN_PAREN){
    unwindStack(operatorStack, operandStack);
    peekItem = peekTopOfStack(operatorStack);
  }
}

//check the index of the top of both stacks to determine which one comes just before this operator to check. 
//if no operator/number before = prefix
//if previous is number = infix / suffix
//if previous is an operator :
    //if previous type is infix = prefix
    //if previous type is prefix = prefix
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

int verifyArityAllowable(Symbolizer  *symbolizer, Symbol *symbol, OperationType typeToCheck){
  switch(returnArityOfAnId(typeToCheck)){
    case  INFIX:
      if(isIdArity(symbolizer->lastSymbol->id, SUFFIX || isIdArity(symbolizer->lastSymbol->id, NUMBER)))    //3++ +3 OR 2+3
        return  1;
      else
      symbolThrowInfixException(symbol, ERROR_INVALID_INFIX, symbolizer);
      break;
    case  PREFIX:
      if(isIdArity(symbolizer->lastSymbol->id, NONE) || isIdArity(symbolizer->lastSymbol->id, INFIX) || isIdArity(symbolizer->lastSymbol->id, PREFIX))  //2+ ++3 OR -++3 OR -3
        return  1;
      else
        symbolThrowPrefixException(symbol, ERROR_INVALID_PREFIX, symbolizer);
      break;
    case  SUFFIX:
      if(isIdArity(symbolizer->lastSymbol->id, NUMBER))    //3++
        return  1;
      else
        symbolThrowSuffixException(symbol, ERROR_INVALID_SUFFIX, symbolizer);
     break;
    case  NUMBER:
      if(isIdArity(symbolizer->lastSymbol->id, NONE) || isIdArity(symbolizer->lastSymbol->id, PREFIX) || isIdArity(symbolizer->lastSymbol->id, INFIX))  //3 OR -3 OR 2+3
        return  1;
      else
        symbolThrowNumberException(symbol, ERROR_INVALID_NUMBER, symbolizer);
    break;
    default:
      throwException(UNEXPECTED_OPERATOR, NULL, 0, "ERROR code %x : Not the desired operator!", UNEXPECTED_OPERATOR);
  }
  return  0;
}

int returnOperatorPrecedence(OperationType  type){
  SymbolTableStruct instruction = symbolTable[type];
  return  instruction.precedence;
}

int  handleInfix(StackStruct *operandStack, StackStruct *operatorStack){
  ListItem *operand2 = popFromStack(operandStack);
  ListItem *operand1 = popFromStack(operandStack);
  ListItem *operator = popFromStack(operatorStack);
  Symbol  *result;
  SymbolTableStruct instruction = symbolTable[getItemSymbolId(operator)];
  if(instruction.arithmeticHandler == NULL)
    return  0;
  result = instruction.arithmeticHandler(getItemSymbol(operand1), getItemSymbol(operand2));
  pushToStack(operandStack, (void *)result);
  freeListItemWithSymbol(operand1);                         
  freeListItemWithSymbol(operand2); 
  freeListItemWithSymbol(operator);
  return  1;
}

int  handlePrefix(StackStruct *operandStack, StackStruct *operatorStack){
  ListItem *operator = peekTopOfStack(operatorStack);
  Symbol  *result;
  SymbolTableStruct instruction = symbolTable[getItemSymbolId(operator)];
  if(instruction.arithmeticHandler == NULL)
    return  0;
  ListItem *operand = popFromStack(operandStack);
  operator = popFromStack(operatorStack);
  result = instruction.arithmeticHandler(getItemSymbol(operand), NULL);
  pushToStack(operandStack, (void *)result);
  freeListItemWithSymbol(operand); 
  freeListItemWithSymbol(operator);  
  return  1;
}

int  handleSuffix(StackStruct *operandStack, StackStruct *operatorStack){
  ListItem *operand = popFromStack(operandStack);
  ListItem *operator = popFromStack(operatorStack);
  Symbol  *result;
  SymbolTableStruct instruction = symbolTable[getItemSymbolId(operator)];
  if(instruction.arithmeticHandler == NULL)
    return  0;
  result = instruction.arithmeticHandler(getItemSymbol(operand), NULL);
  pushToStack(operandStack, (void *)result);
  freeListItemWithSymbol(operand); 
  freeListItemWithSymbol(operator);  
  return  1;
}
ARITY returnArityOfAnId(OperationType id){
  if(id == INTEGER || id == DOUBLE)
    return  NUMBER;
  else{
  SymbolTableStruct instruction = symbolTable[id];  
  return  instruction.arity;
  }
}

createInfixArithmeticFunction(infixAdd, +);
createInfixArithmeticFunction(infixMinus, -);
createInfixArithmeticFunction(infixMultiply, *);
createArithmeticDivFunction(infixDivide, /);

createPrefixArithmeticFunction(prefixPlus, +);
createPrefixArithmeticFunction(prefixMinus, -);

createInfixLogicFunctionWithDouble(infixEqual, ==);
createInfixLogicFunctionWithDouble(infixLesser, <);
createInfixLogicFunctionWithDouble(infixLesserEq, <=);
createInfixLogicFunctionWithDouble(infixGreater, >);
createInfixLogicFunctionWithDouble(infixGreaterEq, >=);
createInfixLogicFunction(infixModulus, %);
createInfixLogicFunction(infixShiftLeft, <<);
createInfixLogicFunction(infixShiftRight, >>);
createInfixLogicFunction(infixBitwiseAnd, &);
createInfixLogicFunction(infixLogicalAnd, &&);
createInfixLogicFunction(infixBitwiseOr, |);
createInfixLogicFunction(infixLogicalOr, ||);
createInfixLogicFunction(infixBitwiseXor, ^);
createPrefixLogicFunction(prefixLogicNot, !);
createPrefixArithmeticFunction(prefixInc, ++);
createPrefixArithmeticFunction(prefixDec, --);
createPrefixLogicFunction(prefixBitwiseNot, ~);

createSuffixIncDecFunction(suffixDec, --);
createSuffixIncDecFunction(suffixInc, ++);

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
    resultStr = memAlloc(sizeof(char) * (countIntegerDigitNumber(intNumber) + 1));
    //itoa(intNumber, resultStr, 10);
    snprintf(resultStr, 10, "%d", intNumber);
    return  resultStr;
  }else{
    double  doubleNumber = *((double  *)result);
    resultStr = memAlloc(sizeof(char) * (countDoubleDigitNumber(doubleNumber, 6) + 1));
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
  int i = countIntegerDigitNumber((int)n);
  snprintf(res, i + afterpoint + 2, "%f", n);
}