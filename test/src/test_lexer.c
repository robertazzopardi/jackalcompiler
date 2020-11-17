#include "unity/unity.h"
#include "../../src/lexer.h"

void setUp(void)
{
    // set stuff up here
}

void tearDown(void)
{
    // clean stuff up here
}

void test_isOperator_method(void)
{
    // pass cases
    TEST_ASSERT(isOperator('+') == 1);
    TEST_ASSERT(isOperator('-') == 1);
    TEST_ASSERT(isOperator('/') == 1);
    TEST_ASSERT(isOperator('*') == 1);
    TEST_ASSERT(isOperator('%') == 1);
    TEST_ASSERT(isOperator('^') == 1);

    // fail cases
    TEST_ASSERT(isOperator('a') == 0);
    TEST_ASSERT(isOperator('A') == 0);
    TEST_ASSERT(isOperator('0') == 0);
    TEST_ASSERT(isOperator('}') == 0);
    TEST_ASSERT(isOperator(')') == 0);
    TEST_ASSERT(isOperator('"') == 0);
}

// not needed when using generate_test_runner.rb
int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_isOperator_method);

    return UNITY_END();
}
