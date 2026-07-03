#include "buffer.h"
#include "unity.h"

void setUp(void) {}
void tearDown(void) {}

void test_buffer_init(){
    buffer *buf1 = initialize(5);
    buffer *buf2 = initialize(6);
    TEST_ASSERT_NOT_NULL(buf1);
    TEST_ASSERT_NOT_NULL(buf2);
    TEST_ASSERT_EQUAL_INT(5, buf1->array_size);
    TEST_ASSERT_EQUAL_INT(6, buf2->array_size);
    terminate(buf1);
    terminate(buf2);
}

void test_buffer_new_item(){
    buffer *buf = initialize(3);
    pezzo *p1 = malloc(sizeof(pezzo));
    pezzo *p2 = malloc(sizeof(pezzo));
    pezzo *p3 = malloc(sizeof(pezzo));
    pezzo *p4 = malloc(sizeof(pezzo));

    new_item(p1, buf);
    new_item(p2, buf);
    new_item(p3, buf);
    TEST_ASSERT_TRUE(is_full(buf));

    new_item(p4, buf); // should not add p4 since buffer is full
    TEST_ASSERT_EQUAL_PTR(p1, buf->array[0]);
    TEST_ASSERT_EQUAL_PTR(p2, buf->array[1]);
    TEST_ASSERT_EQUAL_PTR(p3, buf->array[2]);

    terminate(buf);
    free(p1);
    free(p2);
    free(p3);
    free(p4);
}

void test_buffer_take_item(){
    buffer *buf = initialize(3);
    pezzo *p1 = malloc(sizeof(pezzo));
    pezzo *p2 = malloc(sizeof(pezzo));
    pezzo *p3 = malloc(sizeof(pezzo));

    new_item(p1, buf);
    new_item(p2, buf);
    new_item(p3, buf);

    pezzo *taken1 = take_item(buf);
    TEST_ASSERT_EQUAL_PTR(p1, taken1);
    TEST_ASSERT_EQUAL_PTR(p2, buf->array[0]);
    TEST_ASSERT_EQUAL_PTR(p3, buf->array[1]);
    TEST_ASSERT_NULL(buf->array[2]);

    pezzo *taken2 = take_item(buf);
    TEST_ASSERT_EQUAL_PTR(p2, taken2);
    TEST_ASSERT_EQUAL_PTR(p3, buf->array[0]);
    TEST_ASSERT_NULL(buf->array[1]);
    TEST_ASSERT_NULL(buf->array[2]);

    pezzo *taken3 = take_item(buf);
    TEST_ASSERT_EQUAL_PTR(p3, taken3);
    TEST_ASSERT_TRUE(is_empty(buf));

    terminate(buf);
    free(p1);
    free(p2);   
    free(p3);
}

int main(void){
    UNITY_BEGIN();

    RUN_TEST(test_buffer_init);
    RUN_TEST(test_buffer_new_item);
    RUN_TEST(test_buffer_take_item);

    return UNITY_END();
}