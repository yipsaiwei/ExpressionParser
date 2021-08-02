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
  ['+'] = {{ADD, INC, ADD_ASSIGN}, NULL},     //There are postfix infix inc, addition, +=?
  ['-'] = {{MINUS, DEC, MINUS_ASSIGN}, NULL}, 
  ['<'] = {{LESSER, SHIFT_LEFT, LESSER_EQ}, NULL}, 
  ['>'] = {{GREATER, SHIFT_RIGHT, GREATER_EQ}, NULL}, 
};

Operator  *createOperator(char  *str, int  precedence, int arity){
  Operator  *newOperator = malloc(sizeof(Operator));
  newOperator->arity = arity;
  newOperator->precedence = precedence;
  newOperator->str = duplicateSubstring(str, returnStringSize(str));
  return  newOperator;
}

int returnStringSize(char *str){
  char  *ptr = str;
  while(*ptr != '\0')
    ptr++;
  return  (ptr - str);
}

Operator  *extractOperatorFromToken(Token *token, int precedence, Tokenizer *tokenizer){
  Operator  *newOperator = createOperator(token->str, precedence, BINARY);
  return  newOperator;
}

void  freeOperator(Operator *operator){
  if(operator)
    free(operator);
}

void  checkDoubleCharacterOperator(Tokenizer  *tokenizer, Token *token){
  Token *nextToken = peekToken(tokenizer);
  if(isNextTokenAOperator(nextToken)){
    if(isTokenStringSame(token, nextToken)){
      switch(*(token->str)){
        case '+':
        ;
        break;
        case  '-':
        ;
        break;
        case  '<':
        ;
        break;
        case  '>':
        ;
        break;        
        case  '&':
        ;
        break; 
        case  '|':
        ;
        break; 
        default:
        ; //Throw exception
      }
    }else{
      switch(*(token->str)){
        case '+':
        ;
        break;
        case  '-':
        ;
        break;
        case  '<':
        ;
        break;
        case  '>':
        ;
        break;        
        case  '&':
        ;
        break; 
        case  '|':
        ;
        break; 
        default:
        ; //Throw exception
      }      
    }
  }
}


Token *peekToken(Tokenizer  *tokenizer){
  int indexBeforeGetToken = tokenizer->index;
  Token *token = getToken(tokenizer);
  pushBackToken(tokenizer, token);
  tokenizer->index = indexBeforeGetToken;
  return  token;
}