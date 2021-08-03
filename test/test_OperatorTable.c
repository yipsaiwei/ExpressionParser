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

void  test_returnDoubleCharacterOperator_given_2_adjacent_token_expect_correct_double_char_operator_returned(){
  Tokenizer *tokenizer = NULL;
  tokenizer = createTokenizer("  ++  ");  
  Token *token1 = getToken(tokenizer);
  Token *token2 = getToken(tokenizer);
  
  Operator  *result = returnDoubleCharacterOperator(token1, token2);
  
  TEST_ASSERT_EQUAL_STRING("++", result->str);
  TEST_ASSERT_EQUAL(INC, result->id);
}

void  test_returnDoubleCharacterOperator_given_2_adjacent_different_token_expect_correct_double_char_operator_returned(){
  Tokenizer *tokenizer = NULL;
  tokenizer = createTokenizer("  <=  ");  
  Token *token1 = getToken(tokenizer);
  Token *token2 = getToken(tokenizer);
  
  Operator  *result = returnDoubleCharacterOperator(token1, token2);
  
  TEST_ASSERT_EQUAL_STRING("<=", result->str);
  TEST_ASSERT_EQUAL(LESSER_EQ, result->id);
}

void  test_returnDoubleCharacterOperator_given_2_operators_separated_by_space_expect_exception_thrown(){
  Tokenizer *tokenizer = NULL;
  tokenizer = createTokenizer("  < =  ");  
  Token *token1 = getToken(tokenizer);
  Token *token2 = getToken(tokenizer);

  Try{
    Operator  *result = returnDoubleCharacterOperator(token1, token2);
    TEST_FAIL_MESSAGE("EXPECT ERROR_INVALID_STRING_to_be_thrown, BUT UNRECEIVED");
  }Catch(ex){
      dumpTokenErrorMessage(ex, 1);
      TEST_ASSERT_EQUAL(ERROR_INVALID_OPERATOR, ex->errorCode);
      freeException(ex);
    }  
}
