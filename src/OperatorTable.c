#include "OperatorTable.h"

//Change it to symbolTable
OperatorInformationTable operatorInformationTable[] = {
  ['('] = {{OPEN_PAREN, UND, UND}, NULL},  
  [')'] = {{CLOSE_PAREN, UND, UND}, NULL},  
  ['['] = {{OPEN_SQ_BRACKET, UND, UND}, NULL},  
  [']'] = {{CLOSE_SQ_BRACKET, UND, UND}, NULL},  
  ['~'] = {{BITWISE_NOT, UND, UND}, NULL},  
  ['!'] = {{LOGICAL_NOT, UND, UND}, NULL},  
  ['*'] = {{MULTIPLY, UND, UND}, NULL},  
  ['/'] = {{DIVIDE, UND, UND}, NULL},  
  ['%'] = {{REMAINDER, UND, UND}, NULL},  
  ['+'] = {{ADD, INC, ADD_ASSIGN}, returnDoubleCharacterOperator},     //There are postfix infix inc, addition, +=?
  ['-'] = {{MINUS, DEC, MINUS_ASSIGN}, returnDoubleCharacterOperator}, 
  ['<'] = {{LESSER, SHIFT_LEFT, LESSER_EQ}, returnDoubleCharacterOperator}, 
  ['>'] = {{GREATER, SHIFT_RIGHT, GREATER_EQ}, returnDoubleCharacterOperator}, 
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

Operator  *extractOperatorFromToken(Token *token, Tokenizer *tokenizer){
  Operator  *operator;
  char  *operatorStr;
  Token *nextToken = peekToken(tokenizer);
  OperatorInformationTable  information = operatorInformationTable[*(token->str)];
  if(isNextTokenAdjacentToCurrent(token, nextToken) || isNextTokenAOperator(nextToken))
    operator = information.func(token, nextToken);
  else
    operator = createOperator(token->str, 0, information.type[0]); 
  return  operator;
}

void  freeOperator(Operator *operator){
  if(operator)
    free(operator);
}

Operator  *returnDoubleCharacterOperator(Token *token, Token *nextToken){
  char  *operatorStr;
  Operator  *operator;
  OperatorInformationTable  information = operatorInformationTable[*(token->str)];
  if(!isNextTokenAdjacentToCurrent(token, nextToken)){
    //Throw Exception
    throwException(ERROR_INVALID_OPERATOR,NULL, 0, "Invalid Operator!");
  }
  strcat(token->str, nextToken->str);
  operatorStr = strdup(token->str);
  if(areTwoCharSame(token->str, nextToken->str))
    operator = createOperator(operatorStr, 0, information.type[1]);
  else 
    operator = createOperator(operatorStr, 0, information.type[2]);
  return  operator;
}

Token *peekToken(Tokenizer  *tokenizer){
  int indexBeforeGetToken = tokenizer->index;
  Token *token = getToken(tokenizer);
  pushBackToken(tokenizer, token);
  tokenizer->index = indexBeforeGetToken;
  return  token;
}