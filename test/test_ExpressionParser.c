#include "unity.h"
#include "Exception.h"
#include "CException.h"
#include "Stack.h"
#include "MemAlloc.h"
#include "ListItem.h"
#include "DoubleLinkedList.h"
#include "Token.h"
#include "Tokenizer.h"
#include "TokenizerExceptionThrowing.h"
#include  "Data.h"
#include "ExpressionParser.h"

CEXCEPTION_T  ex;

void setUp(void)
{
}

void tearDown(void)
{
}

void  test_checkOperator1PrecedenceGreater_given_2_tokens_token_1_higher_precedence_expect_1_returned(){
  Token *token1 = (Token *)createOperatorToken("~", 0, "~   !", TOKEN_OPERATOR_TYPE);
  Token *token2 = (Token *)createOperatorToken("+", 0, "+   !", TOKEN_OPERATOR_TYPE);
  int precedence = checkOperator1PrecedenceGreater(token1, token2);
  TEST_ASSERT_EQUAL(1, precedence);
}

void  test_checkOperator1PrecedenceGreater_given_2_tokens_token_2_higher_precedence_expect_0_returned(){
  Token *token1 = (Token *)createOperatorToken("%", 0, "~   !", TOKEN_OPERATOR_TYPE);
  Token *token2 = (Token *)createOperatorToken("(", 0, "+   !", TOKEN_OPERATOR_TYPE);
  int precedence = checkOperator1PrecedenceGreater(token1, token2);
  TEST_ASSERT_EQUAL(0, precedence);
}

void  test_extractNumberFromToken_given_1_IntToken_expect_correct_number_with_integer_type_returned(){
  Token *token1 = (Token *)createIntToken(999, 0, "999", "999",TOKEN_INTEGER_TYPE);
  Number  *extractedNumber = NULL;
  int *intPointer;
  extractedNumber = extractNumberFromToken(token1);
  intPointer = extractedNumber->value;
  TEST_ASSERT_EQUAL(INTEGER_NUMBER, extractedNumber->type);
  TEST_ASSERT_EQUAL(999, *intPointer);
}

void  test_extractNumberFromToken_given_1_FloatToken_expect_correct_number_with_float_type_returned(){
  Token *token1 = (Token *)createFloatToken(39.6666, 0, "39.6666", "39.6666",TOKEN_FLOAT_TYPE);
  Number  *extractedNumber = NULL;
  double *floatPointer;
  extractedNumber = extractNumberFromToken(token1);
  floatPointer = extractedNumber->value;
  TEST_ASSERT_EQUAL(FLOAT_NUMBER, extractedNumber->type);
  TEST_ASSERT_EQUAL_FLOAT(39.6666, *floatPointer);
  freeNumber(extractedNumber);
}

void  test_extractOperatorFromToken_given_1_OpToken_expect_correct_operator_returned_precedence_not_yet_stored(){
  Token *token1 = (Token *)createOperatorToken("+", 0, "+++",TOKEN_OPERATOR_TYPE);
  Operator  *extractedOperator = NULL;
  extractedOperator = extractOperatorFromToken(token1);
  TEST_ASSERT_EQUAL("+", extractedOperator->str);
  freeOperator(extractedOperator);
}

void  test_getItemDataInteger_n_getItemDataType_macro_with_listItem_pointing_to_number_expect_value_and_type_able_to_extract(){
  int *integer = malloc(sizeof(int));
  *integer = 67;
  Number  *number = createNumber((void  *)integer, INTEGER_NUMBER);
  ListItem  *itemWithNumber = linkedListCreateListItem(number);
  TEST_ASSERT_EQUAL(67, getItemDataInteger(itemWithNumber));
  TEST_ASSERT_EQUAL(INTEGER_NUMBER, getItemDataType(itemWithNumber));
  freeNumber(number);
  linkedListFreeListItem(itemWithNumber);
}

void  test_getItemDataFloat_n_getItemDataType_macro_with_listItem_pointing_to_number_expect_value_and_type_able_to_extract(){
  double *floatVal = malloc(sizeof(double));
  *floatVal = 67.6666;
  Number  *number = createNumber((void  *)floatVal, FLOAT_NUMBER);
  ListItem  *itemWithNumber = linkedListCreateListItem(number);
  TEST_ASSERT_EQUAL_FLOAT(67.6666, getItemDataFloat(itemWithNumber));
  TEST_ASSERT_EQUAL(FLOAT_NUMBER, getItemDataType(itemWithNumber));
  freeNumber(number);
  linkedListFreeListItem(itemWithNumber);
}

void  test_infixAddition_given_2_integer_45_n_11_expect_result_with_56_returned(){
  int *num1 = malloc(sizeof(int));
  *num1 = 45;
  int *num2 = malloc(sizeof(int));
  *num2 = 11;  
  Number  *number1 = createNumber((void  *)num1, INTEGER_NUMBER);
  Number  *number2 = createNumber((void  *)num2, INTEGER_NUMBER);
  StackStruct *stack = createStack();
  
  pushToStack(stack, (void  *)number2);
  
  pushToStack(stack, (void  *)number1);
  
  infixAddition(stack);
  
  ListItem  *peekItem = peekTopOfStack(stack);
  
  TEST_ASSERT_EQUAL(56, getItemDataInteger(peekItem));
  TEST_ASSERT_EQUAL(INTEGER_NUMBER, getItemDataType(peekItem));
  
  freeNumber(number1);
  freeNumber(number2);
  
  freeStack(stack, free);
}

/*
void  test_infixSubtraction_given_2_integer_tokens_88_and_23_expect_resultToken_with_65_returned(){
  Tokenizer *tokenizer = NULL;
  StackStruct *operandStack = createStack();
  tokenizer = createTokenizer("  88 23"); 
  IntegerToken *token0 = NULL;
  Try{
    token0 = (IntegerToken  *)getToken(tokenizer);
  
    IntegerToken *token1 = NULL;
    token1 = (IntegerToken  *)getToken(tokenizer);
    
    pushToStack(operandStack, (void  *)token0);
    
    pushToStack(operandStack, (void  *)token1);
    
    TEST_ASSERT_EQUAL(2, operandStack->size);
    
    infixSubtraction(operandStack);
    TEST_ASSERT_EQUAL(1, operandStack->size);
    IntegerToken *testIntToken = operandStack->list->head->data;
    TEST_ASSERT_EQUAL(TOKEN_INTEGER_TYPE, testIntToken->type);
    TEST_ASSERT_EQUAL_STRING("65", testIntToken->str);
    
    freeStack(operandStack, freeToken);
    freeTokenizer(tokenizer);
  }Catch(ex){
    dumpTokenErrorMessage(ex, 1);
    TEST_FAIL_MESSAGE("Do not expect any exception to be thrown.");     
  }  
}
*/

/*
void  test_infixSubtraction_given_2_integer_tokens_67_and_99_expect_resultToken_with_negative_32_returned(){
  Tokenizer *tokenizer = NULL;
  StackStruct *operandStack = createStack();
  tokenizer = createTokenizer("67      99"); 
  IntegerToken *token0 = NULL;
  Try{
    token0 = (IntegerToken  *)getToken(tokenizer);
  
    IntegerToken *token1 = NULL;
    token1 = (IntegerToken  *)getToken(tokenizer);
    
    pushToStack(operandStack, (void  *)token0);
    
    pushToStack(operandStack, (void  *)token1);
    
    TEST_ASSERT_EQUAL(2, operandStack->size);
    
    infixSubtraction(operandStack);
    TEST_ASSERT_EQUAL(1, operandStack->size);
    IntegerToken *testOpToken = operandStack->list->head->data;
    TEST_ASSERT_EQUAL(TOKEN_OPERATOR_TYPE, testOpToken->type);
    TEST_ASSERT_EQUAL_STRING("-", testOpToken->str);
    
    TEST_ASSERT_EQUAL(1, operandStack->size);
    IntegerToken *testIntToken = operandStack->list->head->data;
    TEST_ASSERT_EQUAL(TOKEN_INTEGER_TYPE, testIntToken->type);
    ("65", testIntToken->str);
    
    freeStack(operandStack, freeToken);
    freeTokenizer(tokenizer);
  }Catch(ex){
    dumpTokenErrorMessage(ex, 1);
    TEST_FAIL_MESSAGE("Do not expect any exception to be thrown.");     
  }  
}
*/