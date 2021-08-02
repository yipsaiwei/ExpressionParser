#ifndef OPERATORTABLE_H
#define OPERATORTABLE_H

#include "Token.h"
#include "Tokenizer.h"
//#include "ExpressionParser.h"
#include  "Arity.h"

#define isTokenNull(token)                  (token->type == NULL)
#define isTokenStringSame(token1, token2)   (token1->str == token2->str)
#define isNextTokenAOperator(token)         (token->type == TOKEN_OPERATOR_TYPE)
typedef void (*FuncPtr)(Tokenizer *tokenizer, Token *token);

typedef enum{
  UND,
  ADD,
  ADD_ASSIGN,
  MINUS,
  MINUS_ASSIGN,
  DIVIDE,
  MULTIPLY,
  REMAINDER,
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
  
}OperationType;

typedef struct  Operator  Operator;
struct  Operator{
  char  *str;
  int precedence;
  int arity;
};

typedef struct  OperatorInformationTable  OperatorInformationTable;
struct  OperatorInformationTable{
  OperationType type[3];
  FuncPtr func;
};

Operator  *createOperator(char  *str, int  precedence, int arity);
Operator  *extractOperatorFromToken(Token *token, int precedence, Tokenizer *tokenizer);
void  freeOperator(Operator *opt);
int returnStringSize(char *str);
void  checkDoubleCharacterOperator(Tokenizer  *tokenizer, Token *token);
Token *peekToken(Tokenizer  *tokenizer);

#endif // OPERATORTABLE_H
