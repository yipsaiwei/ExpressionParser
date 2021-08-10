#ifndef OPERATORTABLE_H
#define OPERATORTABLE_H

#include "Token.h"
#include "Tokenizer.h"
//#include "ExpressionParser.h"
#include  "Arity.h"
#include <string.h>

#define isTokenNull(token)                                      (token->type == TOKEN_NULL_TYPE)
#define areTokenStringSame(token1, token2)                       (*(token1->str) == *(token2->str))
#define areTwoCharSame(str1, str2)                               *str1 == *str2                                                                    
#define isNextTokenAOperator(token)                             (token->type == TOKEN_OPERATOR_TYPE)
#define isNextTokenAdjacentToCurrent(token, nextToken)          ((token->startColumn + 1) == nextToken->startColumn)
#define isNextAdjacentTokenSame(token, nextToken)               (areTokenStringSame(token, nextToken) && isNextTokenAdjacentToCurrent(token, nextToken))
#define isNextAdjacentTokenStringEqual(token, nextToken)        (isNextTokenAdjacentToCurrent(token, nextToken) && *(nextToken->str) == '=')
typedef enum{
  UND,
  ADD,
  ADD_ASSIGN,
  MINUS,
  MINUS_ASSIGN,
  DIVIDE,
  MULTIPLY,
  REMAINDER,
  REMAINDER_ASSIGN,
  INC,
  DEC,
  OPEN_PAREN,
  CLOSE_PAREN,
  OPEN_SQ_BRACKET,
  CLOSE_SQ_BRACKET,
  BITWISE_NOT,
  LOGICAL_NOT,
  LESSER, 
  GREATER,
  LESSER_EQ,
  GREATER_EQ,
  SHIFT_LEFT,
  SHIFT_RIGHT,
  BITWISE_AND,
  LOGICAL_AND,
  BITWISE_AND_ASSIGN,
  BITWISE_OR, 
  BITWISE_OR_ASSIGN, 
  LOGICAL_OR,
  BITWISE_XOR, 
  BITWISE_XOR_ASSIGN,
  SHIFT_LEFT_ASSIGN,
  SHIFT_RIGHT_ASSIGN,
  ASSIGN, 
  EQUAL,
}OperationType;

typedef struct  Operator  Operator;
struct  Operator{
  char  *str;
  int precedence;
  OperationType id;
};

typedef Operator *(*FuncPtr)(Token *token, Tokenizer *tokenizer);

typedef struct  OperatorInformationTable  OperatorInformationTable;
struct  OperatorInformationTable{
  OperationType type[4];
  FuncPtr func;
};

Operator  *createOperator(char  *str, int  precedence, OperationType id);
Operator  *returnDoubleCharacterOperator(Token *token, Token *nextToken);
void  freeOperator(Operator *opt);
int returnStringSize(char *str);
void  checkDoubleCharacterOperator(Tokenizer  *tokenizer, Token *token);
Token *peekToken(Tokenizer  *tokenizer);
char  *concatenateTwoStrings(char  *str1, char *str2);
Operator  *handleSymbol(Token *token, Tokenizer  *tokenizer);
Operator  *handleRepeatedSymbol(Token *token, Tokenizer  *tokenizer);
Operator  *handleShiftSymbol(Token *token, Tokenizer  *tokenizer);

#endif // OPERATORTABLE_H
