#include "../../../libs/model/include/pezzo.h"
#include "../../../external/Unity/src/unity.h"

void setUp(void) {}
void tearDown(void) {}

void test_modify_status(){
    pezzo *a = new_pezzo();
    reset_pezzo(a);
    TEST_ASSERT_EQUAL_STRING("CREATED", status_pezzo_to_string(a->stato));
    TEST_ASSERT_EQUAL_INT(0, a->ts.ingresso);
    modify_status(a, IN_PRESS);
    TEST_ASSERT_EQUAL_STRING("IN_PRESS", status_pezzo_to_string(a->stato));
}

int main (void)
{
    UNITY_BEGIN();

    RUN_TEST(test_modify_status);

    return UNITY_END();
}