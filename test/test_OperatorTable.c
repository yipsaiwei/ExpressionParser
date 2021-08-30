#include "unity.h"

#include "Exception.h"
#include "CException.h"
#include "DoubleLinkedList.h"
#include "Stack.h"
#include "MemAlloc.h"
#include "ListItem.h"
#include "Token.h"
#include "Tokenizer.h"
#include "TokenizerExceptionThrowing.h"
#include  "Data.h"
#include "ExpressionParser.h"
#include  "OperatorTable.h"

CEXCEPTION_T  ex;

void setUp(void)
{
}

void tearDown(void)
{
}

void  test_createSymbolizer_given_tokenizer_expect_correct_symbolizer_returned(){
  Tokenizer *tokenizer = NULL;
  tokenizer = createTokenizer(" 3-54*422 ");   
  Symbolizer  *symbolizer = createSymbolizer(tokenizer);
  
  TEST_ASSERT_EQUAL_PTR(tokenizer, symbolizer->tokenizer);
  TEST_ASSERT_EQUAL(_NULL, symbolizer->lastSymbolId);
  
  freeSymbolizer(symbolizer);
}

void  test_handleSymbol_given_2_adjacent_different_token_expect_correct_double_char_operator_returned(){
  Tokenizer *tokenizer = NULL;
  tokenizer = createTokenizer("  <=  ");  
  Symbolizer  *symbolizer = createSymbolizer(tokenizer);
  Token *token = getToken(symbolizer->tokenizer);
  
  Symbol  *symbol = handleSymbol(symbolizer, token);
  
  TEST_ASSERT_EQUAL_STRING("<", symbol->token->str);
  TEST_ASSERT_EQUAL(LESSER_EQ, symbol->id);
  
  freeSymbolizer(symbolizer);
  freeSymbol(symbol);
}

void  test_handleSymbol_given_2_operators_separated_by_space_expect_single_character_returned(){
  Tokenizer *tokenizer = NULL;
  tokenizer = createTokenizer("  < =  ");  
  Symbolizer  *symbolizer = createSymbolizer(tokenizer);
  Token *token = getToken(symbolizer->tokenizer);
  
  Symbol  *symbol = handleSymbol(symbolizer, token);
  
  TEST_ASSERT_EQUAL_STRING("<", symbol->token->str);
  TEST_ASSERT_EQUAL(LESSER, symbol->id);
  
  freeSymbolizer(symbolizer);
  freeSymbol(symbol);
}
  
void  test_handleSymbol_given_bitwise_or_expect_bitwise_or_returned(){
  Tokenizer *tokenizer = NULL;
  tokenizer = createTokenizer(" |  ");  
  Symbolizer  *symbolizer = createSymbolizer(tokenizer);
  Token *token = getToken(symbolizer->tokenizer);
  
  Symbol  *symbol = handleSymbol(symbolizer, token);
  
  TEST_ASSERT_EQUAL_STRING("|", symbol->token->str);
  TEST_ASSERT_EQUAL(BITWISE_OR, symbol->id);
  
  freeSymbolizer(symbolizer);
  freeSymbol(symbol);
}

void  test_handleRepeatedSymbol_given_2_adjacent_plus_token_expect_INC_returned(){
  Tokenizer *tokenizer = NULL;
  tokenizer = createTokenizer("  ++  ");  
  Symbolizer  *symbolizer = createSymbolizer(tokenizer);
  Token *token = getToken(symbolizer->tokenizer);
  
  Symbol  *symbol = handleRepeatedSymbol(symbolizer, token);
  
  TEST_ASSERT_EQUAL_STRING("+", symbol->token->str);
  TEST_ASSERT_EQUAL(INC, symbol->id);
  
  freeSymbolizer(symbolizer);
  freeSymbol(symbol); 
}

void  test_handleRepeatedSymbol_given_1_plus_token_expect_ADD_returned(){
  Tokenizer *tokenizer = NULL;
  tokenizer = createTokenizer("     +  ");  
  Symbolizer  *symbolizer = createSymbolizer(tokenizer);
  Token *token = getToken(symbolizer->tokenizer);
  
  Symbol  *symbol = handleRepeatedSymbol(symbolizer, token);
  
  TEST_ASSERT_EQUAL_STRING("+", symbol->token->str);
  TEST_ASSERT_EQUAL(ADD, symbol->id);
  
  freeSymbolizer(symbolizer);
  freeSymbol(symbol);   
}

void  test_handleShiftSymbol_given_3_operators_left_shift_assign_expect_3_characters_returned(){
  Tokenizer *tokenizer = NULL;
  tokenizer = createTokenizer("  <<=  ");  
  Symbolizer  *symbolizer = createSymbolizer(tokenizer);
  Token *token = getToken(symbolizer->tokenizer);

  Symbol  *symbol = handleShiftSymbol(symbolizer, token);
  
  TEST_ASSERT_EQUAL_STRING("<", symbol->token->str);
  TEST_ASSERT_EQUAL(SHIFT_LEFT_ASSIGN, symbol->id);
  
  freeSymbolizer(symbolizer);
  freeSymbol(symbol); 
}

void  test_handleShiftSymbol_given_3_operators_right_shift_assign_expect_3_characters_returned(){
  Tokenizer *tokenizer = NULL;
  tokenizer = createTokenizer("  >>=  ");  
  Symbolizer  *symbolizer = createSymbolizer(tokenizer);
  Token *token = getToken(symbolizer->tokenizer);

  Symbol  *symbol = handleShiftSymbol(symbolizer, token);
  
  TEST_ASSERT_EQUAL_STRING(">", symbol->token->str);
  TEST_ASSERT_EQUAL(SHIFT_RIGHT_ASSIGN, symbol->id);
  
  freeSymbolizer(symbolizer);
  freeSymbol(symbol); 
}

void  test_handleShiftSymbol_given_3_operators_right_shift_space_assign_expect_right_shift_returned(){
  Tokenizer *tokenizer = NULL;
  tokenizer = createTokenizer("  >> =  ");  
  Symbolizer  *symbolizer = createSymbolizer(tokenizer);
  Token *token = getToken(symbolizer->tokenizer);

  Symbol  *symbol = handleShiftSymbol(symbolizer, token);
  
  TEST_ASSERT_EQUAL_STRING(">", symbol->token->str);
  TEST_ASSERT_EQUAL(SHIFT_RIGHT, symbol->id);
  
  freeSymbolizer(symbolizer);
  freeSymbol(symbol);  
}

void  test_get_symbol_given_symbolizer_with_add_operator_expect_operator_symbol_returned(){
  Tokenizer *tokenizer = NULL;
  tokenizer = createTokenizer("    + ");  
  Symbolizer  *symbolizer = createSymbolizer(tokenizer);

  Symbol  *symbol = getSymbol(symbolizer);
  
  TEST_ASSERT_EQUAL_STRING("+", symbol->token->str);
  TEST_ASSERT_EQUAL(ADD, symbol->id);
  
  freeSymbolizer(symbolizer);
  freeSymbol(symbol);   
}

void  test_get_symbol_given_symbolizer_with_bitwise_or_assign_operator_expect_operator_symbol_returned(){
  Tokenizer *tokenizer = NULL;
  tokenizer = createTokenizer("  |= ");  
  Symbolizer  *symbolizer = createSymbolizer(tokenizer);

  Symbol  *symbol = getSymbol(symbolizer);
  
  TEST_ASSERT_EQUAL_STRING("|", symbol->token->str);
  TEST_ASSERT_EQUAL(BITWISE_OR_ASSIGN, symbol->id);
  
  freeSymbolizer(symbolizer);
  freeSymbol(symbol);   
}

void  test_get_symbol_given_symbolizer_with_shift_right_assign_operator_expect_operator_symbol_returned(){
  Tokenizer *tokenizer = NULL;
  tokenizer = createTokenizer("  >>= ");  
  Symbolizer  *symbolizer = createSymbolizer(tokenizer);

  Symbol  *symbol = getSymbol(symbolizer);
  
  TEST_ASSERT_EQUAL_STRING(">", symbol->token->str);
  TEST_ASSERT_EQUAL(SHIFT_RIGHT_ASSIGN, symbol->id);
  
  freeSymbolizer(symbolizer);
  freeSymbol(symbol);   
}

void  test_get_symbol_given_symbolizer_with_shift_left_operator_expect_operator_symbol_returned(){
  Tokenizer *tokenizer = NULL;
  tokenizer = createTokenizer("  << = ");  
  Symbolizer  *symbolizer = createSymbolizer(tokenizer);

  Symbol  *symbol = getSymbol(symbolizer);
  
  TEST_ASSERT_EQUAL_STRING("<", symbol->token->str);
  TEST_ASSERT_EQUAL(SHIFT_LEFT, symbol->id);
  
  freeSymbolizer(symbolizer);
  freeSymbol(symbol);   
}

void  test_get_symbol_given_symbolizer_with_integer_expect_integer_symbol_returned(){
  Tokenizer *tokenizer = NULL;
  tokenizer = createTokenizer("  6767 ");  
  Symbolizer  *symbolizer = createSymbolizer(tokenizer);

  Symbol  *symbol = getSymbol(symbolizer);
  
  TEST_ASSERT_EQUAL_STRING("6767", symbol->token->str);
  TEST_ASSERT_EQUAL(INTEGER, symbol->id);
  
  freeSymbolizer(symbolizer);
  freeSymbol(symbol);   
}

void  test_get_symbol_given_symbolizer_with_float_expect_integer_symbol_returned(){
  Tokenizer *tokenizer = NULL;
  tokenizer = createTokenizer("    12.9876 ");  
  Symbolizer  *symbolizer = createSymbolizer(tokenizer);

  Symbol  *symbol = getSymbol(symbolizer);
  
  TEST_ASSERT_EQUAL_STRING("12.9876", symbol->token->str);
  TEST_ASSERT_EQUAL(DOUBLE, symbol->id);
  
  freeSymbolizer(symbolizer);
  freeSymbol(symbol);   
}
/*
void  test_extractOperatorFromToken_given_single_character_operator_expect_single_operator_returned(){
  Tokenizer *tokenizer = NULL;
  tokenizer = createTokenizer("  <  ");  
  Token *token = getToken(tokenizer);
  
  Operator  *result = extractOperatorFromToken(token, tokenizer);
  
  TEST_ASSERT_EQUAL_STRING("<", result->str);
  TEST_ASSERT_EQUAL(LESSER, result->id);
  
  freeTokenizer(tokenizer);
}

void  test_extractOperatorFromToken_given_double_same_character_operator_expect_double_operator_returned(){
  Tokenizer *tokenizer = NULL;
  tokenizer = createTokenizer("  ++  ");  
  Token *token = getToken(tokenizer);
  
  Operator  *result = extractOperatorFromToken(token, tokenizer);
  
  TEST_ASSERT_EQUAL_STRING("++", result->str);
  TEST_ASSERT_EQUAL(INC, result->id);
  
  freeTokenizer(tokenizer);
}

void  test_extractOperatorFromToken_given_double_different_character_operator_expect_double_operator_returned(){
  Tokenizer *tokenizer = NULL;
  tokenizer = createTokenizer("  -=  ");  
  Token *token = getToken(tokenizer);
  
  Operator  *result = extractOperatorFromToken(token, tokenizer);
  
  TEST_ASSERT_EQUAL_STRING("-=", result->str);
  TEST_ASSERT_EQUAL(MINUS_ASSIGN, result->id);
  
  freeTokenizer(tokenizer);
}

void  test_extractOperatorFromToken_given_double_not_vaild_operator_expect_single_operator_returned(){
  Tokenizer *tokenizer = NULL;
  tokenizer = createTokenizer("  +~  ");  
  Token *token = getToken(tokenizer);
  
  Operator  *result = extractOperatorFromToken(token, tokenizer);
  
  TEST_ASSERT_EQUAL_STRING("+", result->str);
  TEST_ASSERT_EQUAL(ADD, result->id);
  
  freeTokenizer(tokenizer);
}
void  test_extractOperatorFromToken_given_bitwise_not_expect_single_operator_returned(){
  Tokenizer *tokenizer = NULL;
  tokenizer = createTokenizer("       ~  ");  
  Token *token = getToken(tokenizer);
  
  Operator  *result = extractOperatorFromToken(token, tokenizer);
  
  TEST_ASSERT_EQUAL_STRING("~", result->str);
  TEST_ASSERT_EQUAL(BITWISE_NOT, result->id);
  
  freeTokenizer(tokenizer);
}
*/