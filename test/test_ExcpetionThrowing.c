#include "unity.h"

#include "ExcpetionThrowing.h"
#include  "TokenizerExceptionThrowing.h"
#include "Exception.h"
#include "CException.h"
#include "Tokenizer.h"
#include "Token.h"
#include "ExpressionParser.h"
#include  "DoubleLinkedList.h"
#include  "OperatorTable.h"
#include "Stack.h"
#include "FreeListItemWithSymbol.h"

void setUp(void)
{
}

void tearDown(void)
{
}

CEXCEPTION_T  ex;

void  test_symbolThrowInfixException(){
  Tokenizer *tokenizer = NULL;
  tokenizer = createTokenizer("5* /3");

  Symbolizer  *symbolizer = createSymbolizer(tokenizer);
  
  Symbol  *prevSymbol = getSymbol(symbolizer);
  Symbol  *currentSymbol = getSymbol(symbolizer);
  
  symbolizer->lastSymbol = prevSymbol;
  
   Try{
    symbolThrowInfixException(currentSymbol, ERROR_INVALID_INFIX, symbolizer);
    TEST_FAIL_MESSAGE("EXPECT ERROR_INVALID_INFIX TO BE THROWN, BUT UNRECEIVED");
  }Catch(ex){
    dumpSymbolErrorMessage(ex, 1); 
    TEST_ASSERT_EQUAL(ERROR_INVALID_INFIX, ex->errorCode);
    freeException(ex);
  }
}

void  test_symbolThrowPrefixException(){
  Tokenizer *tokenizer = NULL;
  tokenizer = createTokenizer("5++ --3");

  Symbolizer  *symbolizer = createSymbolizer(tokenizer);
  
  Symbol  *symbol1 = getSymbol(symbolizer);
  Symbol  *symbol2 = getSymbol(symbolizer);
  Symbol  *symbol3 = getSymbol(symbolizer);
  
  symbolizer->lastSymbol = symbol2;
  
   Try{
    symbolThrowPrefixException(symbol3, ERROR_INVALID_PREFIX, symbolizer);
    TEST_FAIL_MESSAGE("EXPECT ERROR_INVALID_PREFIX TO BE THROWN, BUT UNRECEIVED");
  }Catch(ex){
    dumpSymbolErrorMessage(ex, 1); 
    TEST_ASSERT_EQUAL(ERROR_INVALID_PREFIX, ex->errorCode);
    freeException(ex);
  } 
  freeSymbol(symbol1);
}


void  test_symbolThrowSuffixException(){
  Tokenizer *tokenizer = NULL;
  tokenizer = createTokenizer("3++ --");

  Symbolizer  *symbolizer = createSymbolizer(tokenizer);
  
  Symbol  *symbol1 = getSymbol(symbolizer);
  Symbol  *symbol2 = getSymbol(symbolizer);
  Symbol  *symbol3 = getSymbol(symbolizer);
  
  symbolizer->lastSymbol = symbol2;
  
   Try{
    symbolThrowSuffixException(symbol3, ERROR_INVALID_SUFFIX, symbolizer);
    TEST_FAIL_MESSAGE("EXPECT ERROR_INVALID_SUFFIX TO BE THROWN, BUT UNRECEIVED");
  }Catch(ex){
    dumpSymbolErrorMessage(ex, 1); 
    TEST_ASSERT_EQUAL(ERROR_INVALID_SUFFIX, ex->errorCode);
    freeException(ex);
  } 
  freeSymbol(symbol1);
}

void  test_symbolThrowNumberException(){
  Tokenizer *tokenizer = NULL;
  tokenizer = createTokenizer("3--3");

  Symbolizer  *symbolizer = createSymbolizer(tokenizer);
  
  Symbol  *symbol1 = getSymbol(symbolizer);
  Symbol  *symbol2 = getSymbol(symbolizer);
  Symbol  *symbol3 = getSymbol(symbolizer);
  
  symbolizer->lastSymbol = symbol2;
  
   Try{
    symbolThrowNumberException(symbol3, ERROR_INVALID_NUMBER, symbolizer);
    TEST_FAIL_MESSAGE("EXPECT ERROR_INVALID_NUMBER TO BE THROWN, BUT UNRECEIVED");
  }Catch(ex){
    dumpSymbolErrorMessage(ex, 1); 
    TEST_ASSERT_EQUAL(ERROR_INVALID_NUMBER, ex->errorCode);
    freeException(ex);
  } 
  freeSymbol(symbol1);
}
