#ifndef ARITHMETIC_H
#define ARITHMETIC_H

//#include  "ExpressionParser.h"

#define createArithmeticFunction(funcName, operator)                        \
Number  *funcName(Number  *number1, Number  *number2){                      \
  Number  *result;                                                          \
  infixArithmeticCalculation(number1, number2, operator);                   \
  return  result;                                                           \
}

#define createArithmeticDivFunction(funcName, operator)                     \
Number  *funcName(Number  *number1, Number  *number2){                      \
  Number  *result;                                                          \
  infixArithmeticDivCalculation(number1, number2, operator);                \
  return  result;                                                           \
}

#define createPrefixLogicFunction(funcName, operator)                       \
Number  *funcName(Number  *number, Number *number2){                        \
  Number  *result;                                                          \
  prefixLogicCalculation(number, operator);                                 \
  return  result;                                                           \
}

#define createInfixLogicFunction(funcName, operator)                        \
Number  *funcName(Number  *number1, Number *number2){                       \
  Number  *result;                                                          \
  infixLogicCalculation(number1, number2, operator)                         \
  return  result;                                                           \
}

#define infixArithmeticCalculation(operand1, operand2, operator)                                                    \
  if(isOperandType(operand1, INTEGER_NUMBER) && isOperandType(operand2, INTEGER_NUMBER)){                           \
    result = (Number  *)createInteger(getNumberInteger(operand1) operator getNumberInteger(operand2));              \
    result->type = INTEGER_NUMBER;                                                                                  \
  }else                                                                                                             \
  {                                                                                                                 \
    if(isOperandType(operand1, FLOAT_NUMBER) && isOperandType(operand2, INTEGER_NUMBER))                            \
      result = (Number  *)createDouble(getNumberDouble(operand1) operator getNumberInteger(operand2));              \
    else if (isOperandType(operand1, INTEGER_NUMBER) && isOperandType(operand2, FLOAT_NUMBER))                      \
      result = (Number  *)createDouble(getNumberInteger(operand1) operator getNumberDouble(operand2));              \
    else                                                                                                            \
      result = (Number  *)createDouble(getNumberDouble(operand1) operator getNumberDouble(operand2));               \
    result->type = FLOAT_NUMBER;                                                                                    \
  }           

#define infixArithmeticDivCalculation(operand1, operand2, operator)                                                 \
  if(isOperandType(operand1, INTEGER_NUMBER) && isOperandType(operand2, INTEGER_NUMBER)){                           \
    if(getNumberInteger(operand1) % getNumberInteger(operand2) == 0){                                               \
      result = (Number  *)createInteger(getNumberInteger(operand1) operator getNumberInteger(operand2));            \
      result->type = INTEGER_NUMBER;                                                                                \
    }else{                                                                                                          \
      result = (Number  *)createDouble(getNumberInteger(operand1) operator getNumberInteger(operand2));             \
      result->type = INTEGER_NUMBER;                                                                                \
    }                                                                                                               \
  }else                                                                                                             \
    {                                                                                                               \
    if(isOperandType(operand1, FLOAT_NUMBER) && isOperandType(operand2, INTEGER_NUMBER))                            \
      result = (Number  *)createDouble(getNumberDouble(operand1) operator getNumberInteger(operand2));              \
    else if (isOperandType(operand1, INTEGER_NUMBER) && isOperandType(operand2, FLOAT_NUMBER))                      \
      result = (Number  *)createDouble(getNumberInteger(operand1) operator getNumberDouble(operand2));              \
    else                                                                                                            \
      result = (Number  *)createDouble(getNumberDouble(operand1) operator getNumberDouble(operand2));               \
    result->type = FLOAT_NUMBER;                                                                                    \
  }   
  
#define infixLogicCalculation(operand1, operand2, operator)                                                         \
    result = (Number  *)createInteger(getNumberInteger(operand1) operator getNumberInteger(operand2));              \
    result->type = INTEGER_NUMBER;                                                                                  \
 
  
#define prefixLogicCalculation(operand, operator)                                                                   \
  result = (Number  *)createInteger(operator(getNumberInteger(operand)));                                            \
  result->type = INTEGER_NUMBER;                                                                                                                                                                                                

#endif // ARITHMETIC_H
