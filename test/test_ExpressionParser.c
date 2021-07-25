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

CEXCEPTION_T  ex;

void setUp(void)
{
}

void tearDown(void)
{
}

void  test_extractIntegerFromToken_given_1_IntToken_expect_correct_number_with_integer_type_returned(){
  Token *token1 = (Token *)createIntToken(999, 0, "999", "999", TOKEN_INTEGER_TYPE);
  Integer *integer = extractIntegerFromToken(token1);
  TEST_ASSERT_EQUAL(INTEGER_NUMBER, integer->type);
  TEST_ASSERT_EQUAL(999, integer->value);
  free(token1);
  free(integer);
}

void  test_extractNumberFromToken_given_1_FloatToken_expect_correct_number_with_float_type_returned(){
  Token *token1 = (Token *)createFloatToken(39.6666, 0, "39.6666", "39.6666", TOKEN_FLOAT_TYPE);
  Double *floatingPt = extractFloatingPointFromToken(token1);
  TEST_ASSERT_EQUAL(FLOAT_NUMBER, floatingPt->type);
  TEST_ASSERT_EQUAL_FLOAT(39.6666, floatingPt->value);
  free(token1);
  free(floatingPt);
}

void  test_extractOperatorFromToken_given_1_OpToken_expect_correct_operator_returned_precedence_not_yet_stored(){
  Token *token1 = (Token *)createOperatorToken("+", 0, "+++",TOKEN_OPERATOR_TYPE);
  Operator  *extractedOperator = NULL;
  extractedOperator = extractOperatorFromToken(token1);
  TEST_ASSERT_EQUAL("+", extractedOperator->str);
  freeOperator(extractedOperator);
}

void  test_getItemDataInteger_n_getItemDataType_macro_with_listItem_pointing_to_number_expect_value_and_type_able_to_extract(){
  Integer *integer = createInteger(12);
  ListItem  *itemWithNumber = linkedListCreateListItem(integer);
  TEST_ASSERT_EQUAL(12, getItemInteger(itemWithNumber));
  TEST_ASSERT_EQUAL(INTEGER_NUMBER, getItemDataType(itemWithNumber));
  free(integer);
  linkedListFreeListItem(itemWithNumber);
}

void  test_createDouble_n_getItemDouble_macro_with_listItem_pointing_to_number_expect_value_and_type_able_to_extract(){
  Double  *floatVal = createDouble(67.6666);
  ListItem  *itemWithNumber = linkedListCreateListItem(floatVal);
  TEST_ASSERT_EQUAL_FLOAT(67.6666, getItemDouble(itemWithNumber));
  TEST_ASSERT_EQUAL(FLOAT_NUMBER, getItemDataType(itemWithNumber));
  free(floatVal);
  linkedListFreeListItem(itemWithNumber);
}

void  test_infixAddition_given_2_integer_45_n_11_expect_result_with_56_returned(){
  Integer  *number1 = createInteger(45);
  Integer  *number2 = createInteger(11);
  StackStruct *stack = createStack();
  
  pushToStack(stack, (void  *)number2);
  
  pushToStack(stack, (void  *)number1);
  
  infixAddition(stack);
  
  ListItem  *peekItem = peekTopOfStack(stack);
  
  TEST_ASSERT_EQUAL(56, getItemInteger(peekItem));
  TEST_ASSERT_EQUAL(INTEGER_NUMBER, getItemDataType(peekItem));
  
  free(number1);
  free(number2);
  
  freeStack(stack, free);
}

void  test_infixAddition_given_2_floating_pt_56point555_n_11point683_expect_result_with_68point238_returned(){
  Double  *number1 = createDouble(56.555);
  Double  *number2 = createDouble(11.683);

  StackStruct *stack = createStack();
  
  pushToStack(stack, (void  *)number2);
  
  pushToStack(stack, (void  *)number1);
  
  infixAddition(stack);
  
  ListItem  *peekItem = peekTopOfStack(stack);
  
  TEST_ASSERT_EQUAL_FLOAT(68.238, getItemDouble(peekItem));
  TEST_ASSERT_EQUAL(FLOAT_NUMBER, getItemDataType(peekItem));
  
  free(number1);
  free(number2);
  
  freeStack(stack, free);
}

void  test_infixAddition_given_1_floating_pt_1_int_39point18_n_90_expect_result_with_129point18_returned(){
  Integer  *number1 = createInteger(90);
  Double  *number2 = createDouble(39.18);

  StackStruct *stack = createStack();
  
  pushToStack(stack, (void  *)number2);
  
  pushToStack(stack, (void  *)number1);
  
  infixAddition(stack);
  
  ListItem  *peekItem = peekTopOfStack(stack);
  
  TEST_ASSERT_EQUAL_FLOAT(129.18, getItemDouble(peekItem));
  TEST_ASSERT_EQUAL(FLOAT_NUMBER, getItemDataType(peekItem));
  
  free(number1);
  free(number2);
  
  freeStack(stack, free);
}

void  test_infixSubtraction_given_2_integer688_n_300_expect_result_with_388_returned(){
  Integer  *number1 = createInteger(688);
  Integer  *number2 = createInteger(300);
  StackStruct *stack = createStack();
  
  pushToStack(stack, (void  *)number1);
  
  pushToStack(stack, (void  *)number2);
  
  infixSubtraction(stack);
  
  ListItem  *peekItem = peekTopOfStack(stack);
  
  TEST_ASSERT_EQUAL(388, getItemInteger(peekItem));
  TEST_ASSERT_EQUAL(INTEGER_NUMBER, getItemDataType(peekItem));
  
  free(number1);
  free(number2);
  
  freeStack(stack, free); 
}

void  test_infixSubtraction_given_2_integer__67_n_99_expect_result_with_negative_32_returned(){
  Integer  *number1 = createInteger(67);
  Integer  *number2 = createInteger(99);
  StackStruct *stack = createStack();
  
  pushToStack(stack, (void  *)number1);
  
  pushToStack(stack, (void  *)number2);
  
  infixSubtraction(stack);
  
  ListItem  *peekItem = peekTopOfStack(stack);
  
  TEST_ASSERT_EQUAL(-32, getItemInteger(peekItem));
  TEST_ASSERT_EQUAL(INTEGER_NUMBER, getItemDataType(peekItem));
  
  free(number1);
  free(number2);
  
  freeStack(stack, free); 
}

void  test_infixSubtraction_given_2_floating_pt_1point567_n_0point632_expect_result_with_0point935_returned(){
  Double  *number1 = createDouble(1.567);
  Double  *number2 = createDouble(0.632);
  StackStruct *stack = createStack();
  
  pushToStack(stack, (void  *)number1);
  
  pushToStack(stack, (void  *)number2);
  
  infixSubtraction(stack);
  
  ListItem  *peekItem = peekTopOfStack(stack);
  
  TEST_ASSERT_EQUAL_FLOAT(0.935, getItemDouble(peekItem));
  TEST_ASSERT_EQUAL(FLOAT_NUMBER, getItemDataType(peekItem));
  
  free(number1);
  free(number2);
  
  freeStack(stack, free); 
}

void  test_infixMultiplication_given_2_integer__8_n_12_expect_result_with_96_returned(){
  Integer  *number1 = createInteger(8);
  Integer  *number2 = createInteger(12);
  StackStruct *stack = createStack();
  
  pushToStack(stack, (void  *)number1);
  
  pushToStack(stack, (void  *)number2);
  
  infixMultiplication(stack);
  
  ListItem  *peekItem = peekTopOfStack(stack);
  
  TEST_ASSERT_EQUAL(96, getItemInteger(peekItem));
  TEST_ASSERT_EQUAL(INTEGER_NUMBER, getItemDataType(peekItem));
  
  free(number1);
  free(number2);
  
  freeStack(stack, free); 
}

void  test_infixMultiplication_given_2_floating_pt_12point67_n_3point1_expect_result_39point277_returned(){
  Double  *number1 = createDouble(12.67);
  Double  *number2 = createDouble(39.277);
  StackStruct *stack = createStack();
  
  pushToStack(stack, (void  *)number1);
  
  pushToStack(stack, (void  *)number2);
  
  infixMultiplication(stack);
  
  ListItem  *peekItem = peekTopOfStack(stack);
  
  TEST_ASSERT_EQUAL_FLOAT(497.63959, getItemDouble(peekItem));
  TEST_ASSERT_EQUAL(FLOAT_NUMBER, getItemDataType(peekItem));
  
  free(number1);
  free(number2);
  
  freeStack(stack, free); 
}

void  test_infixDivision_given_2_integer_144_n_12_expect_result_with_12_returned(){
  Integer  *number1 = createInteger(144);
  Integer  *number2 = createInteger(12);
  StackStruct *stack = createStack();
  
  pushToStack(stack, (void  *)number1);
  
  pushToStack(stack, (void  *)number2);
  
  infixDivision(stack);
  
  ListItem  *peekItem = peekTopOfStack(stack);
  
  TEST_ASSERT_EQUAL(12, getItemInteger(peekItem));
  TEST_ASSERT_EQUAL(INTEGER_NUMBER, getItemDataType(peekItem));
  
  free(number1);
  free(number2);
  
  freeStack(stack, free); 
}

void  test_infixDivision_given_2_floating_pt_4point375_n_1point25_expect_result_with_3point5_returned(){
  Double  *number1 = createDouble(4.375);
  Double  *number2 = createDouble(1.25);
  StackStruct *stack = createStack();
  
  pushToStack(stack, (void  *)number1);
  
  pushToStack(stack, (void  *)number2);
  
  infixDivision(stack);
  
  ListItem  *peekItem = peekTopOfStack(stack);
  
  TEST_ASSERT_EQUAL_FLOAT(3.5, getItemDouble(peekItem));
  TEST_ASSERT_EQUAL(FLOAT_NUMBER, getItemDataType(peekItem));
  
  free(number1);
  free(number2);
  
  freeStack(stack, free); 
}

void  test_infixDivision_given_1_floating_pt_1_int_6point333_n_3_expect_result_with_2point111_returned(){
  Double  *number1 = createDouble(6.333);
  Integer  *number2 = createInteger(3);
  StackStruct *stack = createStack();
  
  pushToStack(stack, (void  *)number1);
  
  pushToStack(stack, (void  *)number2);
  
  infixDivision(stack);
  
  ListItem  *peekItem = peekTopOfStack(stack);
  
  TEST_ASSERT_EQUAL_FLOAT(2.111, getItemDouble(peekItem));
  TEST_ASSERT_EQUAL(FLOAT_NUMBER, getItemDataType(peekItem));
  
  free(number1);
  free(number2);
  
  freeStack(stack, free); 
}