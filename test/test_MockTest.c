#include "unity.h"
#include "FreeListItemWithSymbol.h"
#include "ExpressionParser.h"
#include "OperatorTable.h"
#include "DoubleLinkedList.h"
#include "Tokenizer.h"
#include "Token.h"
#include "mock_MemAlloc.h"
#include "Stack.h"
#include "ExcpetionThrowing.h"
#include "Exception.h"
#include "CException.h"
#include "TokenizerExceptionThrowing.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void  test_createSymbol(){
  Token  token = {NULL, NULL, 0, 0, TOKEN_INTEGER_TYPE};
  Symbol  symbol = {NULL, EMPTY, _NULL, NONE};
  
  memAlloc_ExpectAndReturn(sizeof(Symbol), &symbol);
  Symbol  *symbolptr = createSymbol(&token, NUMBER, INTEGER);
  TEST_ASSERT_EQUAL_PTR(&symbol, symbolptr);
  TEST_ASSERT_EQUAL(&token, symbolptr->token);
}

void  test_freeSymbol(){
  char  *str = "12";
  Token  token = {NULL, str, 0, 0, TOKEN_INTEGER_TYPE};
  Symbol  symbol = {&token, EMPTY, _NULL, NONE};
  memFree_Expect(token.str);
  memFree_Expect(&token);
  memFree_Expect(&symbol);
  freeSymbol(&symbol);
}

void  test_createSymbolizer(){
  Tokenizer tokenizer = {NULL, 0, 0, 0, 0, NULL};
  Token  token = {NULL, NULL, 0, 0, TOKEN_INTEGER_TYPE};
  Symbol  symbol = {NULL, OPERAND, MULTIPLY, INFIX};
  Symbolizer  symbolizer = {NULL, NULL};
  
  memAlloc_ExpectAndReturn(sizeof(Symbolizer), &symbolizer);
  memAlloc_ExpectAndReturn(sizeof(Token), &token);
  memAlloc_ExpectAndReturn(sizeof(Symbol), &symbol);
  Symbolizer  *symbolizerptr = createSymbolizer(&tokenizer);
  TEST_ASSERT_EQUAL_PTR(&tokenizer, symbolizer.tokenizer);
  TEST_ASSERT_EQUAL_PTR(&symbol, symbolizer.lastSymbol);
  TEST_ASSERT_EQUAL_PTR(&token, symbolizer.lastSymbol->token);
}