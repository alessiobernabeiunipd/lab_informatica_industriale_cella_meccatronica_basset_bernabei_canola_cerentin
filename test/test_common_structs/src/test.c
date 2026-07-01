#include "utils.h"
#include "unity.h"

void setUp(void) {}
void tearDown(void) {}

void test_status_pezzo_to_string(void)
{
    TEST_ASSERT_EQUAL_STRING("CREATED", status_pezzo_to_string(CREATED));
    TEST_ASSERT_EQUAL_STRING("IN_PRESS", status_pezzo_to_string(IN_PRESS));
    TEST_ASSERT_EQUAL_STRING("OK", status_pezzo_to_string(OK));
}

void test_status_station_to_string(void)
{
    TEST_ASSERT_EQUAL_STRING("IDLE", status_station_to_string(IDLE));
    TEST_ASSERT_EQUAL_STRING("COOLING", status_station_to_string(COOLING));
    TEST_ASSERT_EQUAL_STRING("BUSY", status_station_to_string(BUSY));
}

int main (void)
{
    UNITY_BEGIN();

    RUN_TEST(test_status_pezzo_to_string);
    RUN_TEST(test_status_station_to_string);

    return UNITY_END();
}
