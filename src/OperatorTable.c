#include "OperatorTable.h"

//Change it to symbolTable
OperatorInformationTable operatorInformationTable[] = {
  ['('] = {{OPEN_PAREN, UND, UND, UND}, NULL},  
  [')'] = {{CLOSE_PAREN, UND, UND, UND}, NULL},  
  ['['] = {{OPEN_SQ_BRACKET, UND, UND, UND}, NULL},  
  [']'] = {{CLOSE_SQ_BRACKET, UND, UND, UND}, NULL},  
  ['~'] = {{BITWISE_NOT, UND, UND, UND}, NULL},  
  ['!'] = {{LOGICAL_NOT, UND, UND, UND}, NULL},  
  ['*'] = {{MULTIPLY, UND, UND, UND}, NULL},  
  ['/'] = {{DIVIDE, UND, UND, UND}, NULL},  
  ['%'] = {{REMAINDER, UND, UND, UND}, NULL},  
  ['+'] = {{ADD,ADD_ASSIGN, INC, UND}, handleRepeatedSymbol},     //There are postfix infix inc, addition, +=?
  ['-'] = {{MINUS, MINUS_ASSIGN, DEC, UND}, handleRepeatedSymbol}, 
  ['<'] = {{LESSER, LESSER_EQ, SHIFT_LEFT, UND}, handleSymbol}, 
  ['>'] = {{GREATER, GREATER_EQ, SHIFT_RIGHT, UND}, handleSymbol}, 
  ['&'] = {{BITWISE_AND, BITWISE_AND_ASSIGN, LOGICAL_AND, UND},handleSymbol},
};

Operator  *createOperator(char  *str, int  precedence, OperationType id){
  Operator  *newOperator = malloc(sizeof(Operator));
  newOperator->precedence = precedence;
  newOperator->id = id;
  newOperator->str = duplicateSubstring(str, returnStringSize(str));
  return  newOperator;
}

int returnStringSize(char *str){
  char  *ptr = str;
  while(*ptr != '\0')
    ptr++;
  return  (ptr - str);
}

void  freeOperator(Operator *operator){
  if(operator->str)
    free(operator->str);
  if(operator)
    free(operator);
}

Operator  *handleRepeatedSymbol(Token *token, Token *nextToken){
  Operator  *operator;
  OperatorInformationTable  information = operatorInformationTable[*(token->str)];
  if(!isTokenNull(nextToken) && areTokenStringSame(token, nextToken) && isNextTokenAdjacentToCurrent(token, nextToken)){
    operator = createOperator(concatenateTwoStrings(token->str, nextToken->str), 0, information.type[2]);
    freeToken(nextToken); 
    freeToken(token);    
  }else{
    operator = handleSymbol(token, nextToken);
  }
  return  operator;
}

Operator  *handleSymbol(Token *token, Token *nextToken){
  Operator  *operator;
  OperatorInformationTable  information = operatorInformationTable[*(token->str)]; 
  if(!isTokenNull(nextToken) && *(nextToken->str) == '=' && isNextTokenAdjacentToCurrent(token, nextToken)){
    operator = createOperator(concatenateTwoStrings(token->str, nextToken->str), 0, information.type[1]);
    freeToken(nextToken);    
  }else
    operator = createOperator(token->str, 0, information.type[0]);
  freeToken(token);
  return  operator;
}

/*
Operator  *handleShiftSymbol(Token *token, Token *nextToken){
  Operator  *operator;
  OperatorInformationTable  information = operatorInformationTable[*(token->str)]; 
  if(!isTokenNull(nextToken) && areTokenStringSame(token, nextToken) && isNextTokenAdjacentToCurrent(token, nextToken)){
    Token *
  }  
}
*/

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