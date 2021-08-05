#include "ExpressionParser.h"

OperatorTableStruct  operatorPrecedenceTable[] = {
  [OPEN_PAREN]        = {1, NULL, NULL},  
  [CLOSE_PAREN]       = {1, NULL, NULL},  
  [OPEN_SQ_BRACKET]   = {1, NULL, NULL},  
  [CLOSE_SQ_BRACKET]  = {1, NULL, NULL},  
  [BITWISE_NOT]       = {2, prefixBitwiseNot, handlePrefix},  
  [LOGICAL_NOT]       = {2, NULL, handlePrefix},  
  [MULTIPLY]          = {3, infixMultiply, handleInfix},  
  [DIVIDE]            = {3, infixDivide, handleInfix},  
  [REMAINDER]         = {3, NULL, NULL},  
  [ADD]               = {4, infixAdd, handleInfix},  
  [MINUS]             = {4, infixMinus, handleInfix},  
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