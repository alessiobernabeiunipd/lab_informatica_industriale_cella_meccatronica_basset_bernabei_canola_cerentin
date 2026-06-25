#include "pezzo.h"
#include "unity.h"

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

void test_copy_pezzo(){
    pezzo *a = new_pezzo();
    reset_pezzo(a);
    a->id_pezzo = 3;
    a->stato = IN_GOM;
    a->ts.ingresso = 100;
    pezzo *copy = copy_pezzo(a);
    TEST_ASSERT_EQUAL_STRING("IN_GOM", status_pezzo_to_string(copy->stato));
    TEST_ASSERT_EQUAL_INT(3, copy->id_pezzo);
    TEST_ASSERT_EQUAL_INT(100, copy->ts.ingresso);
    TEST_ASSERT_EQUAL_INT(NULL, copy->next);
}

void test_list(){
    pezzo *list_head = NULL;
    pezzo *a = new_pezzo();
    reset_pezzo(a);
    pezzo *b = new_pezzo();
    reset_pezzo(b);
    pezzo *c = new_pezzo();
    reset_pezzo(c);
    modify_status(a, IN_LAMINATION);
    modify_status(b, IN_LAMINATION);
    modify_status(c, WAITING_INPUT);
    add_pezzo(&list_head, a);
    add_pezzo(&list_head, b);
    add_pezzo(&list_head, c);
    TEST_ASSERT_EQUAL_INT(b,a->next);
    TEST_ASSERT_EQUAL_INT(c,b->next);
    TEST_ASSERT_EQUAL_INT(a,list_head);
}

void test_first(){
    pezzo *list_head = NULL;
    pezzo *a = new_pezzo();
    reset_pezzo(a);
    pezzo *b = new_pezzo();
    reset_pezzo(b);
    pezzo *c = new_pezzo();
    reset_pezzo(c);
    modify_status(a, IN_LAMINATION);
    modify_status(b, IN_LAMINATION);
    modify_status(c, WAITING_INPUT);
    add_pezzo(&list_head, a);
    add_pezzo(&list_head, b);
    add_pezzo(&list_head, c);

    pezzo *p = first_pezzo_with_status(list_head, WAITING_INPUT);
    pezzo *k = first_pezzo_with_status(list_head, SCRAP);
    TEST_ASSERT_EQUAL_INT(c, p);
    TEST_ASSERT_EQUAL_INT(NULL, k);
}

void test_add(){
    pezzo *list_head = NULL;
    pezzo *a = new_pezzo();
    reset_pezzo(a);
    pezzo *b = new_pezzo();
    reset_pezzo(b);
    pezzo *c = new_pezzo();
    reset_pezzo(c);
    modify_status(a, IN_LAMINATION);
    modify_status(b, IN_LAMINATION);
    modify_status(c, TRAVELING);
    add_pezzo(&list_head, a);
    add_pezzo(&list_head, b);
    add_pezzo(&list_head, c);

    pezzo *d = new_pezzo();
    reset_pezzo(d);
    modify_status(d, SCRAP);
    d->id_pezzo=1;
    add_pezzo_next_in_production(list_head, d);
    TEST_ASSERT_EQUAL_INT(1, c->next->id_pezzo);
    TEST_ASSERT_EQUAL_STRING("CREATED", status_pezzo_to_string(c->next->stato));

    pezzo *e = new_pezzo();
    reset_pezzo(e);
    e->id_pezzo=4;
    modify_status(e, SCRAP);
    add_pezzo_next_in_production(list_head, e);
    TEST_ASSERT_EQUAL_INT(4, c->next->id_pezzo);
    TEST_ASSERT_EQUAL_STRING("CREATED", status_pezzo_to_string(c->next->stato));
}

int main (void)
{
    UNITY_BEGIN();

    RUN_TEST(test_modify_status);
    RUN_TEST(test_copy_pezzo);
    RUN_TEST(test_list);
    RUN_TEST(test_first);
    RUN_TEST(test_add);

    return UNITY_END();
}