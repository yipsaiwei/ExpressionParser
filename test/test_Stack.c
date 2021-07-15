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

CEXCEPTION_T  ex;

void setUp(void)
{
}

void tearDown(void)
{
}

void test_Stack_NeedToImplement(void)
{
    TEST_IGNORE_MESSAGE("Need to Implement Stack");
}

void  test_createStack_expect_size_0_list_points_to_NULL(void){
  StackStruct *stack = createStack();
  TEST_ASSERT_EQUAL(0, stack->size);
  TEST_ASSERT_EQUAL(NULL, stack->list);
  
  freeStack(stack, free);
}

void  test_pushToStack_given_numbers_empty_stack_expect_size_1_list_points_to_data(void){
  StackStruct *stack = createStack();
  Data  *data = memAlloc(sizeof(Data));
  data->val1 = 5;
  data->val2 = 1.999;
  pushToStack(stack, (void  *)data);
  TEST_ASSERT_EQUAL(1, stack->size);
  TEST_ASSERT_EQUAL_PTR(data, stack->list->head->data);
  Data *testData = stack->list->head->data;
  TEST_ASSERT_EQUAL(5, testData->val1);
  TEST_ASSERT_EQUAL(1.999, testData->val2);
  
  freeStack(stack, free);
  }

void  test_pushToStack_given_2_elements_expect_size_2_head_points_to_idata2(){
  StackStruct *stack = createStack();
  Data  *data = memAlloc(sizeof(Data));
  data->val1 = 5;
  data->val2 = 1.999;
  pushToStack(stack, (void  *)data);
  TEST_ASSERT_EQUAL(1, stack->size);
  TEST_ASSERT_EQUAL_PTR(data, stack->list->head->data);
  Data *testData = stack->list->head->data;
  TEST_ASSERT_EQUAL(5, testData->val1);
  TEST_ASSERT_EQUAL(1.999, testData->val2);
  
  Data  *data2 = memAlloc(sizeof(Data));
  data2->val1 = 10;
  data2->val2 = 2.33333;
  pushToStack(stack, (void  *)data2);
  TEST_ASSERT_EQUAL(2, stack->size);
  TEST_ASSERT_EQUAL_PTR(data2, stack->list->head->data);
  testData = stack->list->head->data;
  TEST_ASSERT_EQUAL(10, testData->val1);
  TEST_ASSERT_EQUAL(2.33333, testData->val2);
  
  freeStack(stack, free);
}

void  test_pushToStack_given_3_elements_expect_size_3_head_points_to_item_3(){
  StackStruct *stack = createStack();
  Data  *data = memAlloc(sizeof(Data));
  data->val1 = 5;
  data->val2 = 1.999;
  pushToStack(stack, (void  *)data);
  
  Data  *data2 = memAlloc(sizeof(Data));
  data2->val1 = 10;
  data2->val2 = 2.33333;
  pushToStack(stack, (void  *)data2);
  
  Data  *data3 = memAlloc(sizeof(Data));
  data3->val1 = 20;
  data3->val2 = 67.6767;
  pushToStack(stack, (void  *)data3);
  TEST_ASSERT_EQUAL(3, stack->size);
  Data  *testData = stack->list->head->data;
  TEST_ASSERT_EQUAL(20, testData->val1);
  TEST_ASSERT_EQUAL(67.6767, testData->val2);
  
  freeStack(stack, free);
  
}

void  test_popFromStack_given_1_elements_expect_size_0_head_points_to_NULL(){
  StackStruct *stack = createStack();
  Data  *data = memAlloc(sizeof(Data));
  data->val1 = 5;
  data->val2 = 1.999;
  pushToStack(stack, (void  *)data);
  TEST_ASSERT_EQUAL(1, stack->size);
  TEST_ASSERT_EQUAL_PTR(data, stack->list->head->data);
  Data *testData = stack->list->head->data;
  TEST_ASSERT_EQUAL(5, testData->val1);
  TEST_ASSERT_EQUAL(1.999, testData->val2);
  
  ListItem  *popItem = NULL;
  popItem = popFromStack(stack);
  TEST_ASSERT_EQUAL(0, stack->size);
  testData = popItem->data;
  TEST_ASSERT_EQUAL(5, testData->val1);
  TEST_ASSERT_EQUAL(1.999, testData->val2);
  
  linkedListFreeListItem(popItem);
  freeStack(stack, free);
}

void  test_popFromStack_given_2_elements_expect_all_able_to_pop(){
  StackStruct *stack = createStack();
  Data  *data = memAlloc(sizeof(Data));
  data->val1 = 5;
  data->val2 = 1.999;
  pushToStack(stack, (void  *)data);
  TEST_ASSERT_EQUAL(1, stack->size);
  TEST_ASSERT_EQUAL_PTR(data, stack->list->head->data);
  Data *testData = stack->list->head->data;
  TEST_ASSERT_EQUAL(5, testData->val1);
  TEST_ASSERT_EQUAL(1.999, testData->val2);
  
  Data  *data2 = memAlloc(sizeof(Data));
  data2->val1 = 10;
  data2->val2 = 2.33333;
  pushToStack(stack, (void  *)data2);
  TEST_ASSERT_EQUAL(2, stack->size);
  TEST_ASSERT_EQUAL_PTR(data2, stack->list->head->data);
  testData = stack->list->head->data;
  TEST_ASSERT_EQUAL(10, testData->val1);
  TEST_ASSERT_EQUAL(2.33333, testData->val2);

  ListItem  *popItem = NULL;
  popItem = popFromStack(stack);
  TEST_ASSERT_EQUAL(1, stack->size);
  testData = popItem->data;
  TEST_ASSERT_EQUAL(10, testData->val1);
  TEST_ASSERT_EQUAL(2.33333, testData->val2);
  
  linkedListFreeListItem(popItem);
  
  popItem = popFromStack(stack);
  testData = popItem->data;
  TEST_ASSERT_EQUAL(5, testData->val1);
  TEST_ASSERT_EQUAL(1.999, testData->val2);
  
  linkedListFreeListItem(popItem);
  
  freeStack(stack, free);
}

void  test_popFromStack_expect_ERROR_EMPTY_STACK(void){
  StackStruct *stack = createStack();
  TEST_ASSERT_EQUAL(0, stack->size);
  TEST_ASSERT_EQUAL(NULL, stack->list);
  ListItem  *popItem = NULL;
  
  Try{
    popItem = popFromStack(stack);
    TEST_FAIL_MESSAGE("EXPECT ERROR_EMPTY_STACK TO BE THROWN, BUT UNRECEIVED");
  }Catch(ex){
    dumpException(ex);
		TEST_ASSERT_EQUAL(ERROR_STACK_IS_EMPTY, ex->errorCode);
		freeException(ex);    
  }
  freeStack(stack, free);
}

void  test_popFromStack_given_1_elements_pop_2_times_expect_ERROR_STACK_IS_EMPTY_to_be_thrown(){
  StackStruct *stack = createStack();
  Data  *data = memAlloc(sizeof(Data));
  data->val1 = 5;
  data->val2 = 1.999;
  pushToStack(stack, (void  *)data);
  TEST_ASSERT_EQUAL(1, stack->size);
  TEST_ASSERT_EQUAL_PTR(data, stack->list->head->data);
  Data *testData = stack->list->head->data;
  TEST_ASSERT_EQUAL(5, testData->val1);
  TEST_ASSERT_EQUAL(1.999, testData->val2);
  
  ListItem  *popItem = NULL;
  popItem = popFromStack(stack);
  TEST_ASSERT_EQUAL(0, stack->size);
  testData = popItem->data;
  TEST_ASSERT_EQUAL(5, testData->val1);
  TEST_ASSERT_EQUAL(1.999, testData->val2);
  
  Try{
    popItem = popFromStack(stack);
    TEST_FAIL_MESSAGE("EXPECT ERROR_EMPTY_STACK TO BE THROWN, BUT UNRECEIVED");
  }Catch(ex){
    dumpException(ex);
		TEST_ASSERT_EQUAL(ERROR_STACK_IS_EMPTY, ex->errorCode);
		freeException(ex);    
  }
  freeStack(stack, free);
}

void  test_popFromStack_given_2_elements_pop_3_times_expect_ERROR_STACK_IS_EMPTY_to_be_thrown(){
  StackStruct *stack = createStack();
  Data  *data = memAlloc(sizeof(Data));
  data->val1 = 17;
  data->val2 = 5.5836;
  pushToStack(stack, (void  *)data);
  TEST_ASSERT_EQUAL(1, stack->size);
  TEST_ASSERT_EQUAL_PTR(data, stack->list->head->data);
  Data *testData = stack->list->head->data;
  TEST_ASSERT_EQUAL(17, testData->val1);
  TEST_ASSERT_EQUAL(5.5836, testData->val2);
  
  Data  *data2 = memAlloc(sizeof(Data));
  data2->val1 = 99;
  data2->val2 = 777.14;
  pushToStack(stack, (void  *)data2);
  TEST_ASSERT_EQUAL(2, stack->size);
  TEST_ASSERT_EQUAL_PTR(data2, stack->list->head->data);
  testData = stack->list->head->data;
  TEST_ASSERT_EQUAL(99, testData->val1);
  TEST_ASSERT_EQUAL(777.14, testData->val2);
  
  ListItem  *popItem = NULL;
  popItem = popFromStack(stack);
  TEST_ASSERT_EQUAL(1, stack->size);
  testData = popItem->data;
  TEST_ASSERT_EQUAL(99, testData->val1);
  TEST_ASSERT_EQUAL(777.14, testData->val2);  
  
  linkedListFreeListItem(popItem);
  
  popItem = popFromStack(stack);
  testData = popItem->data;
  TEST_ASSERT_EQUAL(17, testData->val1);
  TEST_ASSERT_EQUAL(5.5836, testData->val2);
  
  linkedListFreeListItem(popItem);
  
  Try{
    popItem = popFromStack(stack);
    TEST_FAIL_MESSAGE("EXPECT ERROR_EMPTY_STACK TO BE THROWN, BUT UNRECEIVED");
  }Catch(ex){
    dumpException(ex);
		TEST_ASSERT_EQUAL(ERROR_STACK_IS_EMPTY, ex->errorCode);
		freeException(ex);    
  }
  freeStack(stack, free);
}

void  test_pushToStack_given_3_tokens_expect_all_able_to_be_pushed(){
  Tokenizer *tokenizer = NULL;
  StackStruct *stack = createStack();
  tokenizer = createTokenizer("   1947/34.32e-2"); 
  IntegerToken *token0 = NULL;
  Try{
    token0 = (IntegerToken  *)getToken(tokenizer);
  
    OperatorToken *token1 = NULL;
    token1 = (OperatorToken  *)getToken(tokenizer);
  
    FloatToken *token2 = NULL;
    token2 = (FloatToken  *)getToken(tokenizer);
    
    pushToStack(stack, (void  *)token0);
    TEST_ASSERT_EQUAL(1, stack->size);
    TEST_ASSERT_EQUAL_PTR(token0, stack->list->head->data);
    IntegerToken  *testIntToken = stack->list->head->data;
    TEST_ASSERT_EQUAL(TOKEN_INTEGER_TYPE, testIntToken->type);
    TEST_ASSERT_EQUAL_STRING("1947", testIntToken->str);
    TEST_ASSERT_EQUAL(1947, testIntToken->value);
    TEST_ASSERT_EQUAL(3, testIntToken->startColumn);    
    
    pushToStack(stack, (void  *)token1);
    TEST_ASSERT_EQUAL(2, stack->size);
    TEST_ASSERT_EQUAL_PTR(token1, stack->list->head->data);
    OperatorToken *testOpToken = stack->list->head->data;
    TEST_ASSERT_EQUAL(TOKEN_OPERATOR_TYPE, testOpToken->type);
    TEST_ASSERT_EQUAL_STRING("/", testOpToken->str);
    TEST_ASSERT_EQUAL(7, testOpToken->startColumn);  

    pushToStack(stack, (void  *)token2);    
    TEST_ASSERT_EQUAL(3, stack->size);
    TEST_ASSERT_EQUAL_PTR(token2, stack->list->head->data);
    FloatToken  *testFloatToken = stack->list->head->data;
    TEST_ASSERT_EQUAL(TOKEN_FLOAT_TYPE, testFloatToken->type);
    TEST_ASSERT_EQUAL_STRING("34.32e-2", testFloatToken->str);
    TEST_ASSERT_EQUAL_FLOAT(34.32e-2, testFloatToken->value);
    TEST_ASSERT_EQUAL(8, testFloatToken->startColumn);    
    
    freeStack(stack, freeToken);
    freeTokenizer(tokenizer);
  }Catch(ex){
    dumpTokenErrorMessage(ex, 1);
    TEST_FAIL_MESSAGE("Do not expect any exception to be thrown.");     
  }
}

void  test_pushToStack_then_popFromStack_given_4_tokens_expect_all_able_to_be_pushed_and_pop(){
  Tokenizer *tokenizer = NULL;
  StackStruct *stack = createStack();
  tokenizer = createTokenizer("   (16/8"); 
  OperatorToken *token0 = NULL;
  Try{
    token0 = (OperatorToken  *)getToken(tokenizer);
  
    IntegerToken *token1 = NULL;
    token1 = (IntegerToken  *)getToken(tokenizer);
  
    OperatorToken *token2 = NULL;
    token2 = (OperatorToken  *)getToken(tokenizer);
    
    IntegerToken  *token3 = NULL;
    token3 = (IntegerToken  *)getToken(tokenizer);
    
    pushToStack(stack, (void  *)token0);
    TEST_ASSERT_EQUAL(1, stack->size);
    TEST_ASSERT_EQUAL_PTR(token0, stack->list->head->data);
    OperatorToken  *testOpToken = stack->list->head->data;
    TEST_ASSERT_EQUAL(TOKEN_OPERATOR_TYPE, testOpToken->type);
    TEST_ASSERT_EQUAL_STRING("(", testOpToken->str);
    TEST_ASSERT_EQUAL(3, testOpToken->startColumn);    
    
    pushToStack(stack, (void  *)token1);
    TEST_ASSERT_EQUAL(2, stack->size);
    TEST_ASSERT_EQUAL_PTR(token1, stack->list->head->data);
    IntegerToken *testIntToken = stack->list->head->data;
    TEST_ASSERT_EQUAL(TOKEN_INTEGER_TYPE, testIntToken->type);
    TEST_ASSERT_EQUAL_STRING("16", testIntToken->str);
    TEST_ASSERT_EQUAL(4, testIntToken->startColumn);  

    pushToStack(stack, (void  *)token2);    
    TEST_ASSERT_EQUAL(3, stack->size);
    TEST_ASSERT_EQUAL_PTR(token2, stack->list->head->data);
    testOpToken = stack->list->head->data;
    TEST_ASSERT_EQUAL(TOKEN_OPERATOR_TYPE, testOpToken->type);
    TEST_ASSERT_EQUAL_STRING("/", testOpToken->str);
    TEST_ASSERT_EQUAL(6, testOpToken->startColumn);    
    
    ListItem  *popItem = popFromStack(stack);
    TEST_ASSERT_EQUAL_PTR(token2, popItem->data);
    linkedListFreeListItem(popItem);
    
    popItem = popFromStack(stack);
    TEST_ASSERT_EQUAL_PTR(token1, popItem->data);
    linkedListFreeListItem(popItem);
    
    popItem = popFromStack(stack);
    TEST_ASSERT_EQUAL_PTR(token0, popItem->data);
    linkedListFreeListItem(popItem);
    
  }Catch(ex){
    dumpTokenErrorMessage(ex, 1);
    TEST_FAIL_MESSAGE("Do not expect any exception to be thrown.");     
  }
  
  Try{
    ListItem  *popItem = popFromStack(stack);
    TEST_FAIL_MESSAGE("EXPECT ERROR_EMPTY_STACK TO BE THROWN, BUT UNRECEIVED");
  }Catch(ex){
    dumpException(ex);
		TEST_ASSERT_EQUAL(ERROR_STACK_IS_EMPTY, ex->errorCode);
		freeException(ex);    
  }
    freeStack(stack, freeToken);
    freeTokenizer(tokenizer);
}




