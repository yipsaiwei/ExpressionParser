#include "ExpressionParser.h"
#include  <math.h>
#include  <stdlib.h>

//SymbolTableStruct   symbolTable
SymbolTableStruct  symbolTable[] = {
  [OPEN_PAREN]        = {10000, PREFIX, NULL,             forcePush,                        "(\0"},  
  [CLOSE_PAREN]       = {1000 , SUFFIX, NULL,             evaluateExpressionWithinBrackets, ")\0"},  //Call unwind until '('  
  [OPEN_SQ_BRACKET]   = {1000 , PREFIX, NULL,             forcePush,                        "[\0"},  
  [CLOSE_SQ_BRACKET]  = {1000 , SUFFIX, NULL,             evaluateExpressionWithinBrackets, "]\0"}, 
  [POST_INC]          = {1000 , SUFFIX, suffixInc,        handlePrefixSuffixInc,            "++\0"},   
  [POST_DEC]          = {1000 , SUFFIX, suffixDec,        handlePrefixSuffixDec,            "--\0"},   
  [INC]               = {2000 , PREFIX, prefixInc,        handlePrefixSuffixInc,            "++\0"},       //Right to left
  [DEC]               = {2000 , PREFIX, prefixDec,        handlePrefixSuffixDec,            "--\0"},       //Right to left
  [BITWISE_NOT]       = {2000 , PREFIX, prefixBitwiseNot, pushAccordingToPrecedence,        "~\0"},   //Right to left  (Change second func so that it will not take double
  [LOGICAL_NOT]       = {2000 , PREFIX, prefixLogicNot,   pushAccordingToPrecedence,        "!\0"},   //Right to left
  [PLUS_SIGN]         = {2000 , PREFIX, prefixPlus,       pushAccordingToPrecedence,        "+\0"},   //Right to left
  [MINUS_SIGN]        = {2000 , PREFIX, prefixMinus,      pushAccordingToPrecedence,        "-\0"},   //Right to left
  [MULTIPLY]          = {3000 , INFIX , infixMultiply,    pushAccordingToPrecedence,        "*\0"},  
  [DIVIDE]            = {3000 , INFIX , infixDivide,      pushAccordingToPrecedence,        "/\0"},  
  [REMAINDER]         = {3000 , INFIX , infixModulus,     pushAccordingToPrecedence,        "%\0"},  
  [ADD]               = {4000 , INFIX , infixAdd,         handleAddOrSub,                   "+\0"},  
  [MINUS]             = {4000 , INFIX , infixMinus,       handleAddOrSub,                   "-\0"},    
  [SHIFT_LEFT]        = {5000 , INFIX , infixShiftLeft,   pushAccordingToPrecedence,        "<<\0"}, 
  [SHIFT_RIGHT]       = {5000 , INFIX , infixShiftRight,  pushAccordingToPrecedence,        ">>\0"}, 
  [LESSER]            = {6000 , INFIX , infixLesser,      pushAccordingToPrecedence,        "<\0"}, 
  [LESSER_EQ]         = {6000 , INFIX , infixLesserEq,    pushAccordingToPrecedence,        "<=\0"}, 
  [GREATER]           = {6000 , INFIX , infixGreater,     pushAccordingToPrecedence,        ">\0"}, 
  [GREATER_EQ]        = {6000 , INFIX , infixGreaterEq,   pushAccordingToPrecedence,        ">=\0"}, 
  [EQUAL]             = {7000 , INFIX , infixEqual,       pushAccordingToPrecedence,        "==\0"}, 
  [BITWISE_AND]       = {8000 , INFIX , infixBitwiseAnd,  pushAccordingToPrecedence,        "&\0"},
  [BITWISE_OR]        = {8000 , INFIX , infixBitwiseOr,   pushAccordingToPrecedence,        "|\0"},
  [LOGICAL_AND]       = {12000, INFIX , infixLogicalAnd,  pushAccordingToPrecedence,        "&&\0"},
  [LOGICAL_OR]        = {12000, INFIX , infixLogicalOr,   pushAccordingToPrecedence,        "||\0"},
  [INTEGER]           = {0,     NUMBER, NULL,             handleNumber,                     NULL},
  [DOUBLE]            = {0,     NUMBER, NULL,             handleNumber,                     NULL},
  [_NULL]             = {0,     NONE,   NULL,             NULL,                             NULL},
};

extern  ExceptionTable  exceptionTable[];

ArityHandler  arityHandler[] = {
  [PREFIX] = handlePrefix,
  [INFIX]  = handleInfix,
  [SUFFIX] = handleSuffix,
  [NUMBER] = NULL,
};

void  shuntingYard(Tokenizer  *tokenizer, StackStruct *operatorStack, StackStruct *operandStack){
  //SymbolTableStruct instruction;
  Symbolizer  *symbolizer = createSymbolizer(tokenizer);
  Symbol  *symbol = getSymbol(symbolizer);
  ListItem  *popItem;
  while(!(isSymbolNull(symbol))){
    executeStoreHandler(operandStack, operatorStack, symbol, symbolizer);
    symbol = symbolizerUpdateLastSymbolAndGetNewSymbol(symbolizer, symbol);
  }
  while(operatorStack->size > 0)
    unwindStack(operatorStack, operandStack);
  freeSymbolizer(symbolizer);
}

void  executeStoreHandler(StackStruct  *operandStack, StackStruct  *operatorStack, Symbol  *symbol, Symbolizer *symbolizer){
  SymbolTableStruct instruction = symbolTable[symbol->id];
  instruction.storeHandler(operandStack, operatorStack, symbol, symbolizer);  
}

Symbol  *symbolizerUpdateLastSymbolAndGetNewSymbol(Symbolizer  *symbolizer, Symbol *symbol){
  symbolizerUpdateLastSymbol(symbol, symbolizer); 
  symbol = getSymbol(symbolizer);
  symbol->arity = returnArityOfAnId(symbol->id); 
  return  symbol;
}

void  symbolizerUpdateLastSymbol(Symbol *symbol, Symbolizer *symbolizer){
  if(symbol == NULL)
    symbolizer->lastSymbol = NULL;
  else{
    if(symbolizer->lastSymbol)
      freeSymbol(symbolizer->lastSymbol);
    symbolizer->lastSymbol = cloneSymbol(symbol);
  }
  //symbolizer->lastSymbolArity = symbol->arity;  
}

void  pushSymbolToStack(StackStruct *operatorStack, StackStruct *operandStack, Symbol *symbol){
  if(isSymbolOperatorType(symbol))
    pushOperator(operandStack, operatorStack, symbol);
  else                                   
    pushToStack(operandStack, (void  *)symbol);  
}

//typedef void    (*preHandleOperator)(StackStruct *operandStack, StackStruct *operatorStack, Symbol  *operator);
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

void  handlePrefixSuffixInc(StackStruct *operandStack, StackStruct *operatorStack, Symbol *symbol, Symbolizer  *symbolizer){
  if(!arityAllowable(symbolizer->lastSymbol->id, symbol->id)){
    if(!arityAllowable(symbolizer->lastSymbol->id, POST_INC))
      verifyArityAllowable(symbolizer, symbol, symbol->id);
    symbol->id = POST_INC; 
  }
  pushAccordingToPrecedence(operandStack, operatorStack, symbol, symbolizer);
}

void  handlePrefixSuffixDec(StackStruct *operandStack, StackStruct *operatorStack, Symbol *symbol, Symbolizer  *symbolizer){
  if(!arityAllowable(symbolizer->lastSymbol->id, symbol->id)){
    if(!arityAllowable(symbolizer->lastSymbol->id, POST_DEC))
      verifyArityAllowable(symbolizer, symbol, symbol->id);
    symbol->id = POST_DEC; 
  }
  pushAccordingToPrecedence(operandStack, operatorStack, symbol, symbolizer);
}

void  handleNumber(StackStruct *operandStack, StackStruct *operatorStack, Symbol *symbol, Symbolizer  *symbolizer){
  verifyArityAllowable(symbolizer, symbol, symbol->id);
  ListItem  *peekItem = peekTopOfStack(operatorStack);
  forcePush(operandStack, operatorStack, symbol, symbolizer);
  //if(isNotPreviousSymbolId(symbolizer, OPEN_PAREN) && isPreviousArity(symbolizer, PREFIX))
  //  unwindStackForAnArityInSequence(operandStack, operatorStack, PREFIX);
}

void  pushAccordingToPrecedence(StackStruct *operandStack, StackStruct *operatorStack, Symbol  *symbol, Symbolizer  *symbolizer){
  verifyArityAllowable(symbolizer, symbol, symbol->id);
  ListItem  *peekItem = peekTopOfStack(operatorStack);
  if(isIdArity(symbol->id, PREFIX) && (!isStackEmpty(operatorStack) && areTwoIdPrecedenceSame(getItemSymbolId(peekItem), symbol->id)))
    handleRightToLeftAssociativity(operandStack, operatorStack, symbol, symbolizer);
  else
    pushOperator(operandStack, operatorStack, symbol);
}

void  handleRightToLeftAssociativity(StackStruct *operandStack, StackStruct *operatorStack, Symbol  *symbol, Symbolizer *symbolizer){
  ListItem  *peekItem = peekTopOfStack(operatorStack);
  if(isIdArity(symbol->id, PREFIX) && areTwoIdPrecedenceSame(getItemSymbolId(peekItem), symbol->id)){
    pushToStack(operatorStack, symbol);
    //symbol = symbolizerUpdateLastSymbolAndGetNewSymbol(symbolizer, symbol);
    //peekItem = peekTopOfStack(operatorStack);
  }
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
  if(getItemSymbolId(popItem) != type){
    throwException(UNEXPECTED_OPERATOR, NULL, 0, "ERROR code %x : Not the desired operator!", UNEXPECTED_OPERATOR);
    return  NULL;
  }
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


void  forcePush(StackStruct *operandStack, StackStruct *operatorStack, Symbol *symbol, Symbolizer  *symbolizer){
  if(symbol->type == OPERATOR)
    pushToStack(operatorStack, (void  *)symbol);
  else
    pushToStack(operandStack, (void  *)symbol);
}

void  evaluateExpressionWithinBrackets(StackStruct *operandStack, StackStruct *operatorStack, Symbol *symbol, Symbolizer  *symbolizer){
  unwindStackUntil(operandStack, operatorStack, OPEN_PAREN);
  ListItem  *operatorItem = popOperator(operatorStack, OPEN_PAREN);
  freeListItemWithSymbol(operatorItem); 
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
        //handleException(symbol, symbolizer, ERROR_INVALID_INFIX);
      symbolThrowInfixException(symbol, ERROR_INVALID_INFIX, symbolizer);
      break;
    case  PREFIX:
      if(isIdArity(symbolizer->lastSymbol->id, NONE) || isIdArity(symbolizer->lastSymbol->id, INFIX) || isIdArity(symbolizer->lastSymbol->id, PREFIX))  //2+ ++3 OR -++3 OR -3
        return  1;
      else
        handleException(symbol, symbolizer, ERROR_INVALID_PREFIX);
      break;
    case  SUFFIX:
      if(isIdArity(symbolizer->lastSymbol->id, NUMBER))    //3++
        return  1;
      else
        throwException(UNEXPECTED_OPERATOR, NULL, 0, "ERROR code %x : Not the desired operator!", UNEXPECTED_OPERATOR);
     break;
    case  NUMBER:
      if(isIdArity(symbolizer->lastSymbol->id, NONE) || isIdArity(symbolizer->lastSymbol->id, PREFIX) || isIdArity(symbolizer->lastSymbol->id, INFIX))  //3 OR -3 OR 2+3
        return  1;
      else
        throwException(UNEXPECTED_OPERATOR, NULL, 0, "ERROR code %x : Not the desired operator!", UNEXPECTED_OPERATOR);
    break;
    default:
      throwException(UNEXPECTED_OPERATOR, NULL, 0, "ERROR code %x : Not the desired operator!", UNEXPECTED_OPERATOR);
  }
  return  0;
}

void  handleException(Symbol  *symbol, Symbolizer *symbolizer, int  errorCode){
  char  *previousStr = returnSymbolCharGivenId(symbolizer->lastSymbol);
  char  *currentStr = returnSymbolCharGivenId(symbol);
  ExceptionTable  exceptionInfo = exceptionTable[errorCode];
  freeSymbolizer(symbolizer);
  exceptionInfo.exceptionPtr(symbol, errorCode, previousStr, currentStr);
}

char  *returnSymbolCharGivenId(Symbol *symbol){
  SymbolTableStruct instruction = symbolTable[symbol->id];
  return  instruction.idChar;
}

int returnOperatorPrecedence(OperationType  type){
  SymbolTableStruct instruction = symbolTable[type];
  return  instruction.precedence;
}

void  freeListItemWithSymbol(ListItem *item){
  if(getItemSymbol(item))
    freeSymbol(getItemSymbol(item));
  if(item)
    linkedListFreeListItem(item);
}

void  handleInfix(StackStruct *operandStack, StackStruct *operatorStack){
  ListItem *operand2 = popFromStack(operandStack);
  ListItem *operand1 = popFromStack(operandStack);
  ListItem *operator = popFromStack(operatorStack);
  Symbol  *result;
  SymbolTableStruct instruction = symbolTable[getItemSymbolId(operator)];
  result = instruction.arithmeticHandler(getItemSymbol(operand1), getItemSymbol(operand2));
  pushToStack(operandStack, (void *)result);
  freeListItemWithSymbol(operand1);                         
  freeListItemWithSymbol(operand2); 
  freeListItemWithSymbol(operator);
}

void  handlePrefix(StackStruct *operandStack, StackStruct *operatorStack){
  ListItem *operand = popFromStack(operandStack);
  ListItem *operator = popFromStack(operatorStack);
  Symbol  *result;
  SymbolTableStruct instruction = symbolTable[getItemSymbolId(operator)];
  result = instruction.arithmeticHandler(getItemSymbol(operand), NULL);
  pushToStack(operandStack, (void *)result);
  freeListItemWithSymbol(operand); 
  freeListItemWithSymbol(operator);  
}

void  handleSuffix(StackStruct *operandStack, StackStruct *operatorStack){
  ListItem *operand = popFromStack(operandStack);
  ListItem *operator = popFromStack(operatorStack);
  Symbol  *result;
  SymbolTableStruct instruction = symbolTable[getItemSymbolId(operator)];
  result = instruction.arithmeticHandler(getItemSymbol(operand), NULL);
  pushToStack(operandStack, (void *)result);
  freeListItemWithSymbol(operand); 
  freeListItemWithSymbol(operator);  
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
createPrefixLogicFunction(prefixLogicNot, !);
//createPrefixLogicFunction(prefixInc, ++);
createPrefixArithmeticFunction(prefixInc, ++);
//createPrefixLogicFunction(prefixDec, --);
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
    resultStr = malloc(sizeof(char) * (countIntegerDigitNumber(intNumber) + 1));
    //itoa(intNumber, resultStr, 10);
    snprintf(resultStr, 10, "%d", intNumber);
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
  int i = countIntegerDigitNumber((int)n);
  snprintf(res, i + afterpoint + 2, "%f", n);
}