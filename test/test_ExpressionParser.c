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
#include "ExcpetionThrowing.h"
#include "FreeListItemWithSymbol.h"

CEXCEPTION_T  ex;

void setUp(void)
{
}

void tearDown(void)
{
}

void  test_ftoa_given_a_double_value_expect_it_converted_to_string(){
  double  number = (double)67.6666;
  char  *str = malloc(sizeof(char) * countDoubleDigitNumber(number, 6) + 2);
  
  ftoa(number, str, 4);
  
  TEST_ASSERT_EQUAL_STRING("67.6666", str);
  
  free(str);
}

void  test_ftoa_given_a_double_value_with_afterpoint_more_expect_it_converted_to_string(){
  double  number = (double)67.6666;
  int doubleDigit = countDoubleDigitNumber(number, 6);
  char  *str = malloc(sizeof(char) * (doubleDigit + 2));
  
  ftoa(number, str, 5);
  
  TEST_ASSERT_EQUAL_STRING("67.66660", str);
  
  free(str);
}

void  test_ftoa_given_a_double_value_with_afterpoint_less_expect_it_converted_to_string(){
  double  number = (double)12.1345523;
  char  *str = malloc(sizeof(char) * countDoubleDigitNumber(number, 6) + 2);
  
  ftoa(number, str, 4);
  
  TEST_ASSERT_EQUAL_STRING("12.1345", str);
  
  free(str);
}

void  test_createResultString_given_a_number_expect_correct_string_returned(){
  int number = 1304;
  
  char  *str = createResultString((void  *)&number, INTEGER);
  
  TEST_ASSERT_EQUAL_STRING("1304", str);
  
  free(str);
}

void  test_createResultString_given_a_doubl_expect_correct_string_returned(){
  double number = 18.946565377337;
  
  char  *str = createResultString((void  *)&number, DOUBLE);
  
  TEST_ASSERT_EQUAL_STRING("18.946565", str);
  
  free(str);
}

void  test_infixAddition_given_2_integer_45_n_11_expect_result_with_56_returned(){
  Tokenizer *tokenizer = createTokenizer("45+11");
  Token *operandToken1 = getToken(tokenizer);
  Token *operatorToken = getToken(tokenizer);
  Token *operandToken2 = getToken(tokenizer);
  
  Symbol  *number1 = createSymbol(operandToken1, OPERAND, INTEGER);
  Symbol  *number2 = createSymbol(operandToken2, OPERAND, INTEGER);
  
  Symbol  *result = infixAdd(number1, number2);
  
  TEST_ASSERT_EQUAL(56, getSymbolInteger(result));
  TEST_ASSERT_EQUAL(INTEGER, result->id);
  
  freeSymbol(number1);
  freeSymbol(number2);
  freeSymbol(result);
  freeToken(operatorToken);
  freeTokenizer(tokenizer);
}

void  test_infixAddition_given_2_floating_pt_56point555_n_11point683_expect_result_with_68point238_returned(){
  Tokenizer *tokenizer = createTokenizer("56.555 +   11.683");
  Token *operandToken1 = getToken(tokenizer);
  Token *operatorToken = getToken(tokenizer);
  Token *operandToken2 = getToken(tokenizer);

  Symbol  *number1 = createSymbol(operandToken1, OPERAND, DOUBLE);
  Symbol  *number2 = createSymbol(operandToken2, OPERAND, DOUBLE);  
  
  Symbol  *result = infixAdd(number1, number2);
  
  TEST_ASSERT_EQUAL_FLOAT(68.238, getSymbolDouble(result));
  TEST_ASSERT_EQUAL(DOUBLE, result->id);
  
  freeSymbol(number1);
  freeSymbol(number2);
  freeSymbol(result);
  freeToken(operatorToken);
  freeTokenizer(tokenizer);
}

void  test_infixAddition_given_1_floating_pt_1_int_39point18_n_90_expect_result_with_129point18_returned(){
  Tokenizer *tokenizer = createTokenizer("  90 +  39.18");
  Token *operandToken1 = getToken(tokenizer);
  Token *operatorToken = getToken(tokenizer);
  Token *operandToken2 = getToken(tokenizer);

  Symbol  *number1 = createSymbol(operandToken1, OPERAND, INTEGER);
  Symbol  *number2 = createSymbol(operandToken2, OPERAND, DOUBLE);  
  
  Symbol  *result = infixAdd(number1, number2);
  
  TEST_ASSERT_EQUAL_FLOAT(129.18, getSymbolDouble(result));
  TEST_ASSERT_EQUAL(DOUBLE, result->id);
  
  freeSymbol(number1);
  freeSymbol(number2);
  freeSymbol(result);
  freeToken(operatorToken);
  freeTokenizer(tokenizer);
}

void  test_infixSubtraction_given_2_integer688_n_300_expect_result_with_388_returned(){
  Tokenizer *tokenizer = createTokenizer("  688- 300");
  Token *operandToken1 = getToken(tokenizer);
  Token *operatorToken = getToken(tokenizer);
  Token *operandToken2 = getToken(tokenizer);
  
  Symbol  *number1 = createSymbol(operandToken1, OPERAND, INTEGER);
  Symbol  *number2 = createSymbol(operandToken2, OPERAND, INTEGER); 
  
  Symbol  *result = infixMinus(number1, number2);

  TEST_ASSERT_EQUAL(388, getSymbolInteger(result));

  TEST_ASSERT_EQUAL(INTEGER, result->id);
  
  freeSymbol(number1);
  freeSymbol(number2);
  freeSymbol(result);
  freeToken(operatorToken);
  freeTokenizer(tokenizer);
}

void  test_infixSubtraction_given_2_integer__67_n_99_expect_result_with_negative_32_returned(){
  Tokenizer *tokenizer = createTokenizer("  67   -99");
  Token *operandToken1 = getToken(tokenizer);
  Token *operatorToken = getToken(tokenizer);
  Token *operandToken2 = getToken(tokenizer);
  
  Symbol  *number1 = createSymbol(operandToken1, OPERAND, INTEGER);
  Symbol  *number2 = createSymbol(operandToken2, OPERAND, INTEGER); 
  
  Symbol  *result = infixMinus(number1, number2);

  TEST_ASSERT_EQUAL(-32, getSymbolInteger(result));

  TEST_ASSERT_EQUAL(INTEGER, result->id);
  
  freeSymbol(number1);
  freeSymbol(number2);
  freeSymbol(result);
  freeToken(operatorToken);
  freeTokenizer(tokenizer);
}

void  test_infixSubtraction_given_2_floating_pt_1point567_n_0point632_expect_result_with_0point935_returned(){
  Tokenizer *tokenizer = createTokenizer("  1.567   -  0.632");
  Token *operandToken1 = getToken(tokenizer);
  Token *operatorToken = getToken(tokenizer);
  Token *operandToken2 = getToken(tokenizer);
  
  Symbol  *number1 = createSymbol(operandToken1, OPERAND, DOUBLE);
  Symbol  *number2 = createSymbol(operandToken2, OPERAND, DOUBLE); 
  
  Symbol  *result = infixMinus(number1, number2);

  TEST_ASSERT_EQUAL(0.935, getSymbolDouble(result));

  TEST_ASSERT_EQUAL(DOUBLE, result->id);
  
  freeSymbol(number1);
  freeSymbol(number2);
  freeSymbol(result);
  freeToken(operatorToken);
  freeTokenizer(tokenizer);
}

void  test_infixSubtraction_given_2_floating_pt_5point6832_9point8_expect_result_with_negative__returned(){
  Tokenizer *tokenizer = createTokenizer("    5.6832-9.8");
  Token *operandToken1 = getToken(tokenizer);
  Token *operatorToken = getToken(tokenizer);
  Token *operandToken2 = getToken(tokenizer);
  
  Symbol  *number1 = createSymbol(operandToken1, OPERAND, DOUBLE);
  Symbol  *number2 = createSymbol(operandToken2, OPERAND, DOUBLE); 
  
  Symbol  *result = infixMinus(number1, number2);

  TEST_ASSERT_EQUAL(-4.1168, getSymbolDouble(result));

  TEST_ASSERT_EQUAL(DOUBLE, result->id);
  
  freeSymbol(number1);
  freeSymbol(number2);
  freeSymbol(result);
  freeToken(operatorToken);
  freeTokenizer(tokenizer);
}

void  test_infixMultiplication_given_2_integer__8_n_12_expect_result_with_96_returned(){
  Tokenizer *tokenizer = createTokenizer("  8*12");
  Token *operandToken1 = getToken(tokenizer);
  Token *operatorToken = getToken(tokenizer);
  Token *operandToken2 = getToken(tokenizer);

  Symbol  *number1 = createSymbol(operandToken1, OPERAND, INTEGER);
  Symbol  *number2 = createSymbol(operandToken2, OPERAND, INTEGER); 
  
  Symbol  *result = infixMultiply(number1, number2);

  TEST_ASSERT_EQUAL(96, getSymbolInteger(result));
  TEST_ASSERT_EQUAL(INTEGER, result->id);

  freeSymbol(number1);
  freeSymbol(number2);
  freeSymbol(result);
  freeToken(operatorToken);
  freeTokenizer(tokenizer);
}

void  test_infixMultiplication_given_2_floating_pt_12point67_n_3point1_expect_result_39point277_returned(){
  Tokenizer *tokenizer = createTokenizer("12.67  * 39.277");
  Token *operandToken1 = getToken(tokenizer);
  Token *operatorToken = getToken(tokenizer);
  Token *operandToken2 = getToken(tokenizer);

  Symbol  *number1 = createSymbol(operandToken1, OPERAND, DOUBLE);
  Symbol  *number2 = createSymbol(operandToken2, OPERAND, DOUBLE); 
  
  Symbol  *result = infixMultiply(number1, number2);

  TEST_ASSERT_EQUAL(497.63959, getSymbolDouble(result));
  TEST_ASSERT_EQUAL(DOUBLE, result->id);

  freeSymbol(number1);
  freeSymbol(number2);
  freeSymbol(result);
  freeToken(operatorToken);
  freeTokenizer(tokenizer);
}

void  test_infixDivision_given_2_integer_144_n_12_expect_result_with_12_returned(){
  Tokenizer *tokenizer = createTokenizer("144/12");
  Token *operandToken1 = getToken(tokenizer);
  Token *operatorToken = getToken(tokenizer);
  Token *operandToken2 = getToken(tokenizer);

  Symbol  *number1 = createSymbol(operandToken1, OPERAND, INTEGER);
  Symbol  *number2 = createSymbol(operandToken2, OPERAND, INTEGER); 

  Symbol  *result = infixDivide(number1, number2);

  TEST_ASSERT_EQUAL(12, getSymbolInteger(result));
  TEST_ASSERT_EQUAL(INTEGER, result->id);

  freeSymbol(number1);
  freeSymbol(number2);
  freeSymbol(result);
  freeToken(operatorToken);
  freeTokenizer(tokenizer);
}

void  test_infixDivision_given_2_floating_pt_4point375_n_1point25_expect_result_with_3point5_returned(){
  Tokenizer *tokenizer = createTokenizer("  4.375/ 1.25");
  Token *operandToken1 = getToken(tokenizer);
  Token *operatorToken = getToken(tokenizer);
  Token *operandToken2 = getToken(tokenizer);

  Symbol  *number1 = createSymbol(operandToken1, OPERAND, DOUBLE);
  Symbol  *number2 = createSymbol(operandToken2, OPERAND, DOUBLE); 

  Symbol  *result = infixDivide(number1, number2);

  TEST_ASSERT_EQUAL(3.5, getSymbolDouble(result));
  TEST_ASSERT_EQUAL(DOUBLE, result->id);

  freeSymbol(number1);
  freeSymbol(number2);
  freeSymbol(result);
  freeToken(operatorToken);
  freeTokenizer(tokenizer);
}

void  test_infixDivision_given_1_floating_pt_1_int_6point333_n_3_expect_result_with_2point111_returned(){
  Tokenizer *tokenizer = createTokenizer(" 6.333  / 3");
  Token *operandToken1 = getToken(tokenizer);
  Token *operatorToken = getToken(tokenizer);
  Token *operandToken2 = getToken(tokenizer);

  Symbol  *number1 = createSymbol(operandToken1, OPERAND, DOUBLE);
  Symbol  *number2 = createSymbol(operandToken2, OPERAND, INTEGER); 

  Symbol  *result = infixDivide(number1, number2);

  TEST_ASSERT_EQUAL(2.111, getSymbolDouble(result));
  TEST_ASSERT_EQUAL(DOUBLE, result->id);

  freeSymbol(number1);
  freeSymbol(number2);
  freeSymbol(result);
  freeToken(operatorToken);
  freeTokenizer(tokenizer);
}

void  test_infixDivision_given_2_int_expect_result_with_1point55_returned(){
  Tokenizer *tokenizer = createTokenizer(" 7/4");
  Token *operandToken1 = getToken(tokenizer);
  Token *operatorToken = getToken(tokenizer);
  Token *operandToken2 = getToken(tokenizer);

  Symbol  *number1 = createSymbol(operandToken1, OPERAND, INTEGER);
  Symbol  *number2 = createSymbol(operandToken2, OPERAND, INTEGER); 

  Symbol  *result = infixDivide(number1, number2);

  TEST_ASSERT_EQUAL_FLOAT(1.75, getSymbolDouble(result));
  TEST_ASSERT_EQUAL(DOUBLE, result->id);

  freeSymbol(number1);
  freeSymbol(number2);
  freeSymbol(result);
  freeToken(operatorToken);
  freeTokenizer(tokenizer);
}

void  test_infixShiftLeft_given_2_int_expect_result_with_hex_AC_returned(){
  Tokenizer *tokenizer = createTokenizer(" 0x2B <<2");
  Symbolizer  *symbolizer = createSymbolizer(tokenizer);
  Symbol  *number1 = getSymbol(symbolizer);
  Symbol  *operator = getSymbol(symbolizer);
  Symbol  *number2 = getSymbol(symbolizer);

  Symbol  *result = infixShiftLeft(number1, number2);

  TEST_ASSERT_EQUAL(0xAC, getSymbolInteger(result));
  TEST_ASSERT_EQUAL(INTEGER, result->id);

  freeSymbol(number1);
  freeSymbol(number2);
  freeSymbol(operator);
  freeSymbol(result);
  freeSymbolizer(symbolizer);
}

void  test_infixShiftLeft_given_2_int_expect_result_with_hex_1840_returned(){
  Tokenizer *tokenizer = createTokenizer(" 0xC2 <<5");
  Symbolizer  *symbolizer = createSymbolizer(tokenizer);
  Symbol  *number1 = getSymbol(symbolizer);
  Symbol  *operator = getSymbol(symbolizer);
  Symbol  *number2 = getSymbol(symbolizer);

  Symbol  *result = infixShiftLeft(number1, number2);

  TEST_ASSERT_EQUAL(0x1840, getSymbolInteger(result));
  TEST_ASSERT_EQUAL(INTEGER, result->id);

  freeSymbol(number1);
  freeSymbol(number2);
  freeSymbol(operator);
  freeSymbol(result);
  freeSymbolizer(symbolizer);
}

void  test_infixShiftRight_given_2_int_expect_result_with_hex_1840_returned(){
  Tokenizer *tokenizer = createTokenizer(" 0x33>>2");
  Symbolizer  *symbolizer = createSymbolizer(tokenizer);
  Symbol  *number1 = getSymbol(symbolizer);
  Symbol  *operator = getSymbol(symbolizer);
  Symbol  *number2 = getSymbol(symbolizer);

  Symbol  *result = infixShiftRight(number1, number2);

  TEST_ASSERT_EQUAL(0xC, getSymbolInteger(result));
  TEST_ASSERT_EQUAL(INTEGER, result->id);

  freeSymbol(number1);
  freeSymbol(number2);
  freeSymbol(operator);
  freeSymbol(result);
  freeSymbolizer(symbolizer);
}

void  test_infixLesser_given_2_int_lesser_condition_expect_result_with_1_returned(){
  Tokenizer *tokenizer = createTokenizer(" 12<89");
  Symbolizer  *symbolizer = createSymbolizer(tokenizer);
  Symbol  *number1 = getSymbol(symbolizer);
  Symbol  *operator = getSymbol(symbolizer);
  Symbol  *number2 = getSymbol(symbolizer);

  Symbol  *result = infixLesser(number1, number2);

  TEST_ASSERT_EQUAL(1, getSymbolInteger(result));
  TEST_ASSERT_EQUAL(INTEGER, result->id);

  freeSymbol(number1);
  freeSymbol(number2);
  freeSymbol(operator);
  freeSymbol(result);
  freeSymbolizer(symbolizer);
}

void  test_infixLesser_given_2_int_not_lesser_condition_expect_result_with_0_returned(){
  Tokenizer *tokenizer = createTokenizer(" 422<89");
  Symbolizer  *symbolizer = createSymbolizer(tokenizer);
  Symbol  *number1 = getSymbol(symbolizer);
  Symbol  *operator = getSymbol(symbolizer);
  Symbol  *number2 = getSymbol(symbolizer);

  Symbol  *result = infixLesser(number1, number2);

  TEST_ASSERT_EQUAL(0, getSymbolInteger(result));
  TEST_ASSERT_EQUAL(INTEGER, result->id);

  freeSymbol(number1);
  freeSymbol(number2);
  freeSymbol(operator);
  freeSymbol(result);
  freeSymbolizer(symbolizer);
}

void  test_infixLesserEq_given_2_int_not_lesser_condition_expect_result_with_0_returned(){
  Tokenizer *tokenizer = createTokenizer(" 54  <= 12");
  Symbolizer  *symbolizer = createSymbolizer(tokenizer);
  Symbol  *number1 = getSymbol(symbolizer);
  Symbol  *operator = getSymbol(symbolizer);
  Symbol  *number2 = getSymbol(symbolizer);

  Symbol  *result = infixLesserEq(number1, number2);

  TEST_ASSERT_EQUAL(0, getSymbolInteger(result));
  TEST_ASSERT_EQUAL(INTEGER, result->id);

  freeSymbol(number1);
  freeSymbol(number2);
  freeSymbol(operator);
  freeSymbol(result);
  freeSymbolizer(symbolizer);
}

void  test_infixLesserEq_given_2_int_equal_condition_expect_result_with_1_returned(){
  Tokenizer *tokenizer = createTokenizer(" 66  <= 66");
  Symbolizer  *symbolizer = createSymbolizer(tokenizer);
  Symbol  *number1 = getSymbol(symbolizer);
  Symbol  *operator = getSymbol(symbolizer);
  Symbol  *number2 = getSymbol(symbolizer);

  Symbol  *result = infixLesserEq(number1, number2);

  TEST_ASSERT_EQUAL(1, getSymbolInteger(result));
  TEST_ASSERT_EQUAL(INTEGER, result->id);

  freeSymbol(number1);
  freeSymbol(number2);
  freeSymbol(operator);
  freeSymbol(result);
  freeSymbolizer(symbolizer);
}

void  test_infixLesserEq_given_2_int_lesser_condition_expect_result_with_1_returned(){
  Tokenizer *tokenizer = createTokenizer(" 65  <= 66");
  Symbolizer  *symbolizer = createSymbolizer(tokenizer);
  Symbol  *number1 = getSymbol(symbolizer);
  Symbol  *operator = getSymbol(symbolizer);
  Symbol  *number2 = getSymbol(symbolizer);

  Symbol  *result = infixLesserEq(number1, number2);

  TEST_ASSERT_EQUAL(1, getSymbolInteger(result));
  TEST_ASSERT_EQUAL(INTEGER, result->id);

  freeSymbol(number1);
  freeSymbol(number2);
  freeSymbol(operator);
  freeSymbol(result);
  freeSymbolizer(symbolizer);
}

void  test_infixGreater_given_2_int_greater_condition_expect_result_with_1_returned(){
  Tokenizer *tokenizer = createTokenizer(" 422>89");
  Symbolizer  *symbolizer = createSymbolizer(tokenizer);
  Symbol  *number1 = getSymbol(symbolizer);
  Symbol  *operator = getSymbol(symbolizer);
  Symbol  *number2 = getSymbol(symbolizer);

  Symbol  *result = infixGreater(number1, number2);

  TEST_ASSERT_EQUAL(1, getSymbolInteger(result));
  TEST_ASSERT_EQUAL(INTEGER, result->id);

  freeSymbol(number1);
  freeSymbol(number2);
  freeSymbol(operator);
  freeSymbol(result);
  freeSymbolizer(symbolizer);
}

void  test_infixGreater_given_2_int_not_greater_condition_expect_result_with_0_returned(){
  Tokenizer *tokenizer = createTokenizer(" 67  >     999");
  Symbolizer  *symbolizer = createSymbolizer(tokenizer);
  Symbol  *number1 = getSymbol(symbolizer);
  Symbol  *operator = getSymbol(symbolizer);
  Symbol  *number2 = getSymbol(symbolizer);

  Symbol  *result = infixGreater(number1, number2);

  TEST_ASSERT_EQUAL(0, getSymbolInteger(result));
  TEST_ASSERT_EQUAL(INTEGER, result->id);

  freeSymbol(number1);
  freeSymbol(number2);
  freeSymbol(operator);
  freeSymbol(result);
  freeSymbolizer(symbolizer);
}

void  test_infixGreaterEq_given_2_int_greater_condition_expect_result_with_1_returned(){
  Tokenizer *tokenizer = createTokenizer(" 11111>=1223");
  Symbolizer  *symbolizer = createSymbolizer(tokenizer);
  Symbol  *number1 = getSymbol(symbolizer);
  Symbol  *operator = getSymbol(symbolizer);
  Symbol  *number2 = getSymbol(symbolizer);

  Symbol  *result = infixGreaterEq(number1, number2);

  TEST_ASSERT_EQUAL(1, getSymbolInteger(result));
  TEST_ASSERT_EQUAL(INTEGER, result->id);

  freeSymbol(number1);
  freeSymbol(number2);
  freeSymbol(operator);
  freeSymbol(result);
  freeSymbolizer(symbolizer);
}

void  test_infixGreaterEq_given_2_int_not_greater_condition_expect_result_with_0_returned(){
  Tokenizer *tokenizer = createTokenizer(" 0X22 >=   355");
  Symbolizer  *symbolizer = createSymbolizer(tokenizer);
  Symbol  *number1 = getSymbol(symbolizer);
  Symbol  *operator = getSymbol(symbolizer);
  Symbol  *number2 = getSymbol(symbolizer);

  Symbol  *result = infixGreaterEq(number1, number2);

  TEST_ASSERT_EQUAL(0, getSymbolInteger(result));
  TEST_ASSERT_EQUAL(INTEGER, result->id);

  freeSymbol(number1);
  freeSymbol(number2);
  freeSymbol(operator);
  freeSymbol(result);
  freeSymbolizer(symbolizer);
}

void  test_infixGreaterEq_given_2_int_equal_condition_expect_result_with_1_returned(){
  Tokenizer *tokenizer = createTokenizer(" 0X55 >=   85");
  Symbolizer  *symbolizer = createSymbolizer(tokenizer);
  Symbol  *number1 = getSymbol(symbolizer);
  Symbol  *operator = getSymbol(symbolizer);
  Symbol  *number2 = getSymbol(symbolizer);

  Symbol  *result = infixGreaterEq(number1, number2);

  TEST_ASSERT_EQUAL(1, getSymbolInteger(result));
  TEST_ASSERT_EQUAL(INTEGER, result->id);

  freeSymbol(number1);
  freeSymbol(number2);
  freeSymbol(operator);
  freeSymbol(result);
  freeSymbolizer(symbolizer);
}

void  test_infixLogicalOr_given_2_int_1_expect_result_with_1_returned(){
  Tokenizer *tokenizer = createTokenizer(" 1  ||   1");
  Symbolizer  *symbolizer = createSymbolizer(tokenizer);
  Symbol  *number1 = getSymbol(symbolizer);
  Symbol  *operator = getSymbol(symbolizer);
  Symbol  *number2 = getSymbol(symbolizer);

  Symbol  *result = infixLogicalOr(number1, number2);

  TEST_ASSERT_EQUAL(1, getSymbolInteger(result));
  TEST_ASSERT_EQUAL(INTEGER, result->id);

  freeSymbol(number1);
  freeSymbol(number2);
  freeSymbol(operator);
  freeSymbol(result);
  freeSymbolizer(symbolizer);
}

void  test_infixLogicalOr_given_2_int_0_expect_result_with_0_returned(){
  Tokenizer *tokenizer = createTokenizer(" 0  ||   0");
  Symbolizer  *symbolizer = createSymbolizer(tokenizer);
  Symbol  *number1 = getSymbol(symbolizer);
  Symbol  *operator = getSymbol(symbolizer);
  Symbol  *number2 = getSymbol(symbolizer);

  Symbol  *result = infixLogicalOr(number1, number2);

  TEST_ASSERT_EQUAL(0, getSymbolInteger(result));
  TEST_ASSERT_EQUAL(INTEGER, result->id);

  freeSymbol(number1);
  freeSymbol(number2);
  freeSymbol(operator);
  freeSymbol(result);
  freeSymbolizer(symbolizer);
}

void  test_infixLogicalOr_given_int_0_and_1_expect_result_with_1_returned(){
  Tokenizer *tokenizer = createTokenizer(" 0  ||   1");
  Symbolizer  *symbolizer = createSymbolizer(tokenizer);
  Symbol  *number1 = getSymbol(symbolizer);
  Symbol  *operator = getSymbol(symbolizer);
  Symbol  *number2 = getSymbol(symbolizer);

  Symbol  *result = infixLogicalOr(number1, number2);

  TEST_ASSERT_EQUAL(1, getSymbolInteger(result));
  TEST_ASSERT_EQUAL(INTEGER, result->id);

  freeSymbol(number1);
  freeSymbol(number2);
  freeSymbol(operator);
  freeSymbol(result);
  freeSymbolizer(symbolizer);
}

void  test_infixLogicalAnd_given_2_int_0__expect_result_with_0_returned(){
  Tokenizer *tokenizer = createTokenizer(" 0  &&   0");
  Symbolizer  *symbolizer = createSymbolizer(tokenizer);
  Symbol  *number1 = getSymbol(symbolizer);
  Symbol  *operator = getSymbol(symbolizer);
  Symbol  *number2 = getSymbol(symbolizer);

  Symbol  *result = infixLogicalAnd(number1, number2);

  TEST_ASSERT_EQUAL(0, getSymbolInteger(result));
  TEST_ASSERT_EQUAL(INTEGER, result->id);

  freeSymbol(number1);
  freeSymbol(number2);
  freeSymbol(operator);
  freeSymbol(result);
  freeSymbolizer(symbolizer);
}

void  test_infixLogicalAnd_given_int_1_and_0_expect_result_with_0_returned(){
  Tokenizer *tokenizer = createTokenizer(" 1  &&   0");
  Symbolizer  *symbolizer = createSymbolizer(tokenizer);
  Symbol  *number1 = getSymbol(symbolizer);
  Symbol  *operator = getSymbol(symbolizer);
  Symbol  *number2 = getSymbol(symbolizer);

  Symbol  *result = infixLogicalAnd(number1, number2);

  TEST_ASSERT_EQUAL(0, getSymbolInteger(result));
  TEST_ASSERT_EQUAL(INTEGER, result->id);

  freeSymbol(number1);
  freeSymbol(number2);
  freeSymbol(operator);
  freeSymbol(result);
  freeSymbolizer(symbolizer);
}

void  test_infixLogicalAnd_given_2_int_1_expect_result_with_1_returned(){
  Tokenizer *tokenizer = createTokenizer(" 1  &&   1");
  Symbolizer  *symbolizer = createSymbolizer(tokenizer);
  Symbol  *number1 = getSymbol(symbolizer);
  Symbol  *operator = getSymbol(symbolizer);
  Symbol  *number2 = getSymbol(symbolizer);

  Symbol  *result = infixLogicalAnd(number1, number2);

  TEST_ASSERT_EQUAL(1, getSymbolInteger(result));
  TEST_ASSERT_EQUAL(INTEGER, result->id);

  freeSymbol(number1);
  freeSymbol(number2);
  freeSymbol(operator);
  freeSymbol(result);
  freeSymbolizer(symbolizer);
}

void  test_infixBitwiseOr_given_2_int_12_and_55_expect_result_with_63_returned(){
  Tokenizer *tokenizer = createTokenizer(" 12|55");
  Symbolizer  *symbolizer = createSymbolizer(tokenizer);
  Symbol  *number1 = getSymbol(symbolizer);
  Symbol  *operator = getSymbol(symbolizer);
  Symbol  *number2 = getSymbol(symbolizer);

  Symbol  *result = infixBitwiseOr(number1, number2);

  TEST_ASSERT_EQUAL(63, getSymbolInteger(result));
  TEST_ASSERT_EQUAL(INTEGER, result->id);

  freeSymbol(number1);
  freeSymbol(number2);
  freeSymbol(operator);
  freeSymbol(result);
  freeSymbolizer(symbolizer);
}

void  test_infixEqual_given_2_equal_int_expect_1_returned(){
  Tokenizer *tokenizer = createTokenizer(" 77==77");
  Symbolizer  *symbolizer = createSymbolizer(tokenizer);
  Symbol  *number1 = getSymbol(symbolizer);
  Symbol  *operator = getSymbol(symbolizer);
  Symbol  *number2 = getSymbol(symbolizer);

  Symbol  *result = infixEqual(number1, number2);

  TEST_ASSERT_EQUAL(1, getSymbolInteger(result));
  TEST_ASSERT_EQUAL(INTEGER, result->id);

  freeSymbol(number1);
  freeSymbol(number2);
  freeSymbol(operator);
  freeSymbol(result);
  freeSymbolizer(symbolizer);
}

void  test_infixEqual_given_2_not_equal_int_expect_0_returned(){
  Tokenizer *tokenizer = createTokenizer(" 71==77");
  Symbolizer  *symbolizer = createSymbolizer(tokenizer);
  Symbol  *number1 = getSymbol(symbolizer);
  Symbol  *operator = getSymbol(symbolizer);
  Symbol  *number2 = getSymbol(symbolizer);

  Symbol  *result = infixEqual(number1, number2);

  TEST_ASSERT_EQUAL(0, getSymbolInteger(result));
  TEST_ASSERT_EQUAL(INTEGER, result->id);

  freeSymbol(number1);
  freeSymbol(number2);
  freeSymbol(operator);
  freeSymbol(result);
  freeSymbolizer(symbolizer);
}

void  test_infixEqual_given_2_equal_double_expect_0_returned(){
  Tokenizer *tokenizer = createTokenizer(" 11.111==11.111");
  Symbolizer  *symbolizer = createSymbolizer(tokenizer);
  Symbol  *number1 = getSymbol(symbolizer);
  Symbol  *operator = getSymbol(symbolizer);
  Symbol  *number2 = getSymbol(symbolizer);

  Symbol  *result = infixEqual(number1, number2);

  TEST_ASSERT_EQUAL(1, getSymbolInteger(result));
  TEST_ASSERT_EQUAL(INTEGER, result->id);

  freeSymbol(number1);
  freeSymbol(number2);
  freeSymbol(operator);
  freeSymbol(result);
  freeSymbolizer(symbolizer);
}

void  test_handleInfix_given_two_operand_Integers_1_infix_add_expect_the_values_able_to_be_returned(){
  Tokenizer *tokenizer = createTokenizer(" 13+77");
  Token *operandToken1 = getToken(tokenizer);
  Token *operatorToken = getToken(tokenizer);
  Token *operandToken2 = getToken(tokenizer);

  Symbol  *number1 = createSymbol(operandToken1, OPERAND, INTEGER);
  Symbol  *number2 = createSymbol(operandToken2, OPERAND, INTEGER); 
  Symbol  *operator = createSymbol(operatorToken, OPERATOR, ADD);
  
  StackStruct *operandStack = createStack();
  StackStruct *operatorStack = createStack();
  
  pushToStack(operandStack, (void*)number1);
  pushToStack(operandStack, (void*)number2);
  pushToStack(operatorStack, (void*)operator);
  
  handleInfix(operandStack, operatorStack);
  
  ListItem  *peekItem = peekTopOfStack(operandStack);
  
  TEST_ASSERT_EQUAL(90, getItemSymbolInteger(peekItem));
  TEST_ASSERT_EQUAL(OPERAND, getItemSymbolType(peekItem));
  TEST_ASSERT_EQUAL(INTEGER, getItemSymbolId(peekItem));
  
  freeStack(operandStack, freeSymbolInLinkedList); 
  freeStack(operatorStack, freeSymbolInLinkedList); 
  freeTokenizer(tokenizer);
}

void  test_handleInfix_given_two_operand_Integer_Double_1_infix_mult_expect_the_values_able_to_be_returned(){
  Tokenizer *tokenizer = createTokenizer("34*4.888");
  Token *operandToken1 = getToken(tokenizer);
  Token *operatorToken = getToken(tokenizer);
  Token *operandToken2 = getToken(tokenizer);

  Symbol  *number1 = createSymbol(operandToken1, OPERAND, INTEGER);
  Symbol  *number2 = createSymbol(operandToken2, OPERAND, DOUBLE); 
  Symbol  *operator = createSymbol(operatorToken, OPERATOR, MULTIPLY);
  
  StackStruct *operandStack = createStack();
  StackStruct *operatorStack = createStack();
  
  pushToStack(operandStack, (void*)number1);
  pushToStack(operandStack, (void*)number2);
  pushToStack(operatorStack, (void*)operator);
  
  handleInfix(operandStack, operatorStack);
  
  ListItem  *peekItem = peekTopOfStack(operandStack);
  
  TEST_ASSERT_EQUAL_FLOAT(166.192, getItemSymbolDouble(peekItem));
  TEST_ASSERT_EQUAL(OPERAND, getItemSymbolType(peekItem));
  TEST_ASSERT_EQUAL(DOUBLE, getItemSymbolId(peekItem));
  
  freeStack(operandStack, freeSymbolInLinkedList); 
  freeStack(operatorStack, freeSymbolInLinkedList); 
  freeTokenizer(tokenizer);
}

void  test_handleInfix_given_two_operand_Integers_1_infix_mult_expect_double_value_able_to_be_returned(){
  Tokenizer *tokenizer = createTokenizer("7/4");
  Token *operandToken1 = getToken(tokenizer);
  Token *operatorToken = getToken(tokenizer);
  Token *operandToken2 = getToken(tokenizer);

  Symbol  *number1 = createSymbol(operandToken1, OPERAND, INTEGER);
  Symbol  *number2 = createSymbol(operandToken2, OPERAND, INTEGER); 
  Symbol  *operator = createSymbol(operatorToken, OPERATOR, DIVIDE);
  
  StackStruct *operandStack = createStack();
  StackStruct *operatorStack = createStack();
  
  pushToStack(operandStack, (void*)number1);
  pushToStack(operandStack, (void*)number2);
  pushToStack(operatorStack, (void*)operator);
  
  handleInfix(operandStack, operatorStack);
  
  ListItem  *peekItem = peekTopOfStack(operandStack);
  
  TEST_ASSERT_EQUAL_FLOAT(1.75, getItemSymbolDouble(peekItem));
  TEST_ASSERT_EQUAL(OPERAND, getItemSymbolType(peekItem));
  TEST_ASSERT_EQUAL(DOUBLE, getItemSymbolId(peekItem));
  
  freeStack(operandStack, freeSymbolInLinkedList); 
  freeStack(operatorStack, freeSymbolInLinkedList); 
  freeTokenizer(tokenizer);
}

void  test_handleInfix_given_two_operand_Doubles_1_infix_division_expect_the_values_able_to_be_returned(){
  Tokenizer *tokenizer = createTokenizer("9.57434/4.888");
  Token *operandToken1 = getToken(tokenizer);
  Token *operatorToken = getToken(tokenizer);
  Token *operandToken2 = getToken(tokenizer);

  Symbol  *number1 = createSymbol(operandToken1, OPERAND, DOUBLE);
  Symbol  *number2 = createSymbol(operandToken2, OPERAND, DOUBLE); 
  Symbol  *operator = createSymbol(operatorToken, OPERATOR, DIVIDE);
  
  StackStruct *operandStack = createStack();
  StackStruct *operatorStack = createStack();
  
  pushToStack(operandStack, (void*)number1);
  pushToStack(operandStack, (void*)number2);
  pushToStack(operatorStack, (void*)operator);
  
  handleInfix(operandStack, operatorStack);
  
  ListItem  *peekItem = peekTopOfStack(operandStack);
  
  TEST_ASSERT_EQUAL_FLOAT(1.958743863, getItemSymbolDouble(peekItem));
  TEST_ASSERT_EQUAL(OPERAND, getItemSymbolType(peekItem));
  TEST_ASSERT_EQUAL(DOUBLE, getItemSymbolId(peekItem));
  
  freeStack(operandStack, freeSymbolInLinkedList); 
  freeStack(operatorStack, freeSymbolInLinkedList); 
  freeTokenizer(tokenizer);
}

void  test_handlePrefix_given_operand_bitwise_not_expect_the_value_able_to_be_returned(){
  Tokenizer *tokenizer = createTokenizer("~13");
  Token *operatorToken = getToken(tokenizer);
  Token *operandToken = getToken(tokenizer);

  Symbol  *number = createSymbol(operandToken, OPERAND, INTEGER); 
  Symbol  *operator = createSymbol(operatorToken, OPERATOR, BITWISE_NOT);
  
  StackStruct *operandStack = createStack();
  StackStruct *operatorStack = createStack();
  
  pushToStack(operandStack, (void*)number);
  pushToStack(operatorStack, (void*)operator);
  
  handlePrefix(operandStack, operatorStack);
  
  ListItem  *peekItem = peekTopOfStack(operandStack);
  
  TEST_ASSERT_EQUAL_FLOAT(-14, getItemSymbolInteger(peekItem));
  TEST_ASSERT_EQUAL(OPERAND, getItemSymbolType(peekItem));
  TEST_ASSERT_EQUAL(INTEGER, getItemSymbolId(peekItem));
  
  freeStack(operandStack, freeSymbolInLinkedList); 
  freeStack(operatorStack, freeSymbolInLinkedList); 
  freeTokenizer(tokenizer);
}

void  test_handlePrefix_given_operand_logic_not_expect_the_value_able_to_be_returned(){
  Tokenizer *tokenizer = createTokenizer("!0");
  Token *operatorToken = getToken(tokenizer);
  Token *operandToken = getToken(tokenizer);

  Symbol  *number = createSymbol(operandToken, OPERAND, INTEGER); 
  Symbol  *operator = createSymbol(operatorToken, OPERATOR, LOGICAL_NOT);
  
  StackStruct *operandStack = createStack();
  StackStruct *operatorStack = createStack();
  
  pushToStack(operandStack, (void*)number);
  pushToStack(operatorStack, (void*)operator);
  
  handlePrefix(operandStack, operatorStack);
  
  ListItem  *peekItem = peekTopOfStack(operandStack);
  
  TEST_ASSERT_EQUAL_FLOAT(1, getItemSymbolInteger(peekItem));
  TEST_ASSERT_EQUAL(OPERAND, getItemSymbolType(peekItem));
  TEST_ASSERT_EQUAL(INTEGER, getItemSymbolId(peekItem));
  
  freeStack(operandStack, freeSymbolInLinkedList); 
  freeStack(operatorStack, freeSymbolInLinkedList); 
  freeTokenizer(tokenizer);
}

void  test_unwindStack_given_2_operands_1_operator_inside_operator_stack_expect_operator_computed(){
  Tokenizer *tokenizer = createTokenizer("13 + 54.542");
  Symbolizer  *symbolizer = createSymbolizer(tokenizer);

  Symbol  *number1 = getSymbol(symbolizer);
  Symbol  *operator = getSymbol(symbolizer);
  Symbol  *number2 = getSymbol(symbolizer);
  
  StackStruct *operandStack = createStack();
  StackStruct *operatorStack = createStack();
  
  pushToStack(operandStack, (void*)number1);
  pushToStack(operandStack, (void*)number2);
  pushToStack(operatorStack, (void*)operator);
  
  unwindStack(operatorStack, operandStack);
  
  ListItem  *peekItem = peekTopOfStack(operatorStack);
  
  peekItem = peekTopOfStack(operandStack);
  TEST_ASSERT_EQUAL(0, operatorStack->size);
  TEST_ASSERT_EQUAL_FLOAT(67.542, getItemSymbolDouble(peekItem));
  
  freeStack(operandStack, freeSymbolInLinkedList); 
  freeStack(operatorStack, freeSymbolInLinkedList); 
  freeSymbolizer(symbolizer);
}

void  test_unwindStack_given_2_operands_1_lower_precedence_operator_inside_operator_stack_expect_currentOperator_pushed_into_operator_stack(){
  Tokenizer *tokenizer = createTokenizer("13 + 54.452 /");
  Symbolizer  *symbolizer = createSymbolizer(tokenizer);

  Symbol  *number1 = getSymbol(symbolizer);
  Symbol  *operator1 = getSymbol(symbolizer);
  Symbol  *number2 = getSymbol(symbolizer);
  Symbol  *operator2 = getSymbol(symbolizer);
  
  StackStruct *operandStack = createStack();
  StackStruct *operatorStack = createStack();
  
  pushToStack(operandStack, (void*)number1);
  pushToStack(operandStack, (void*)number2);
  pushToStack(operatorStack, (void*)operator1);
  pushToStack(operatorStack, (void*)operator2);
  
  unwindStack(operatorStack, operandStack);
  
  ListItem  *peekItem = peekTopOfStack(operatorStack);
  
  TEST_ASSERT_EQUAL(ADD, ((Symbol  *)(*peekItem).data)->id);
  
  peekItem = peekTopOfStack(operandStack);
  TEST_ASSERT_EQUAL(1, operatorStack->size);
  TEST_ASSERT_EQUAL_FLOAT(0.2387424, getItemSymbolDouble(peekItem));
  
  freeStack(operandStack, freeSymbolInLinkedList); 
  freeStack(operatorStack, freeSymbolInLinkedList); 
  freeSymbolizer(symbolizer);
}

void  test_unwindStack_given_2_operands_1_operator_inside_operator_stack_expect_multiply_computed(){
  Tokenizer *tokenizer = createTokenizer("13 * 3");
  Symbolizer  *symbolizer = createSymbolizer(tokenizer);

  Symbol  *number1 = getSymbol(symbolizer);
  Symbol  *operator1 = getSymbol(symbolizer);
  Symbol  *number2 = getSymbol(symbolizer);
  
  StackStruct *operandStack = createStack();
  StackStruct *operatorStack = createStack();
  
  pushToStack(operandStack, (void*)number1);
  pushToStack(operandStack, (void*)number2);
  pushToStack(operatorStack, (void*)operator1);
  
  unwindStack(operatorStack, operandStack);
  
  ListItem  *peekItem = peekTopOfStack(operandStack);
  TEST_ASSERT_EQUAL(39, getItemSymbolInteger(peekItem));
  TEST_ASSERT_EQUAL(OPERAND, getItemSymbolType(peekItem));
  TEST_ASSERT_EQUAL(INTEGER, getItemSymbolId(peekItem));
  
  freeStack(operandStack, freeSymbolInLinkedList); 
  freeStack(operatorStack, freeSymbolInLinkedList); 
  freeSymbolizer(symbolizer);
}

void  test_forcePush_given_open_bracket_operator_expect_the_bracket_able_to_be_pushed(){
  Tokenizer *tokenizer = createTokenizer("13 *54.452 (");
  Symbolizer  *symbolizer = createSymbolizer(tokenizer);

  Symbol  *number1 = getSymbol(symbolizer);
  Symbol  *operator = getSymbol(symbolizer);
  Symbol  *number2 = getSymbol(symbolizer);
  Symbol  *currentOperator = getSymbol(symbolizer);
  
  StackStruct *operandStack = createStack();
  StackStruct *operatorStack = createStack();
  
  pushToStack(operandStack, (void*)number1);
  pushToStack(operandStack, (void*)number2);
  pushToStack(operatorStack, (void*)operator);  
  
  forcePush(operandStack, operatorStack, currentOperator, 0);
  
  ListItem  *peekItem = peekTopOfStack(operatorStack);
  
  TEST_ASSERT_EQUAL(2, operatorStack->size);
  TEST_ASSERT_EQUAL(OPEN_PAREN, getItemSymbolId(peekItem));  
  
  freeStack(operandStack, freeSymbolInLinkedList); 
  freeStack(operatorStack, freeSymbolInLinkedList); 
  freeSymbolizer(symbolizer);
}

void  test_pushOperator_given_2_operands_1_lower_precedence_operator_inside_operator_stack_expect_currentOperator_pushed_into_operator_stack(){
  Tokenizer *tokenizer = createTokenizer("13 +54.452 /");
  Symbolizer  *symbolizer = createSymbolizer(tokenizer);

  Symbol  *number1 = getSymbol(symbolizer);
  Symbol  *operator = getSymbol(symbolizer);
  Symbol  *number2 = getSymbol(symbolizer);
  Symbol  *currentOperator = getSymbol(symbolizer);
  
  StackStruct *operandStack = createStack();
  StackStruct *operatorStack = createStack();
  
  pushToStack(operandStack, (void*)number1);
  pushToStack(operandStack, (void*)number2);
  pushToStack(operatorStack, (void*)operator);
  
  pushOperator(operandStack, operatorStack, currentOperator);
  
  ListItem  *peekItem = peekTopOfStack(operatorStack);
  
  TEST_ASSERT_EQUAL(2, operatorStack->size);
  TEST_ASSERT_EQUAL(2, operandStack->size);
  TEST_ASSERT_EQUAL(DIVIDE, getItemSymbolId(peekItem));
  
  freeStack(operandStack, freeSymbolInLinkedList); 
  freeStack(operatorStack, freeSymbolInLinkedList); 
  freeSymbolizer(symbolizer);
}

//13-3*6+8
void  test_pushOperator_given_2_operators_1_higher_precedence_operator_inside_operator_stack_expect_higher_operator_computed(){
  Tokenizer *tokenizer = createTokenizer(" 13-3*6+8 ");
  Symbolizer  *symbolizer = createSymbolizer(tokenizer);

  Symbol  *number1 = getSymbol(symbolizer);
  Symbol  *operator1 = getSymbol(symbolizer);
  Symbol  *number2 = getSymbol(symbolizer);
  Symbol  *operator2 = getSymbol(symbolizer);
  Symbol  *number3 = getSymbol(symbolizer);
  Symbol  *currentOperator = getSymbol(symbolizer);
  
  StackStruct *operandStack = createStack();
  StackStruct *operatorStack = createStack();
  
  pushToStack(operandStack, (void*)number1);
  pushToStack(operandStack, (void*)number2);
  pushToStack(operandStack, (void*)number3);
  pushToStack(operatorStack, (void*)operator1);
  pushToStack(operatorStack, (void*)operator2);
  
  pushOperator(operandStack, operatorStack, currentOperator);
  
  ListItem  *peekItem = peekTopOfStack(operatorStack);
  
  TEST_ASSERT_EQUAL(ADD, getItemSymbolId(peekItem));
  
  peekItem = peekTopOfStack(operandStack);
  TEST_ASSERT_EQUAL(-5, getItemSymbolInteger(peekItem));
  TEST_ASSERT_EQUAL(1, operatorStack->size);
  TEST_ASSERT_EQUAL(OPERAND, getItemSymbolType(peekItem));
  TEST_ASSERT_EQUAL(INTEGER, getItemSymbolId(peekItem));
  
  freeStack(operandStack, freeSymbolInLinkedList); 
  freeStack(operatorStack, freeSymbolInLinkedList); 
  freeSymbolizer(symbolizer);
}

//(3+18)
void  test_evaluateExpressionWithinBrackets_given_brackets_operators_operands(){
  Tokenizer *tokenizer = createTokenizer(" (3+18) ");
  Symbolizer  *symbolizer = createSymbolizer(tokenizer);
  Symbol  *operator1 = getSymbol(symbolizer);
  Symbol  *number1 = getSymbol(symbolizer);
  Symbol  *operator2 = getSymbol(symbolizer);
  Symbol  *number2 = getSymbol(symbolizer);
  Symbol  *currentOperator = getSymbol(symbolizer);
  
  Symbol  *lastSymbol = cloneSymbol(number2);
  
  StackStruct *operandStack = createStack();
  StackStruct *operatorStack = createStack();  
  
  pushToStack(operatorStack, (void*)operator1);
  pushToStack(operatorStack, (void*)operator2);
  
  pushToStack(operandStack, (void*)number1);
  pushToStack(operandStack, (void*)number2);
  
  symbolizer->lastSymbol = lastSymbol;
  
  evaluateExpressionWithinBrackets(operandStack, operatorStack, currentOperator, symbolizer);
  
  ListItem  *peekItem = peekTopOfStack(operandStack);
  
  TEST_ASSERT_EQUAL(21, getItemSymbolInteger(peekItem));
  
  freeStack(operandStack, freeSymbolInLinkedList); 
  freeStack(operatorStack, freeSymbolInLinkedList); 
  freeSymbolizer(symbolizer);
}

//6*(200 -10/2)
void  test_evaluateExpressionWithinBrackets_given_brackets_3operators_4operands(){
  Tokenizer *tokenizer = createTokenizer(" 6*(200 -10/2) ");
  Symbolizer  *symbolizer = createSymbolizer(tokenizer);

  Symbol  *number1 = getSymbol(symbolizer);
  Symbol  *operator1 = getSymbol(symbolizer);
  Symbol  *operator2 = getSymbol(symbolizer);
  Symbol  *number2 = getSymbol(symbolizer);
  Symbol  *operator3 = getSymbol(symbolizer);
  Symbol  *number3 = getSymbol(symbolizer);
  Symbol  *operator4 = getSymbol(symbolizer);
  Symbol  *number4 = getSymbol(symbolizer);
  Symbol  *currentOperator = getSymbol(symbolizer);
  
  Symbol  *lastSymbol = cloneSymbol(number4);
  
  symbolizer->lastSymbol = lastSymbol;
  
  StackStruct *operandStack = createStack();
  StackStruct *operatorStack = createStack();  
  
  pushToStack(operatorStack, (void*)operator1);
  pushToStack(operatorStack, (void*)operator2);
  pushToStack(operatorStack, (void*)operator3);
  pushToStack(operatorStack, (void*)operator4);
  
  pushToStack(operandStack, (void*)number1);
  pushToStack(operandStack, (void*)number2);
  pushToStack(operandStack, (void*)number3);
  pushToStack(operandStack, (void*)number4);
  
  evaluateExpressionWithinBrackets(operandStack, operatorStack, currentOperator, symbolizer);
  
  ListItem  *peekItem = peekTopOfStack(operandStack);
  
  TEST_ASSERT_EQUAL(195, getItemSymbolInteger(peekItem));
  TEST_ASSERT_EQUAL(2, operandStack->size);
  
  peekItem = peekTopOfStack(operatorStack);
  TEST_ASSERT_EQUAL(MULTIPLY, getItemSymbolId(peekItem));
  TEST_ASSERT_EQUAL(1, operatorStack->size);
  
  freeStack(operandStack, freeSymbolInLinkedList); 
  freeStack(operatorStack, freeSymbolInLinkedList); 
  freeSymbolizer(symbolizer); 
}

/*
void  test_obtainSymbolFromSymbolizer_given_a_symbolizer_with_a_operand_expect_OPERAND_symbol_with_updated_last_symbol_data_in_symbolizer(){
  Tokenizer *tokenizer = createTokenizer(" 6 ");
  Symbolizer  *symbolizer = createSymbolizer(tokenizer);
  Symbol  *symbol = symbolizerGetSymbol(symbolizer);
  
  TEST_ASSERT_EQUAL(6, getSymbolInteger(symbol));
  TEST_ASSERT_EQUAL(INTEGER, getSymbolId(symbol));
  TEST_ASSERT_EQUAL(OPERAND, getSymbolType(symbol));
  TEST_ASSERT_EQUAL(NUMBER, getSymbolArity(symbol));
  
  TEST_ASSERT_EQUAL(INTEGER, symbolizer->lastSymbolId);
  
  freeSymbol(symbol);
  freeSymbolizer(symbolizer);
}

void  test_obtainSymbolFromSymbolizer_given_a_symbolizer_with_a_operandand_an_operator_expect_OPERAND_OPERATOR_symbol_with_updated_last_symbol_data_in_symbolizer(){
  Tokenizer *tokenizer = createTokenizer(" 7&33 ");
  Symbolizer  *symbolizer = createSymbolizer(tokenizer);
  Symbol  *symbol = symbolizerGetSymbol(symbolizer);
  
  TEST_ASSERT_EQUAL(7, getSymbolInteger(symbol));
  TEST_ASSERT_EQUAL(INTEGER, getSymbolId(symbol));
  TEST_ASSERT_EQUAL(OPERAND, getSymbolType(symbol));
  TEST_ASSERT_EQUAL(NUMBER, getSymbolArity(symbol));
  
  TEST_ASSERT_EQUAL(INTEGER, symbolizer->lastSymbolId);
  
  freeSymbol(symbol);
  
  symbol = symbolizerGetSymbol(symbolizer);
  
  TEST_ASSERT_EQUAL(BITWISE_AND, getSymbolId(symbol));
  TEST_ASSERT_EQUAL(OPERATOR, getSymbolType(symbol));
  TEST_ASSERT_EQUAL(INFIX, getSymbolArity(symbol));

  TEST_ASSERT_EQUAL(BITWISE_AND, symbolizer->lastSymbolId);
  
  freeSymbol(symbol);
  freeSymbolizer(symbolizer);
}
*/

void  test_shuntingYard_given_1_operator_2_operands_expect_correct_value_121_returned(){
  Tokenizer *tokenizer = createTokenizer(" 33+88");
  
  StackStruct *operandStack = createStack();
  StackStruct *operatorStack = createStack();
  
  shuntingYard(tokenizer, operatorStack, operandStack);
  
  ListItem  *peekItem = peekTopOfStack(operandStack);
  
  TEST_ASSERT_EQUAL(121, getItemSymbolInteger(peekItem));
  
  freeStack(operandStack, freeSymbolInLinkedList); 
  freeStack(operatorStack, freeSymbolInLinkedList); 
}

void  test_shuntingYard_given_2_operator_4_operands_expect_correct_value_42_returned(){
  Tokenizer *tokenizer = createTokenizer(" 12+41-11");
  
  StackStruct *operandStack = createStack();
  StackStruct *operatorStack = createStack();
  
  shuntingYard(tokenizer, operatorStack, operandStack);
  
  ListItem  *peekItem = peekTopOfStack(operandStack);
  
  TEST_ASSERT_EQUAL(42, getItemSymbolInteger(peekItem));
  
  freeStack(operandStack, freeSymbolInLinkedList); 
  freeStack(operatorStack, freeSymbolInLinkedList); 
}

void  test_shuntingYard_given_2_operator_4_operands_with_ftpoint_expect_correct_value_31point777_returned(){
  Tokenizer *tokenizer = NULL;
  tokenizer = createTokenizer("    19.777  +4 * 3");
  
  StackStruct *operandStack = createStack();
  StackStruct *operatorStack = createStack();
  
  shuntingYard(tokenizer, operatorStack, operandStack);
  
  ListItem  *peekItem = peekTopOfStack(operandStack);
  
  TEST_ASSERT_EQUAL_FLOAT(31.777, getItemSymbolDouble(peekItem));
  
  freeStack(operandStack, freeSymbolInLinkedList); 
  freeStack(operatorStack, freeSymbolInLinkedList); 
}

void  test_shuntingYard_given_3_operator_6_operands_with_ftpoint_expect_correct_value_5point5_returned(){
  Tokenizer *tokenizer = NULL;
  tokenizer = createTokenizer("   9-7  /4*2");
  
  StackStruct *operandStack = createStack();
  StackStruct *operatorStack = createStack();
  
  shuntingYard(tokenizer, operatorStack, operandStack);
  
  ListItem  *peekItem = peekTopOfStack(operandStack);
  
  TEST_ASSERT_EQUAL_FLOAT(5.5, getItemSymbolDouble(peekItem));
  
  freeStack(operandStack, freeSymbolInLinkedList); 
  freeStack(operatorStack, freeSymbolInLinkedList); 
}

void  test_shuntingYard_given_4_operator_5_operands_with_bracket_expect_correct_value_48_returned(){
  Tokenizer *tokenizer = NULL;
  tokenizer = createTokenizer("  6*(5+5-2/1)");
  
  StackStruct *operandStack = createStack();
  StackStruct *operatorStack = createStack();
  
  shuntingYard(tokenizer, operatorStack, operandStack);
  
  ListItem  *peekItem = peekTopOfStack(operandStack);
  
  TEST_ASSERT_EQUAL(48, getItemSymbolInteger(peekItem));
  
  freeStack(operandStack, freeSymbolInLinkedList); 
  freeStack(operatorStack, freeSymbolInLinkedList); 
}

void  test_shuntingYard_given_pre_INC_expect_correct_value_6_returned(){
  Tokenizer *tokenizer = NULL;
  tokenizer = createTokenizer("  ++5");
  
  StackStruct *operandStack = createStack();
  StackStruct *operatorStack = createStack();
  
  shuntingYard(tokenizer, operatorStack, operandStack);
  
  ListItem  *peekItem = peekTopOfStack(operandStack);
  
  TEST_ASSERT_EQUAL(6, getItemSymbolInteger(peekItem));
  
  freeStack(operandStack, freeSymbolInLinkedList); 
  freeStack(operatorStack, freeSymbolInLinkedList); 
}

void  test_shuntingYard_given_post_INC_expect_correct_value_6_returned(){
  Tokenizer *tokenizer = NULL;
  tokenizer = createTokenizer("  15++");
  
  StackStruct *operandStack = createStack();
  StackStruct *operatorStack = createStack();
  
  shuntingYard(tokenizer, operatorStack, operandStack);
  
  ListItem  *peekItem = peekTopOfStack(operandStack);
  
  TEST_ASSERT_EQUAL(16, getItemSymbolInteger(peekItem));
  
  freeStack(operandStack, freeSymbolInLinkedList); 
  freeStack(operatorStack, freeSymbolInLinkedList); 
}


void  test_shuntingYard_given_prefix_minus_pre_INC_expect_correct_value_minus_4_returned(){
  Tokenizer *tokenizer = NULL;
  tokenizer = createTokenizer("  -++3");
  
  StackStruct *operandStack = createStack();
  StackStruct *operatorStack = createStack();
  
  shuntingYard(tokenizer, operatorStack, operandStack);
  
  ListItem  *peekItem = peekTopOfStack(operandStack);
  
  TEST_ASSERT_EQUAL(-4, getItemSymbolInteger(peekItem));
  
  freeStack(operandStack, freeSymbolInLinkedList); 
  freeStack(operatorStack, freeSymbolInLinkedList); 
}

void  test_shuntingYard_given_prefix_plus_pre_INC_expect_correct_value_4_returned(){
  Tokenizer *tokenizer = NULL;
  tokenizer = createTokenizer("  + ++3");
  
  StackStruct *operandStack = createStack();
  StackStruct *operatorStack = createStack();
  
  shuntingYard(tokenizer, operatorStack, operandStack);
  
  ListItem  *peekItem = peekTopOfStack(operandStack);
  
  TEST_ASSERT_EQUAL(4, getItemSymbolInteger(peekItem));
  
  freeStack(operandStack, freeSymbolInLinkedList); 
  freeStack(operatorStack, freeSymbolInLinkedList); 
}

void  test_shuntingYard_given_multiply_plus_shift_left_expect_correct_value_7_returned(){
  Tokenizer *tokenizer = NULL;
  tokenizer = createTokenizer(" 6* 5  + 1 >>2");
  
  StackStruct *operandStack = createStack();
  StackStruct *operatorStack = createStack();
  
  shuntingYard(tokenizer, operatorStack, operandStack);
  
  ListItem  *peekItem = peekTopOfStack(operandStack);
  
  TEST_ASSERT_EQUAL(7, getItemSymbolInteger(peekItem));
  
  freeStack(operandStack, freeSymbolInLinkedList); 
  freeStack(operatorStack, freeSymbolInLinkedList); 
}

void  test_shuntingYard_given_prefix_inc_infix_minus_prefix_plus_multiply_expect_correct_value_0_returned(){
  Tokenizer *tokenizer = NULL;
  tokenizer = createTokenizer(" ++5-+3*2");
  
  StackStruct *operandStack = createStack();
  StackStruct *operatorStack = createStack();
  
  shuntingYard(tokenizer, operatorStack, operandStack);
  
  ListItem  *peekItem = peekTopOfStack(operandStack);
  
  TEST_ASSERT_EQUAL(0, getItemSymbolInteger(peekItem));
  
  freeStack(operandStack, freeSymbolInLinkedList); 
  freeStack(operatorStack, freeSymbolInLinkedList); 
}

void  test_shuntingYard_given_multiply_post_inc_infix_minus_pre_inc_expect_correct_value_seven_returned(){
  Tokenizer *tokenizer = NULL;
  tokenizer = createTokenizer("3*2++-++1");
  
  StackStruct *operandStack = createStack();
  StackStruct *operatorStack = createStack();
  
  shuntingYard(tokenizer, operatorStack, operandStack);
  
  ListItem  *peekItem = peekTopOfStack(operandStack);
  
  TEST_ASSERT_EQUAL(7, getItemSymbolInteger(peekItem));
  
  freeStack(operandStack, freeSymbolInLinkedList); 
  freeStack(operatorStack, freeSymbolInLinkedList); 
}

void  test_shuntingYard_given_post_inc_infix_plus_prefix_minus_infix_minus_multiply_expect_correct_value_minus4_returned(){
  Tokenizer *tokenizer = NULL;
  tokenizer = createTokenizer("3+++-2-3*2");
  
  StackStruct *operandStack = createStack();
  StackStruct *operatorStack = createStack();
  
  shuntingYard(tokenizer, operatorStack, operandStack);
  
  ListItem  *peekItem = peekTopOfStack(operandStack);
  
  TEST_ASSERT_EQUAL(-4, getItemSymbolInteger(peekItem));
  
  freeStack(operandStack, freeSymbolInLinkedList); 
  freeStack(operatorStack, freeSymbolInLinkedList); 
} 

void  test_shuntingYard_given_post_dec_infix_minus_divide_expect_correct_value_minus0_returned(){
  Tokenizer *tokenizer = NULL;
  tokenizer = createTokenizer("5---1/3");
  
  StackStruct *operandStack = createStack();
  StackStruct *operatorStack = createStack();
  
  shuntingYard(tokenizer, operatorStack, operandStack);
  
  ListItem  *peekItem = peekTopOfStack(operandStack);
  
  TEST_ASSERT_EQUAL_FLOAT(4.0-1.0/3.0, getItemSymbolDouble(peekItem));
  
  freeStack(operandStack, freeSymbolInLinkedList); 
  freeStack(operatorStack, freeSymbolInLinkedList); 
} 

void  test_shuntingYard_given_infix_minus_bracket_prefix_dec_divide_expect_correct_value_5_returned(){
  Tokenizer *tokenizer = NULL;
  tokenizer = createTokenizer("5-(--1)/3");
  
  StackStruct *operandStack = createStack();
  StackStruct *operatorStack = createStack();
  
  shuntingYard(tokenizer, operatorStack, operandStack);
  
  ListItem  *peekItem = peekTopOfStack(operandStack);
  
  TEST_ASSERT_EQUAL(5-0/3, getItemSymbolInteger(peekItem));
  
  freeStack(operandStack, freeSymbolInLinkedList); 
  freeStack(operatorStack, freeSymbolInLinkedList);  
} 

void  test_shuntingYard_given_multiply_divide_expect_exception_to_be_thrown(){
  Tokenizer *tokenizer = NULL;
  tokenizer = createTokenizer("5* /3");
  
  StackStruct *operandStack = createStack();
  StackStruct *operatorStack = createStack();
  
  Try{
    shuntingYard(tokenizer, operatorStack, operandStack);
    TEST_FAIL_MESSAGE("EXPECT ERROR_INVALID_INFIX TO BE THROWN, BUT UNRECEIVED");
  }Catch(ex){
    dumpSymbolErrorMessage(ex, __LINE__); 
    TEST_ASSERT_EQUAL(ERROR_INVALID_INFIX, ex->errorCode);
    freeException(ex);
  }
  freeStack(operandStack, freeSymbolInLinkedList); 
  freeStack(operatorStack, freeSymbolInLinkedList);   
}

void  test_shuntingYard_given_multiply_pre_inc_exception_to_be_thrown(){
  Tokenizer *tokenizer = NULL;
  tokenizer = createTokenizer("  *  ++3");
  
  StackStruct *operandStack = createStack();
  StackStruct *operatorStack = createStack();
  
  Try{
    shuntingYard(tokenizer, operatorStack, operandStack);
    TEST_FAIL_MESSAGE("EXPECT ERROR_INVALID_INFIX TO BE THROWN, BUT UNRECEIVED");
  }Catch(ex){
    dumpSymbolErrorMessage(ex, __LINE__); 
    TEST_ASSERT_EQUAL(ERROR_INVALID_INFIX, ex->errorCode);
    freeException(ex);
  }
  freeStack(operandStack, freeSymbolInLinkedList); 
  freeStack(operatorStack, freeSymbolInLinkedList);   
}

void  test_verifyArityAllowable_expect_exception_to_be_thrown(){
  Tokenizer *tokenizer = NULL;
  tokenizer = createTokenizer("5* /3");

  Symbolizer  *symbolizer = createSymbolizer(tokenizer);
  Symbol  *symbol1 = getSymbol(symbolizer);
  Symbol  *symbol2 = getSymbol(symbolizer);
  Symbol  *symbol3 = getSymbol(symbolizer);
  
  symbolizer->lastSymbol = symbol2;
  
   Try{
    verifyArityAllowable(symbolizer, symbol3, symbol3->id);
    TEST_FAIL_MESSAGE("EXPECT ERROR_INVALID_INFIX TO BE THROWN, BUT UNRECEIVED");
  }Catch(ex){
    dumpSymbolErrorMessage(ex, __LINE__); 
    TEST_ASSERT_EQUAL(ERROR_INVALID_INFIX, ex->errorCode);
    freeException(ex);
  } 
  freeSymbol(symbol1);
}

void  test_shuntingYard_given_prefix_followed_after_suffix_expect_exception_to_be_thrown(){
  Tokenizer *tokenizer = NULL;
  tokenizer = createTokenizer(" 3*1+ 2-- ++1");
  
  StackStruct *operandStack = createStack();
  StackStruct *operatorStack = createStack();
  
  Try{
    shuntingYard(tokenizer, operatorStack, operandStack);
    TEST_FAIL_MESSAGE("EXPECT ERROR_INVALID_PREFIX TO BE THROWN, BUT UNRECEIVED");
  }Catch(ex){
    dumpSymbolErrorMessage(ex, __LINE__); 
    TEST_ASSERT_EQUAL(ERROR_INVALID_PREFIX, ex->errorCode);
    freeException(ex);
  }
  freeStack(operandStack, freeSymbolInLinkedList); 
  freeStack(operatorStack, freeSymbolInLinkedList);   
}

void  test_shuntingYard_given_suffix_followed_after_suffix_expect_exception_to_be_thrown(){
  Tokenizer *tokenizer = NULL;
  tokenizer = createTokenizer(" (1+2*)");
  
  StackStruct *operandStack = createStack();
  StackStruct *operatorStack = createStack();
  
  Try{
    shuntingYard(tokenizer, operatorStack, operandStack);
    TEST_FAIL_MESSAGE("EXPECT ERROR_INVALID_SUFFIX TO BE THROWN, BUT UNRECEIVED");
  }Catch(ex){
    dumpSymbolErrorMessage(ex, __LINE__); 
    TEST_ASSERT_EQUAL(ERROR_INVALID_SUFFIX, ex->errorCode);
    freeException(ex);
  }
  freeStack(operandStack, freeSymbolInLinkedList); 
  freeStack(operatorStack, freeSymbolInLinkedList);   
}

void  test_shuntingYard_given_missing_open_paren_expect_exception_to_be_thrown(){
  Tokenizer *tokenizer = NULL;
  tokenizer = createTokenizer(" 3+ 5/67)");
  
  StackStruct *operandStack = createStack();
  StackStruct *operatorStack = createStack();
  
  Try{
    shuntingYard(tokenizer, operatorStack, operandStack);
    TEST_FAIL_MESSAGE("EXPECT ERROR_MISSING_OPEN_PAREN TO BE THROWN, BUT UNRECEIVED");
  }Catch(ex){
    dumpSymbolErrorMessage(ex, __LINE__); 
    TEST_ASSERT_EQUAL(ERROR_MISSING_OPEN_PAREN, ex->errorCode);
    freeException(ex);
  }
  freeStack(operandStack, freeSymbolInLinkedList); 
  freeStack(operatorStack, freeSymbolInLinkedList);   
}

void  test_shuntingYard_given_complex_missing_open_paren_expect_exception_to_be_thrown(){
  Tokenizer *tokenizer = NULL;
  tokenizer = createTokenizer("(5-3*6/12*(3-3)))");
  
  StackStruct *operandStack = createStack();
  StackStruct *operatorStack = createStack();
  
  Try{
    shuntingYard(tokenizer, operatorStack, operandStack);
    TEST_FAIL_MESSAGE("EXPECT ERROR_MISSING_OPEN_PAREN TO BE THROWN, BUT UNRECEIVED");
  }Catch(ex){
    dumpSymbolErrorMessage(ex, __LINE__); 
    TEST_ASSERT_EQUAL(ERROR_MISSING_OPEN_PAREN, ex->errorCode);
    freeException(ex);
  }
  freeStack(operandStack, freeSymbolInLinkedList); 
  freeStack(operatorStack, freeSymbolInLinkedList);   
}

void  test_shuntingYard_given__missing_closing_paren_expect_exception_to_be_thrown(){
  Tokenizer *tokenizer = NULL;
  tokenizer = createTokenizer("1*(3+4-5");
  
  StackStruct *operandStack = createStack();
  StackStruct *operatorStack = createStack();
  
  Try{
    shuntingYard(tokenizer, operatorStack, operandStack);
    TEST_FAIL_MESSAGE("EXPECT ERROR_MISSING_CLOSING_PAREN TO BE THROWN, BUT UNRECEIVED");
  }Catch(ex){
    dumpSymbolErrorMessage(ex, __LINE__); 
    TEST_ASSERT_EQUAL(ERROR_MISSING_CLOSING_PAREN, ex->errorCode);
    freeException(ex);
  }
  freeStack(operandStack, freeSymbolInLinkedList); 
  freeStack(operatorStack, freeSymbolInLinkedList);   
}

void  test_shuntingYard_given_complex_missing_closing_paren_expect_exception_to_be_thrown(){
  Tokenizer *tokenizer = NULL;
  tokenizer = createTokenizer("1-2/4+(5*(8-999)*10");
  
  StackStruct *operandStack = createStack();
  StackStruct *operatorStack = createStack();
  
  Try{
    shuntingYard(tokenizer, operatorStack, operandStack);
    TEST_FAIL_MESSAGE("EXPECT ERROR_MISSING_CLOSING_PAREN TO BE THROWN, BUT UNRECEIVED");
  }Catch(ex){
    dumpSymbolErrorMessage(ex, __LINE__); 
    TEST_ASSERT_EQUAL(ERROR_MISSING_CLOSING_PAREN, ex->errorCode);
    freeException(ex);
  }
  freeStack(operandStack, freeSymbolInLinkedList); 
  freeStack(operatorStack, freeSymbolInLinkedList);   
}

void  test_shuntingYard_given_invalid_number_position_expect_exception_to_be_thrown(){
  Tokenizer *tokenizer = NULL;
  tokenizer = createTokenizer("(5*1++2)-3");
  
  StackStruct *operandStack = createStack();
  StackStruct *operatorStack = createStack();
  
  Try{
    shuntingYard(tokenizer, operatorStack, operandStack);
    TEST_FAIL_MESSAGE("EXPECT ERROR_INVALID_NUMBER TO BE THROWN, BUT UNRECEIVED");
  }Catch(ex){
    dumpSymbolErrorMessage(ex, __LINE__); 
    TEST_ASSERT_EQUAL(ERROR_INVALID_NUMBER, ex->errorCode);
    freeException(ex);
  }
  freeStack(operandStack, freeSymbolInLinkedList); 
  freeStack(operatorStack, freeSymbolInLinkedList);   
}

void  test_shuntingYard_given_invalid_number_after_number_expect_exception_to_be_thrown(){
  Tokenizer *tokenizer = NULL;
  tokenizer = createTokenizer("1*1 3+53");
  
  StackStruct *operandStack = createStack();
  StackStruct *operatorStack = createStack();
  
  Try{
    shuntingYard(tokenizer, operatorStack, operandStack);
    TEST_FAIL_MESSAGE("EXPECT ERROR_INVALID_NUMBER TO BE THROWN, BUT UNRECEIVED");
  }Catch(ex){
    dumpSymbolErrorMessage(ex, __LINE__); 
    TEST_ASSERT_EQUAL(ERROR_INVALID_NUMBER, ex->errorCode);
    freeException(ex);
  }
  freeStack(operandStack, freeSymbolInLinkedList); 
  freeStack(operatorStack, freeSymbolInLinkedList);   
}

void  test_shuntingYard_given_multiple_brackets_with_prefixes_expect_calculate_correctly(){
  Tokenizer *tokenizer = NULL;
  tokenizer = createTokenizer(" -((-+-(-2)* + -2) - +9)");
  
  StackStruct *operandStack = createStack();
  StackStruct *operatorStack = createStack();
  
  shuntingYard(tokenizer, operatorStack, operandStack);
  
  ListItem  *peekItem = peekTopOfStack(operandStack);
  
  TEST_ASSERT_EQUAL(-((-+-(-2)* + -2) - +9), getItemSymbolInteger(peekItem));
  
  freeStack(operandStack, freeSymbolInLinkedList); 
  freeStack(operatorStack, freeSymbolInLinkedList); 
}

void  test_shuntingYard_given_prefix_inc_dec_expect_calculate_correctly(){
  Tokenizer *tokenizer = NULL;
  tokenizer = createTokenizer(" 3++ *2 /4 + --2 + ++-12-+-3");
  
  StackStruct *operandStack = createStack();
  StackStruct *operatorStack = createStack();
  
  shuntingYard(tokenizer, operatorStack, operandStack);
  
  ListItem  *peekItem = peekTopOfStack(operandStack);
  
  TEST_ASSERT_EQUAL_FLOAT(4 *2 /4 + 1 + -11-+-3, getItemSymbolInteger(peekItem));
  
  freeStack(operandStack, freeSymbolInLinkedList); 
  freeStack(operatorStack, freeSymbolInLinkedList); 
}

void  test_shuntingYard_given_prefix_PLUS_MINUS_expect_calculate_correctly(){
  Tokenizer *tokenizer = NULL;
  tokenizer = createTokenizer(" +-3");
  
  StackStruct *operandStack = createStack();
  StackStruct *operatorStack = createStack();
  
  shuntingYard(tokenizer, operatorStack, operandStack);
  
  ListItem  *peekItem = peekTopOfStack(operandStack);
  
  TEST_ASSERT_EQUAL_FLOAT(-3, getItemSymbolInteger(peekItem));
  
  freeStack(operandStack, freeSymbolInLinkedList); 
  freeStack(operatorStack, freeSymbolInLinkedList); 
}

void  test_shuntingYard_given_infix_MINUS_expect_calculate_correctly(){
  Tokenizer *tokenizer = NULL;
  tokenizer = createTokenizer("3-6");
  
  StackStruct *operandStack = createStack();
  StackStruct *operatorStack = createStack();
  
  shuntingYard(tokenizer, operatorStack, operandStack);
  
  ListItem  *peekItem = peekTopOfStack(operandStack);
  
  TEST_ASSERT_EQUAL_FLOAT(-3, getItemSymbolInteger(peekItem));
  
  freeStack(operandStack, freeSymbolInLinkedList); 
  freeStack(operatorStack, freeSymbolInLinkedList); 
}

void  test_shuntingYard_given_suffix_dec_infix_minus_shift_left_multiply_expect_correct_value_returned(){
  Tokenizer *tokenizer = NULL;
  tokenizer = createTokenizer(" 19---3*5<<1*2");
  
  StackStruct *operandStack = createStack();
  StackStruct *operatorStack = createStack();
  
  shuntingYard(tokenizer, operatorStack, operandStack);
  
  ListItem  *peekItem = peekTopOfStack(operandStack);
  
  TEST_ASSERT_EQUAL(18-3*5<<1*2, getItemSymbolInteger(peekItem));
  
  freeStack(operandStack, freeSymbolInLinkedList); 
  freeStack(operatorStack, freeSymbolInLinkedList);  
}

void  test_shuntingYard_given_infix_plus_greater_infix_minus_right_shift_expect_correct_value_returned(){
  Tokenizer *tokenizer = NULL;
  tokenizer = createTokenizer(" 13 + 6 > 7 - 2 >> 1");
  
  StackStruct *operandStack = createStack();
  StackStruct *operatorStack = createStack();
  
  shuntingYard(tokenizer, operatorStack, operandStack);
  
  ListItem  *peekItem = peekTopOfStack(operandStack);
  
  TEST_ASSERT_EQUAL(13 + 6 > 7 - 2 >> 1, getItemSymbolInteger(peekItem));
  
  freeStack(operandStack, freeSymbolInLinkedList); 
  freeStack(operatorStack, freeSymbolInLinkedList);  
}

//If double passed into the logix that only receive integer, it will forced to int before evaluation
void  test_shuntingYard_given_infix_shift_left_with_double_expect_correct_value_returned(){
  Tokenizer *tokenizer = NULL;
  tokenizer = createTokenizer(" 8.97 >> 1.12");
  
  StackStruct *operandStack = createStack();
  StackStruct *operatorStack = createStack();
  
  shuntingYard(tokenizer, operatorStack, operandStack);
  
  ListItem  *peekItem = peekTopOfStack(operandStack);
  
  TEST_ASSERT_EQUAL(4, getItemSymbolInteger(peekItem));
  
  freeStack(operandStack, freeSymbolInLinkedList); 
  freeStack(operatorStack, freeSymbolInLinkedList);  
}

void  test_shuntingYard_given_complex_brackets_correct_value_returned(){
  Tokenizer *tokenizer = NULL;
  tokenizer = createTokenizer("((2+--2)*2+- -2)*(+-2*2/-2)*(100/--+10)");
  
  StackStruct *operandStack = createStack();
  StackStruct *operatorStack = createStack();
  
  shuntingYard(tokenizer, operatorStack, operandStack);
  
  ListItem  *peekItem = peekTopOfStack(operandStack);
  
  TEST_ASSERT_EQUAL_FLOAT(((2.0+1.0)*2.0+- -2.0)*(+-2.0*2.0/-2.0)*(100.0/9.0), getItemSymbolDouble(peekItem));
  
  freeStack(operandStack, freeSymbolInLinkedList); 
  freeStack(operatorStack, freeSymbolInLinkedList);  
}

//1-4+-+-+-5+63*+-+-+-(12+1*(2-2+3*3))/2
void  test_shuntingYard_given_tons_of_prefix_plus_minus_correct_value_returned(){
  Tokenizer *tokenizer = NULL;
  tokenizer = createTokenizer("1-4+-+-+-5+63*+-+-+-(12+1*(2-2+3*3))/2");
  
  StackStruct *operandStack = createStack();
  StackStruct *operatorStack = createStack();
  
  shuntingYard(tokenizer, operatorStack, operandStack);
  
  ListItem  *peekItem = peekTopOfStack(operandStack);
  
  TEST_ASSERT_EQUAL_FLOAT(1-4+-+-+-5+63*+-+-+-(12+1*(2-2+3*3))/2.0, getItemSymbolDouble(peekItem));
  
  freeStack(operandStack, freeSymbolInLinkedList); 
  freeStack(operatorStack, freeSymbolInLinkedList);  
}

//"9*(9*(1-5*(5+4/(3*6+4-(9+3/2)))))"
void  test_shuntingYard_given_brackets_in_brackets_expect_correct_value_returned(){
  Tokenizer *tokenizer = NULL;
  tokenizer = createTokenizer("9*(9* (1-   5*( 5+ 4/( 3*6+  4- (9+ 3.0 / 2.0)))))");
  
  StackStruct *operandStack = createStack();
  StackStruct *operatorStack = createStack();
  
  shuntingYard(tokenizer, operatorStack, operandStack);
  
  ListItem  *peekItem = peekTopOfStack(operandStack);
  
  TEST_ASSERT_EQUAL_FLOAT(9*(9*(1-5*(5+4/(3.0*6.0+4-(9+3.0/2.0))))), getItemSymbolDouble(peekItem));
  
  freeStack(operandStack, freeSymbolInLinkedList); 
  freeStack(operatorStack, freeSymbolInLinkedList);  
}

//1.435+3.565-6.674/4*7
void  test_shuntingYard_given_double_values_expect_correct_value_returned(){
  Tokenizer *tokenizer = NULL;
  tokenizer = createTokenizer("1.435+3.565-6.674/4*7");
  
  StackStruct *operandStack = createStack();
  StackStruct *operatorStack = createStack();
  
  shuntingYard(tokenizer, operatorStack, operandStack);
  
  ListItem  *peekItem = peekTopOfStack(operandStack);
  
  TEST_ASSERT_EQUAL_FLOAT(1.435+3.565-6.674/4*7, getItemSymbolDouble(peekItem));
  
  freeStack(operandStack, freeSymbolInLinkedList); 
  freeStack(operatorStack, freeSymbolInLinkedList);  
}

//-+-12.56363-6.7/(-8.999*4.65+12.2-(- -8.963+-+4*5.3))
void  test_shuntingYard_given_complex_double_values_expect_correct_value_returned(){
  Tokenizer *tokenizer = NULL;
  tokenizer = createTokenizer("-+-12.56363-6.7/(-8.999*4.65+12.2-(- -8.963+-+4*5.3))");
  
  StackStruct *operandStack = createStack();
  StackStruct *operatorStack = createStack();
  
  shuntingYard(tokenizer, operatorStack, operandStack);
  
  ListItem  *peekItem = peekTopOfStack(operandStack);
  
  TEST_ASSERT_EQUAL_FLOAT(-+-12.56363-6.7/(-8.999*4.65+12.2-(- -8.963+-+4*5.3)), getItemSymbolDouble(peekItem));
  
  freeStack(operandStack, freeSymbolInLinkedList); 
  freeStack(operatorStack, freeSymbolInLinkedList);  
}

void  test_shuntingYard_given_mixed_integer_double_values_expect_correct_value_returned(){
  Tokenizer *tokenizer = NULL;
  tokenizer = createTokenizer(" (1+3.2)*0.453+(6/(5*105)-4)");
  
  StackStruct *operandStack = createStack();
  StackStruct *operatorStack = createStack();
  
  shuntingYard(tokenizer, operatorStack, operandStack);
  
  ListItem  *peekItem = peekTopOfStack(operandStack);
  
  TEST_ASSERT_EQUAL_FLOAT((1+3.2)*0.453+(6/(5*105.0)-4.0), getItemSymbolDouble(peekItem));
  
  freeStack(operandStack, freeSymbolInLinkedList); 
  freeStack(operatorStack, freeSymbolInLinkedList);  
}