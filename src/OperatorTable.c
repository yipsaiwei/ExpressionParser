#include "OperatorTable.h"

//Change it to symbolTable
OperatorInformationTable operatorInformationTable[] = {
  ['('] = {{OPEN_PAREN      , UND               , UND         , UND}                , handleSymbol},  
  [')'] = {{CLOSE_PAREN     , UND               , UND         , UND}                , handleSymbol},  
  ['['] = {{OPEN_SQ_BRACKET , UND               , UND         , UND}                , NULL},  
  [']'] = {{CLOSE_SQ_BRACKET, UND               , UND         , UND}                , NULL},  
  ['~'] = {{BITWISE_NOT     , UND               , UND         , UND}                , handleSymbol},  
  ['!'] = {{LOGICAL_NOT     , UND               , UND         , UND}                , handleSymbol},  
  ['*'] = {{MULTIPLY        , UND               , UND         , UND}                , handleSymbol},  
  ['/'] = {{DIVIDE          , UND               , UND         , UND}                , handleSymbol},  
  ['%'] = {{REMAINDER       , REMAINDER_ASSIGN  , UND         , UND}                , handleSymbol},  
  ['+'] = {{ADD             , ADD_ASSIGN        , INC         , UND}                , handleRepeatedSymbol},     //There are postfix infix inc, addition, +=?
  ['-'] = {{MINUS           , MINUS_ASSIGN      , DEC         , UND}                , handleRepeatedSymbol}, 
  ['<'] = {{LESSER          , LESSER_EQ         , SHIFT_LEFT  , SHIFT_LEFT_ASSIGN}  , handleShiftSymbol}, 
  ['>'] = {{GREATER         , GREATER_EQ        , SHIFT_RIGHT , SHIFT_RIGHT_ASSIGN} , handleShiftSymbol}, 
  ['|'] = {{BITWISE_OR      , BITWISE_OR_ASSIGN , LOGICAL_OR  , UND}                , handleRepeatedSymbol},
  ['&'] = {{BITWISE_AND     , BITWISE_AND_ASSIGN, LOGICAL_AND , UND}                , handleRepeatedSymbol},
  ['^'] = {{BITWISE_XOR     , BITWISE_XOR_ASSIGN, UND         , UND}                , handleSymbol},
  ['='] = {{ASSIGN          , EQUAL             , UND         , UND}                , handleSymbol},
};

int returnStringSize(char *str){
  char  *ptr = str;
  while(*ptr != '\0')
    ptr++;
  return  (ptr - str);
}

Symbolizer  *createSymbolizer(Tokenizer  *tokenizer){
  Symbolizer  *symbolizer = malloc(sizeof(Symbolizer));
  symbolizer->tokenizer = tokenizer;
  symbolizer->lastSymbolId = _NULL;
  return  symbolizer;
}

void  freeSymbolizer(Symbolizer *symbolizer){
  if(symbolizer->tokenizer)
    freeTokenizer(symbolizer->tokenizer);
  if(symbolizer)
    free(symbolizer);
}

Symbol  *createSymbol(Token *token, AttributeType type, OperationType id){
  Symbol  *symbol = malloc(sizeof(Symbol));
  symbol->token = token;
  symbol->type = type;
  symbol->id = id;
  return  symbol;
}

void  freeSymbol(Symbol *symbol){
  if(symbol->token)
    freeToken(symbol->token);
  if(symbol)
    free(symbol);
}

Symbol  *getSymbol(Symbolizer *symbolizer){
  Token *token = getToken(symbolizer->tokenizer);
  Symbol  *symbol;
  if(isTokenOperatorType(token)){
    OperatorInformationTable  information = operatorInformationTable[*(token->str)];
    symbol = information.func(symbolizer, token);
  }else if (isTokenIntegerType(token))
    symbol = createSymbol(token, OPERAND, INTEGER);
  else if (isTokenDoubleType(token))
    symbol = createSymbol(token, OPERAND, DOUBLE);    //Later need to implement exception for operands other than double and int
  else 
    symbol = createSymbol(token, EMPTY, _NULL);
  return  symbol;
}

Symbol  *handleSymbol(Symbolizer *symbolizer, Token  *token){
  Symbol  *symbol;
  Token *nextToken = peekToken(symbolizer->tokenizer);
  OperatorInformationTable  information = operatorInformationTable[*(token->str)]; 
  if(!isTokenNull(nextToken) && isNextAdjacentTokenStringEqual(token, nextToken)){
    nextToken = getToken(symbolizer->tokenizer);
    symbol = createSymbol(token, OPERATOR, information.type[1]);   
  }else
    symbol = createSymbol(token, OPERATOR, information.type[0]);
  return  symbol;
}

Symbol  *handleRepeatedSymbol(Symbolizer *symbolizer, Token  *token){
  Symbol  *symbol;
  Token *nextToken = peekToken(symbolizer->tokenizer);
  OperatorInformationTable  information = operatorInformationTable[*(token->str)];
  if(!isTokenNull(nextToken) && isNextAdjacentTokenSame(token, nextToken)){
    nextToken = getToken(symbolizer->tokenizer);
    symbol = createSymbol(token, OPERATOR, information.type[2]); 
    freeToken(nextToken); 
  }else
    symbol = handleSymbol(symbolizer, token);
  return  symbol;
}

Symbol  *handleShiftSymbol(Symbolizer *symbolizer, Token  *token){
  Symbol  *symbol;
  Token *nextToken = peekToken(symbolizer->tokenizer);
  OperatorInformationTable  information = operatorInformationTable[*(token->str)]; 
  if(!isTokenNull(nextToken) && isNextAdjacentTokenSame(token, nextToken)){
    nextToken = getToken(symbolizer->tokenizer);
    Token *nextNextToken = peekToken(symbolizer->tokenizer);
    if(!isTokenNull(nextNextToken) && isNextAdjacentTokenStringEqual(nextToken, nextNextToken)){
      nextNextToken = getToken(symbolizer->tokenizer);
      char  *strPtr = concatenateTwoStrings(token->str, nextToken->str);
      symbol = createSymbol(token, OPERATOR, information.type[3]);
      freeToken(nextToken);
      freeToken(nextNextToken);
    }else{
      pushBackToken(symbolizer->tokenizer, nextToken);
      symbol = handleRepeatedSymbol(symbolizer, token);
    }
  }else
    symbol = handleSymbol(symbolizer, token);
  return  symbol;
}

AttributeType getLastSymbolType(Symbolizer  *symbolizer){
  if(isPreviousSymbolId(symbolizer, INTEGER) || isPreviousSymbolId(symbolizer, DOUBLE))
    return  OPERAND;
  else
    return  OPERATOR;
}

char  *concatenateTwoStrings(char  *str1, char *str2){
  char  *destination = malloc(sizeof(char) * (1 + strlen(str1) + strlen(str2)));
  destination = strdup(str1);
  strcat(destination, str2);
  return  destination;
}

/*
Operator  *returnDoubleCharacterOperator(Token *token, Token *nextToken){
  char  *operatorStr;
  Operator  *operator;
  OperatorInformationTable  information = operatorInformationTable[*(token->str)];
  if(!isNextTokenAdjacentToCurrent(token, nextToken) || 
  (!areTokenStringSame(token, nextToken) && *(nextToken->str) != '=')){
    operator = createOperator(token->str, 0, information.type[0]); 
    freeToken(token);
  }else{
    operatorStr = malloc(sizeof(char) * 3);
    operatorStr = strdup(token->str);
    strcat(operatorStr, nextToken->str);
    if(areTwoCharSame(token->str, nextToken->str))
      operator = createOperator(operatorStr, 0, information.type[2]);
    else 
      operator = createOperator(operatorStr, 0, information.type[1]);
    // Free operator
    freeToken(token);
    freeToken(nextToken);
  }
  return  operator;
}
*/

Token *peekToken(Tokenizer  *tokenizer){
  int indexBeforeGetToken = tokenizer->index;
  Token *token = getToken(tokenizer);
  pushBackToken(tokenizer, token);
  tokenizer->index = indexBeforeGetToken;
  return  token;
}