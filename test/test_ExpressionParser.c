#include "unity.h"

#include "ExpressionParser.h"
#include "OperatorTable.h"
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
#include  "Arity.h"

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

/*
void  test_extractOperatorFromToken_given_1_OpToken_expect_correct_operator_returned_precedence_not_yet_stored(){
  Token *token1 = (Token *)createOperatorToken("+", 0, "+++",TOKEN_OPERATOR_TYPE);
  Operator  *extractedOperator = NULL;
  extractedOperator = extractOperatorFromToken(token1, 3, BINARY);
  TEST_ASSERT_EQUAL_STRING("+", extractedOperator->str);
  TEST_ASSERT_EQUAL(3, extractedOperator->precedence);
  TEST_ASSERT_EQUAL(BINARY, extractedOperator->arity);
  freeOperator(extractedOperator);
}
*/

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
  
  Integer  *result = (Integer *)infixAddition((Number *)number1, (Number *)number2);
  
  TEST_ASSERT_EQUAL(56, result->value);
  TEST_ASSERT_EQUAL(INTEGER_NUMBER, result->type);
  
  free(number1);
  free(number2);
 
}

void  test_infixAddition_given_2_floating_pt_56point555_n_11point683_expect_result_with_68point238_returned(){
  Double  *number1 = createDouble(56.555);
  Double  *number2 = createDouble(11.683);
  
  Double  *result = (Double *)infixAddition((Number *)number1, (Number *)number2);
  
  TEST_ASSERT_EQUAL_FLOAT(68.238, result->value);
  TEST_ASSERT_EQUAL(FLOAT_NUMBER, result->type);
  
  free(number1);
  free(number2);
  
}

void  test_infixAddition_given_1_floating_pt_1_int_39point18_n_90_expect_result_with_129point18_returned(){
  Integer  *number1 = createInteger(90);
  Double  *number2 = createDouble(39.18);

  Double  *result = (Double *)infixAddition((Number *)number1, (Number *)number2);
  
  TEST_ASSERT_EQUAL_FLOAT(129.18, result->value);
  TEST_ASSERT_EQUAL(FLOAT_NUMBER, result->type);
  
  free(number1);
  free(number2);
}

void  test_infixSubtraction_given_2_integer688_n_300_expect_result_with_388_returned(){
  Integer  *number1 = createInteger(688);
  Integer  *number2 = createInteger(300);
  
  Integer *result = (Integer  *)infixSubtraction((Number *)number1, (Number *)number2);

  TEST_ASSERT_EQUAL(388, result->value);
  TEST_ASSERT_EQUAL(INTEGER_NUMBER, result->type);
  
  free(number1);
  free(number2);
}

void  test_infixSubtraction_given_2_integer__67_n_99_expect_result_with_negative_32_returned(){
  Integer  *number1 = createInteger(67);
  Integer  *number2 = createInteger(99);
  
  Integer *result = (Integer  *)infixSubtraction((Number *)number1, (Number *)number2);

  TEST_ASSERT_EQUAL(-32, result->value);
  TEST_ASSERT_EQUAL(INTEGER_NUMBER, result->type);
  
  free(number1);
  free(number2);
}

void  test_infixSubtraction_given_2_floating_pt_1point567_n_0point632_expect_result_with_0point935_returned(){
  Double  *number1 = createDouble(1.567);
  Double  *number2 = createDouble(0.632);

  Double  *result = (Double  *)infixSubtraction((Number *)number1, (Number *)number2);
  
  TEST_ASSERT_EQUAL_FLOAT(0.935, result->value);
  TEST_ASSERT_EQUAL(FLOAT_NUMBER, result->type);
  
  free(number1);
  free(number2);
}

void  test_infixMultiplication_given_2_integer__8_n_12_expect_result_with_96_returned(){
  Integer  *number1 = createInteger(8);
  Integer  *number2 = createInteger(12);

  Integer *result = (Integer *)infixMultiplication((Number *)number1, (Number *)number2);

  TEST_ASSERT_EQUAL(96, result->value);
  TEST_ASSERT_EQUAL(INTEGER_NUMBER, result->type);
  
  free(number1);
  free(number2);
}

void  test_infixMultiplication_given_2_floating_pt_12point67_n_3point1_expect_result_39point277_returned(){
  Double  *number1 = createDouble(12.67);
  Double  *number2 = createDouble(39.277);
  
  Double *result = (Double *)infixMultiplication((Number *)number1, (Number *)number2);

  TEST_ASSERT_EQUAL_FLOAT(497.63959, result->value);
  TEST_ASSERT_EQUAL(FLOAT_NUMBER, result->type);
  
  free(number1);
  free(number2);
}

void  test_infixDivision_given_2_integer_144_n_12_expect_result_with_12_returned(){
  Integer  *number1 = createInteger(144);
  Integer  *number2 = createInteger(12);

  Integer *result = (Integer  *)infixDivision((Number *)number1, (Number *)number2);

  TEST_ASSERT_EQUAL(12, result->value);
  TEST_ASSERT_EQUAL(INTEGER_NUMBER, result->type);
  
  free(number1);
  free(number2);
}

void  test_infixDivision_given_2_floating_pt_4point375_n_1point25_expect_result_with_3point5_returned(){
  Double  *number1 = createDouble(4.375);
  Double  *number2 = createDouble(1.25);

  Double *result = (Double  *)infixDivision((Number *)number1, (Number *)number2);

  TEST_ASSERT_EQUAL_FLOAT(3.5, result->value);
  TEST_ASSERT_EQUAL(FLOAT_NUMBER, result->type);
  
  free(number1);
  free(number2);
}

void  test_infixDivision_given_1_floating_pt_1_int_6point333_n_3_expect_result_with_2point111_returned(){
  Double  *number1 = createDouble(6.333);
  Integer  *number2 = createInteger(3);

  Double *result = (Double  *)infixDivision((Number *)number1, (Number *)number2);

  TEST_ASSERT_EQUAL_FLOAT(2.111, result->value);
  TEST_ASSERT_EQUAL(FLOAT_NUMBER, result->type);
  
  free(number1);
  free(number2);
}


void  test_handleInfix_given_two_operand_Integers_1_infix_add_expect_the_values_able_to_be_returned(){
  Integer *number1 = createInteger(13);
  Integer  *number2 = createInteger(77);
  
  Operator  *operator = createOperator("+", 4, BINARY);
  
  StackStruct *operandStack = createStack();
  StackStruct *operatorStack = createStack();
  
  pushToStack(operandStack, (void*)number1);
  pushToStack(operandStack, (void*)number2);
  pushToStack(operatorStack, (void*)operator);
  
  handleInfix(operandStack, operatorStack);
  
  ListItem  *peekItem = peekTopOfStack(operandStack);
  
  TEST_ASSERT_EQUAL(90, getItemInteger(peekItem));
  TEST_ASSERT_EQUAL(INTEGER_NUMBER, getItemDataType(peekItem));
  
  freeStack(operandStack, free); 
  freeStack(operatorStack, free); 
}

void  test_handleInfix_given_two_operand_Integer_Double_1_infix_mult_expect_the_values_able_to_be_returned(){
  Integer *number1 = createInteger(34);
  Double  *number2 = createDouble(4.888);
  
  Operator  *operator = createOperator("*", 3, BINARY);
  
  StackStruct *operandStack = createStack();
  StackStruct *operatorStack = createStack();
  
  pushToStack(operandStack, (void*)number1);
  pushToStack(operandStack, (void*)number2);
  pushToStack(operatorStack, (void*)operator);
  
  handleInfix(operandStack, operatorStack);
  
  ListItem  *peekItem = peekTopOfStack(operandStack);
  
  TEST_ASSERT_EQUAL_FLOAT(166.192, getItemDouble(peekItem));
  TEST_ASSERT_EQUAL(FLOAT_NUMBER, getItemDataType(peekItem));
  
  freeStack(operandStack, free); 
  freeStack(operatorStack, free); 
}

void  test_handleInfix_given_two_operand_Doubles_1_infix_division_expect_the_values_able_to_be_returned(){
  Double  *number1 = createDouble(9.57434);
  Double  *number2 = createDouble(4.888);
  
  Operator  *operator = createOperator("/", 3, BINARY);
  
  StackStruct *operandStack = createStack();
  StackStruct *operatorStack = createStack();
  
  pushToStack(operandStack, (void*)number1);
  pushToStack(operandStack, (void*)number2);
  pushToStack(operatorStack, (void*)operator);
  
  handleInfix(operandStack, operatorStack);
  
  ListItem  *peekItem = peekTopOfStack(operandStack);
  
  TEST_ASSERT_EQUAL_FLOAT(1.958743863, getItemDouble(peekItem));
  TEST_ASSERT_EQUAL(FLOAT_NUMBER, getItemDataType(peekItem));
  
  freeStack(operandStack, free); 
  freeStack(operatorStack, free); 
}

/*
void  test_unwindStack_given_2_operands_1_lower_precedence_operator_inside_operator_stack_expect_currentOperator_pushed_into_operator_stack(){
  Integer *number1 = createInteger(13);
  Double  *number2 = createDouble(54.452);

  Operator  *currentOperator = createOperator("+", 4, BINARY);
  Operator  *operator = createOperator("&", 8, UNARY);
  
  StackStruct *operandStack = createStack();
  StackStruct *operatorStack = createStack();
  
  pushToStack(operandStack, (void*)number1);
  pushToStack(operandStack, (void*)number2);
  pushToStack(operatorStack, (void*)operator);
  
  unwindStack(operatorStack, operandStack, currentOperator);
  
  ListItem  *peekItem = peekTopOfStack(operatorStack);
  
  TEST_ASSERT_EQUAL_STRING("+", ((Operator  *)(*peekItem).data)->str);
  
  //free(number1);                    //Since no arithmetic is performed, the numbers still belong to stack
  //free(number2);                    //Thus let freeStack do the number1 and number2 freeing
  //free(currentOperator);
  
  freeStack(operandStack, free); 
  freeStack(operatorStack, free); 
}
void  test_unwindStack_given_2_operands_1_higher_precedence_operator_inside_operator_stack_expect_higher_operator_computed(){
  Integer *number1 = createInteger(13);
  Integer  *number2 = createInteger(3);

  Operator  *currentOperator = createOperator("+", 4, BINARY);
  Operator  *operator = createOperator("*", 3, UNARY);
  
  StackStruct *operandStack = createStack();
  StackStruct *operatorStack = createStack();
  
  pushToStack(operandStack, (void*)number1);
  pushToStack(operandStack, (void*)number2);
  pushToStack(operatorStack, (void*)operator);
  
  unwindStack(operatorStack, operandStack, currentOperator);
  
  ListItem  *peekItem = peekTopOfStack(operatorStack);
  
  TEST_ASSERT_EQUAL_STRING("+", ((Operator  *)(*peekItem).data)->str);
  peekItem = peekTopOfStack(operandStack);
  TEST_ASSERT_EQUAL(39, getItemInteger(peekItem));
  TEST_ASSERT_EQUAL(INTEGER_NUMBER, getItemDataType(peekItem));
  
  free(number1);                    
  free(number2);                      
  //free(currentOperator);        //This operator is inside the operator stack, thus don't free it
  
  freeStack(operandStack, free); 
  freeStack(operatorStack, free); 
}

void  test_unwindStack_given_2_operators_1_higher_precedence_operator_inside_operator_stack_expect_higher_operator_computed(){
  Integer *number1 = createInteger(13);
  Integer  *number2 = createInteger(3);
  Integer  *number3 = createInteger(6);
  Integer  *number4 = createInteger(8);

  Operator  *currentOperator = createOperator("+", 4, BINARY);
  Operator  *operator1 = createOperator("-", 3, UNARY);
  Operator  *operator2 = createOperator("*", 3, UNARY);
  
  StackStruct *operandStack = createStack();
  StackStruct *operatorStack = createStack();
  
  pushToStack(operandStack, (void*)number1);
  pushToStack(operandStack, (void*)number2);
  pushToStack(operandStack, (void*)number3);
  pushToStack(operandStack, (void*)number4);
  pushToStack(operatorStack, (void*)operator1);
  pushToStack(operatorStack, (void*)operator2);
  
  unwindStack(operatorStack, operandStack, currentOperator);
  
  ListItem  *peekItem = peekTopOfStack(operatorStack);
  
  TEST_ASSERT_EQUAL_STRING("+", ((Operator  *)(*peekItem).data)->str);
  peekItem = peekTopOfStack(operandStack);
  TEST_ASSERT_EQUAL(-45, getItemInteger(peekItem));
  TEST_ASSERT_EQUAL(INTEGER_NUMBER, getItemDataType(peekItem));

  
  freeStack(operandStack, free); 
  freeStack(operatorStack, free); 
}
*/