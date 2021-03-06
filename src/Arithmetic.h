#ifndef ARITHMETIC_H
#define ARITHMETIC_H

//#include  "ExpressionParser.h"

#define createInfixArithmeticFunction(funcName, operator)                   \
Symbol  *funcName(Symbol  *number1, Symbol  *number2){                      \
  Symbol  *result;                                                          \
  Token *resultToken;                                                       \
  char  *resultStr;                                                         \
  infixArithmeticCalculation(number1, number2, operator);                   \
  return  result;                                                           \
}

#define createPrefixArithmeticFunction(funcName, operator)                  \
Symbol  *funcName(Symbol  *number, Symbol  *number2){                       \
  Symbol  *result;                                                          \
  Token *resultToken;                                                       \
  char  *resultStr;                                                         \
  prefixArithmeticCalculation(number, operator);                            \
  return  result;                                                           \
}

#define createSuffixIncDecFunction(funcName, operator)                      \
Symbol  *funcName(Symbol  *number, Symbol  *number2){                       \
  Symbol  *result;                                                          \
  Token *resultToken;                                                       \
  char  *resultStr;                                                         \
  suffixIncDecCalculation(number, operator);                                \
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

#define createSuffixLogicFunction(funcName, operator)                       \
Symbol  *funcName(Symbol  *number, Symbol *number2){                        \
  Symbol  *result;                                                          \
  Token *resultToken;                                                       \
  char  *resultStr;                                                         \
  suffixLogicCalculation(number, operator);                                 \
  return  result;                                                           \
}

#define createInfixLogicFunction(funcName, operator)                        \
Symbol  *funcName(Symbol  *number1, Symbol *number2){                       \
  Symbol  *result;                                                          \
  Token *resultToken;                                                       \
  char  *resultStr;                                                         \
  infixLogicCalculation(number1, number2, operator);                        \
  return  result;                                                           \
}

#define createInfixLogicFunctionWithDouble(funcName, operator)              \
Symbol  *funcName(Symbol  *number1, Symbol *number2){                       \
  Symbol  *result;                                                          \
  Token *resultToken;                                                       \
  char  *resultStr;                                                         \
  infixLogicCalculationWithDouble(number1, number2, operator);              \
  return  result;                                                           \
}

#define infixArithmeticCalculation(operand1, operand2, operator)                                                                                \
  if(isSymbolInteger(number1) && isSymbolInteger(number2)){                                                                                     \
    int resultNum = getSymbolInteger(number1) operator getSymbolInteger(number2);                                                               \
    resultStr = createResultString((void  *)&resultNum, INTEGER);                                                                               \
    resultToken = (Token  *)createIntToken(resultNum, number2->token->startColumn, number2->token->originalstr, resultStr, TOKEN_INTEGER_TYPE); \
    result = createSymbol(resultToken,  OPERAND, INTEGER);                                                                                      \
  }else{                                                                                                                                        \
    double  resultNum;                                                                                                                          \
    if(isSymbolDouble(number1) && isSymbolInteger(number2))                                                                                     \
      resultNum = getSymbolDouble(number1) operator getSymbolInteger(number2);                                                                  \
    else if (isSymbolInteger(number1) && isSymbolDouble(number2))                                                                               \
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

#define prefixArithmeticCalculation(operand, operator)                                                                                           \
  if(isSymbolInteger(operand)){                                                                                                                  \
    int resultNum = operator(getSymbolInteger(operand));                                                                                         \
    resultStr = createResultString((void  *)&resultNum, INTEGER);                                                                                \
    resultToken = (Token  *)createIntToken(resultNum, operand->token->startColumn, operand->token->originalstr, resultStr, TOKEN_INTEGER_TYPE);  \
    result = createSymbol(resultToken,  OPERAND, INTEGER);                                                                                       \
  }else{                                                                                                                                         \
    double  resultNum = operator(getSymbolDouble(operand));                                                                                      \
    resultStr = createResultString((void  *)&resultNum, DOUBLE);                                                                                 \
    resultToken = (Token  *)createFloatToken(resultNum, number->token->startColumn, number->token->originalstr, resultStr, TOKEN_FLOAT_TYPE);    \
    result = createSymbol(resultToken, OPERAND, DOUBLE);                                                                                         \
  }    
  
#define suffixIncDecCalculation(operand, operator)                                                                                               \
  if(isSymbolInteger(operand)){                                                                                                                  \
    int resultNum = getSymbolInteger(operand);                                                                                                   \
    resultNum operator;                                                                                                                         \
    resultStr = createResultString((void  *)&resultNum, INTEGER);                                                                                \
    resultToken = (Token  *)createIntToken(resultNum, operand->token->startColumn, operand->token->originalstr, resultStr, TOKEN_INTEGER_TYPE);  \
    result = createSymbol(resultToken,  OPERAND, INTEGER);                                                                                       \
  }else{                                                                                                                                         \
    double  resultNum = getSymbolDouble(operand);                                                                                                \
    resultNum operator;                                                                                                                         \
    resultStr = createResultString((void  *)&resultNum, DOUBLE);                                                                                 \
    resultToken = (Token  *)createFloatToken(resultNum, number2->token->startColumn, number2->token->originalstr, resultStr, TOKEN_FLOAT_TYPE);  \
    result = createSymbol(resultToken, OPERAND, DOUBLE);                                                                                         \
  }    

//If operand 1 or operand 2 is double, it will be forced to convert to integer (For shift left right etc that won't accept double) 
#define infixLogicCalculation(operand1, operand2, operator)                                                                                      \
  int resultNum;                                                                                                                                 \
  if(isSymbolInteger(number1) && isSymbolInteger(number2))                                                                                       \
    resultNum = getSymbolInteger(number1) operator getSymbolInteger(number2);                                                                    \
  else if(isSymbolDouble(number1) && isSymbolInteger(number2))                                                                                   \
   resultNum = (int)getSymbolDouble(number1) operator getSymbolInteger(number2);                                                                 \
  else if(isSymbolInteger(number1) && isSymbolDouble(number2))                                                                                   \
    resultNum = getSymbolInteger(number1) operator (int)getSymbolDouble(number2);                                                                \
  else                                                                                                                                           \
    resultNum = (int)getSymbolDouble(number1) operator (int)getSymbolDouble(number2);                                                            \
  resultStr = createResultString((void  *)&resultNum, INTEGER);                                                                                  \
  resultToken = (Token  *)createIntToken(resultNum, number2->token->startColumn, number2->token->originalstr, resultStr, TOKEN_INTEGER_TYPE);    \
  result = createSymbol(resultToken,  OPERAND, INTEGER);                  

#define infixLogicCalculationWithDouble(operand1, operand2, operator)                                                                            \
  int resultNum;                                                                                                                                 \
  if(isSymbolInteger(number1) && isSymbolInteger(number2))                                                                                       \
    resultNum = getSymbolInteger(number1) operator getSymbolInteger(number2);                                                                    \
  else if(isSymbolDouble(number1) && isSymbolInteger(number2))                                                                                   \
   resultNum = getSymbolDouble(number1) operator getSymbolInteger(number2);                                                                      \
  else if(isSymbolInteger(number1) && isSymbolDouble(number2))                                                                                   \
    resultNum = getSymbolInteger(number1) operator getSymbolDouble(number2);                                                                     \
  else                                                                                                                                           \
    resultNum = getSymbolDouble(number1) operator getSymbolDouble(number2);                                                                      \
  resultStr = createResultString((void  *)&resultNum, INTEGER);                                                                                  \
  resultToken = (Token  *)createIntToken(resultNum, number2->token->startColumn, number2->token->originalstr, resultStr, TOKEN_INTEGER_TYPE);    \
  result = createSymbol(resultToken,  OPERAND, INTEGER);   
 
  
#define prefixLogicCalculation(operand, operator)                                                                                               \
    int resultNum;                                                                                                                              \
    if(isSymbolInteger(operand))                                                                                                                \
      resultNum = operator(getSymbolInteger(operand));                                                                                          \
    else                                                                                                                                        \
      resultNum = operator((int)getSymbolDouble(operand));                                                                                      \
    resultStr = createResultString((void  *)&resultNum, INTEGER);                                                                               \
    resultToken = (Token  *)createIntToken(resultNum, operand->token->startColumn, operand->token->originalstr, resultStr, TOKEN_INTEGER_TYPE); \
    result = createSymbol(resultToken,  OPERAND, INTEGER);     

#define suffixLogicCalculation(operand, operator)                                                                                               \
    int resultNum = (getSymbolInteger(operand))operator;                                                                                        \
    resultStr = createResultString((void  *)&resultNum, INTEGER);                                                                               \
    resultToken = (Token  *)createIntToken(resultNum, operand->token->startColumn, operand->token->originalstr, resultStr, TOKEN_INTEGER_TYPE); \
    result = createSymbol(resultToken,  OPERAND, INTEGER);      

#endif // ARITHMETIC_H
