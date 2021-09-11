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
    dumpSymbolErrorMessageV2(ex, 1); 
    TEST_ASSERT_EQUAL(ERROR_INVALID_INFIX, ex->errorCode);
    freeException(ex);
  } 
}