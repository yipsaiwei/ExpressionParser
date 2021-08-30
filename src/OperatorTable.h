#ifndef OPERATORTABLE_H
#define OPERATORTABLE_H

#include "Token.h"
#include "Tokenizer.h"
//#include "ExpressionParser.h"
#include  "Arity.h"
#include <string.h>

#define isTokenNull(token)                                      (token->type == TOKEN_NULL_TYPE)
#define areTokenStringSame(token1, token2)                      (*(token1->str) == *(token2->str))
#define areTwoCharSame(str1, str2)                               *str1 == *str2                                                                    
#define isNextTokenAOperator(token)                             (token->type == TOKEN_OPERATOR_TYPE)
#define isNextTokenAdjacentToCurrent(token, nextToken)          ((token->startColumn + 1) == nextToken->startColumn)
#define isNextAdjacentTokenSame(token, nextToken)               (areTokenStringSame(token, nextToken) && isNextTokenAdjacentToCurrent(token, nextToken))
#define isNextAdjacentTokenStringEqual(token, nextToken)        (isNextTokenAdjacentToCurrent(token, nextToken) && *(nextToken->str) == '=')
#define isTokenOperatorType(token)                              token->type == TOKEN_OPERATOR_TYPE
#define isTokenIntegerType(token)                               token->type == TOKEN_INTEGER_TYPE
#define isTokenDoubleType(token)                                token->type == TOKEN_FLOAT_TYPE
#define isTokenNullType(token)                                  token->type == TOKEN_NULL_TYPE

#define isSymbolInteger(symbol)                                 symbol->id == INTEGER
#define isSymbolDouble(symbol)                                  symbol->id == DOUBLE
#define isSymbolNull(symbol)                                    symbol->id == _NULL
#define isSymbolOperatorType(symbol)                            symbol->type == OPERATOR

#define isPreviousSymbolType(symbolizer, type)                  getLastSymbolType(symbolizer) == type
#define isPreviousArity(symbolizer, arity)                      returnArityOfAnId(symbolizer->lastSymbolId) == arity
#define isIdArity(symbolId, arity)                              returnArityOfAnId(symbolId) == arity
#define isPreviousSymbolId(symbolizer, id)                      symbolizer->lastSymbolId== id

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
  PRE_INC,
  POST_INC,
  DEC,
  PLUS_SIGN,
  MINUS_SIGN,
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
  INTEGER,
  DOUBLE,
  _NULL,
}OperationType;

typedef enum{
  OPERAND,
  OPERATOR,
  EMPTY,
}AttributeType;

typedef struct  Symbol  Symbol;
struct  Symbol{
  Token *token;
  AttributeType type;
  OperationType id;
  ARITY arity;
};

typedef struct  Symbolizer  Symbolizer;
struct  Symbolizer{
  Tokenizer *tokenizer;
  OperationType lastSymbolId;
  //AttributeType lastSymbolType;   //macro
  //ARITY lastSymbolArity;          //macro
};

typedef Symbol *(*FuncPtr)(Symbolizer *symbolizer, Token  *token);

typedef struct  OperatorInformationTable  OperatorInformationTable;
struct  OperatorInformationTable{
  OperationType type[4];
  FuncPtr func;
};

int returnStringSize(char *str);
void  checkDoubleCharacterOperator(Tokenizer  *tokenizer, Token *token);
Token *peekToken(Tokenizer  *tokenizer);
char  *concatenateTwoStrings(char  *str1, char *str2);
Symbol  *handleSymbol(Symbolizer *symbolizer, Token  *token);
Symbol  *handleRepeatedSymbol(Symbolizer *symbolizer, Token  *token);
Symbol  *handleShiftSymbol(Symbolizer *symbolizer, Token  *token);

Symbolizer  *createSymbolizer(Tokenizer  *tokenizer);
void  freeSymbolizer(Symbolizer *symbolizer);
Symbol  *createSymbol(Token *token, AttributeType type, OperationType id);
Symbol  *getSymbol(Symbolizer *symbolizer);
void  freeSymbol(Symbol *symbol);

AttributeType getLastSymbolType(Symbolizer  *symbolizer);

ARITY returnArityOfAnId(OperationType type);
#endif // OPERATORTABLE_H
