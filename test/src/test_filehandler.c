#include "unity/unity.h"
#include "filehandler.h"

void setUp(void)
{
    // set stuff up here
}

void tearDown(void)
{
    // clean stuff up here
}

void test_function_should_doBlahAndBlah(void)
{
    //test stuff
}

// not needed when using generate_test_runner.rb
int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_function_should_doBlahAndBlah);

    return UNITY_END();
}
