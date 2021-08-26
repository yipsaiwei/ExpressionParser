#ifndef ARITHMETIC_H
#define ARITHMETIC_H

//#include  "ExpressionParser.h"

#define createArithmeticFunction(funcName, operator)                        \
Symbol  *funcName(Symbol  *number1, Symbol  *number2){                      \
  Symbol  *result;                                                          \
  Token *resultToken;                                                       \
  char  *resultStr;                                                         \
  infixArithmeticCalculation(number1, number2, operator);                   \
  return  result;                                                           \
}

#define createArithmeticDivFunction(funcName, operator)                     \
Symbol  *funcName(Symbol  *number1, Symbol  *number2){                      \
  Symbol  *result;                                                          \
  Token *resultToken;                                                       \
  char  *resultStr;                                                         \
  infixArithmeticDivCalculation(number1, number2, operator);                \
  return  result;                                                           \
}

#define createPrefixLogicFunction(funcName, operator)                       \
Symbol  *funcName(Symbol  *number, Symbol *number2){                        \
  Symbol  *result;                                                          \
  Token *resultToken;                                                       \
  char  *resultStr;                                                         \
  prefixLogicCalculation(number, operator);                                 \
  return  result;                                                           \
}

#define createInfixLogicFunction(funcName, operator)                        \
Symbol  *funcName(Symbol  *number1, Symbol *number2){                       \
  Symbol  *result;                                                          \
  Token *resultToken;                                                       \
  char  *resultStr;                                                         \
  infixLogicCalculation(number1, number2, operator)                         \
  return  result;                                                           \
}

#define infixArithmeticCalculation(operand1, operand2, operator)                                                                                \
  if(isSymbolInteger(number1) && isSymbolInteger(number2)){                                                                             \
    int resultNum = getSymbolInteger(number1) operator getSymbolInteger(number2);                                                               \
    resultStr = createResultString((void  *)&resultNum, INTEGER);                                                                               \
    resultToken = (Token  *)createIntToken(resultNum, number2->token->startColumn, number2->token->originalstr, resultStr, TOKEN_INTEGER_TYPE); \
    result = createSymbol(resultToken,  OPERAND, INTEGER);                                                                                      \
  }else{                                                                                                                                        \
    double  resultNum;                                                                                                                          \
    if(isSymbolDouble(number1) && isSymbolInteger(number2))                                                                             \
      resultNum = getSymbolDouble(number1) operator getSymbolInteger(number2);                                                                  \
    else if (isSymbolInteger(number1) && isSymbolDouble(number2))                                                                       \
      resultNum = getSymbolInteger(number1) operator getSymbolDouble(number2);                                                                  \
    else                                                                                                                                        \
      resultNum = getSymbolDouble(number1) operator getSymbolDouble(number2);                                                                   \
    resultStr = createResultString((void  *)&resultNum, DOUBLE);                                                                                \
    resultToken = (Token  *)createFloatToken(resultNum, number2->token->startColumn, number2->token->originalstr, resultStr, TOKEN_FLOAT_TYPE); \
    result = createSymbol(resultToken, OPERAND, DOUBLE);                                                                                        \
  }                                                                                                                                                   

#define infixArithmeticDivCalculation(operand1, operand2, operator)                                                                               \
  if(isSymbolInteger(operand1) && isSymbolInteger(operand2)){                                                                                     \
    if(getSymbolInteger(operand1) % getSymbolInteger(operand2) == 0){                                                                             \
      int  resultNum = getSymbolInteger(number1) operator getSymbolInteger(number2);                                                              \
      resultStr = createResultString((void  *)&resultNum, INTEGER);                                                                               \
      resultToken = (Token  *)createIntToken(resultNum, number2->token->startColumn, number2->token->originalstr, resultStr, TOKEN_INTEGER_TYPE); \
      result = createSymbol(resultToken,  OPERAND, INTEGER);                                                                                      \
    }else{                                                                                                                                        \
      double  resultNum = ((double)getSymbolInteger(number1) operator getSymbolInteger(number2));                                                 \
      resultStr = createResultString((void  *)&resultNum, DOUBLE);                                                                                \
      resultToken = (Token  *)createFloatToken(resultNum, number2->token->startColumn, number2->token->originalstr, resultStr, TOKEN_FLOAT_TYPE); \
      result = createSymbol(resultToken, OPERAND, DOUBLE);                                                                                        \
    }                                                                                                                                             \
  }else{                                                                                                                                          \
    double  resultNum;                                                                                                                            \
    if(isSymbolDouble(number1) && isSymbolInteger(number2))                                                                                       \
      resultNum = getSymbolDouble(number1) operator getSymbolInteger(number2);                                                                    \
    else if (isSymbolInteger(number1) && isSymbolDouble(number2))                                                                                 \
      resultNum = getSymbolInteger(number1) operator getSymbolDouble(number2);                                                                    \
    else                                                                                                                                          \
      resultNum = getSymbolDouble(number1) operator getSymbolDouble(number2);                                                                     \
    resultStr = createResultString((void  *)&resultNum, DOUBLE);                                                                                  \
    resultToken = (Token  *)createFloatToken(resultNum, number2->token->startColumn, number2->token->originalstr, resultStr, TOKEN_FLOAT_TYPE);   \
    result = createSymbol(resultToken, OPERAND, DOUBLE);                                                                                          \
  }             
  
#define infixLogicCalculation(operand1, operand2, operator)                                                                                     \
    int resultNum = getSymbolInteger(number1) operator getSymbolInteger(number2);                                                               \
    resultStr = createResultString((void  *)&resultNum, INTEGER);                                                                               \
    resultToken = (Token  *)createIntToken(resultNum, number2->token->startColumn, number2->token->originalstr, resultStr, TOKEN_INTEGER_TYPE); \
    result = createSymbol(resultToken,  OPERAND, INTEGER);                                                                                      
 
  
#define prefixLogicCalculation(operand, operator)                                                                                               \
    int resultNum = operator(getSymbolInteger(operand));                                                                                        \
    resultStr = createResultString((void  *)&resultNum, INTEGER);                                                                               \
    resultToken = (Token  *)createIntToken(resultNum, operand->token->startColumn, operand->token->originalstr, resultStr, TOKEN_INTEGER_TYPE); \
    result = createSymbol(resultToken,  OPERAND, INTEGER);      

#endif // ARITHMETIC_H
