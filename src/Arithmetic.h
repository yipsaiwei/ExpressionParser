#ifndef ARITHMETIC_H
#define ARITHMETIC_H

#include  "ExpressionParser.h"
#define arithmeticCalculation(operand1, operand2, operator)                                                         \
  if(isOperandType(operand1->data, INTEGER_NUMBER) && isOperandType(operand2->data, INTEGER_NUMBER)){               \
    result = (Number  *)createInteger(getItemInteger(operand1) operator getItemInteger(operand2));                  \
    result->type = INTEGER_NUMBER;                                                                                  \
  }else                                                                                                             \
  {                                                                                                                 \
    if(isOperandType(operand1->data, FLOAT_NUMBER) && isOperandType(operand2->data, INTEGER_NUMBER))                \
      result = (Number  *)createDouble(getItemDouble(operand1) operator getItemInteger(operand2));                  \
    else if (isOperandType(operand1->data, INTEGER_NUMBER) && isOperandType(operand2->data, FLOAT_NUMBER))          \
      result = (Number  *)createDouble(getItemInteger(operand1) operator getItemDouble(operand2));                  \
    else                                                                                                            \
      result = (Number  *)createDouble(getItemDouble(operand1) operator getItemDouble(operand2));                   \
    result->type = FLOAT_NUMBER;                                                                                    \
  }           

#define arithmeticDivisionCalculation(operand1, operand2)                                                           \
  if(isOperandType(operand1->data, INTEGER_NUMBER) && isOperandType(operand2->data, INTEGER_NUMBER)){               \
    if(getItemInteger(operand1) % getItemInteger(operand2) == 0){                                                   \
      result = (Number  *)createInteger(getItemInteger(operand1) / getItemInteger(operand2));                       \
      result->type = INTEGER_NUMBER;                                                                                \
    }else{                                                                                                          \
      result = (Number  *)createDouble(getItemInteger(operand1) / getItemInteger(operand2));                        \
      result->type = INTEGER_NUMBER;                                                                                \
    }                                                                                                               \
  }else                                                                                                             \
    {                                                                                                               \
    if(isOperandType(operand1->data, FLOAT_NUMBER) && isOperandType(operand2->data, INTEGER_NUMBER))                \
      result = (Number  *)createDouble(getItemDouble(operand1) / getItemInteger(operand2));                         \
    else if (isOperandType(operand1->data, INTEGER_NUMBER) && isOperandType(operand2->data, FLOAT_NUMBER))          \
      result = (Number  *)createDouble(getItemInteger(operand1) / getItemDouble(operand2));                         \
    else                                                                                                            \
      result = (Number  *)createDouble(getItemDouble(operand1) / getItemDouble(operand2));                          \
    result->type = FLOAT_NUMBER;                                                                                    \
  }   
#endif // ARITHMETIC_H
