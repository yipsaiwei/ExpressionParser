#ifndef ARITHMETIC_H
#define ARITHMETIC_H

//#include  "ExpressionParser.h"

#define arithmeticCalculation(operand1, operand2, operator)                                                         \
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

#define arithmeticDivisionCalculation(operand1, operand2, operator)                                                 \
  if(isOperandType(operand1, INTEGER_NUMBER) && isOperandType(operand2, INTEGER_NUMBER)){                           \
    if(getNumberInteger(operand1) % getNumberInteger(operand2) == 0){                                                   \
      result = (Number  *)createInteger(getNumberInteger(operand1) operator getNumberInteger(operand2));            \
      result->type = INTEGER_NUMBER;                                                                                \
    }else{                                                                                                          \
      result = (Number  *)createDouble(getNumberInteger(operand1) operator getNumberInteger(operand2));             \
      result->type = INTEGER_NUMBER;                                                                                \
    }                                                                                                               \
  }else                                                                                                             \
    {                                                                                                               \
    if(isOperandType(operand1, FLOAT_NUMBER) && isOperandType(operand2, INTEGER_NUMBER))                            \
      result = (Number  *)createDouble(getNumberDouble(operand1) operator getNumberInteger(operand2));                \
    else if (isOperandType(operand1, INTEGER_NUMBER) && isOperandType(operand2, FLOAT_NUMBER))                      \
      result = (Number  *)createDouble(getNumberInteger(operand1) operator getNumberDouble(operand2));                \
    else                                                                                                            \
      result = (Number  *)createDouble(getNumberDouble(operand1) operator getNumberDouble(operand2));               \
    result->type = FLOAT_NUMBER;                                                                                    \
  }   
#endif // ARITHMETIC_H
