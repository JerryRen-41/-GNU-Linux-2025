#include <check.h>
#include <stdlib.h>
#include <string.h>
#include "../src/mbuf.h"

START_TEST(test_init)
{
    mbuf b;
    mbuf_init(&b);
    ck_assert_ptr_null(b.data);
    ck_assert_int_eq(b.len, 0);
    ck_assert_int_eq(b.cap, 0);
}
END_TEST

START_TEST(test_append)
{
    mbuf b;
    mbuf_init(&b);

    const char *s = "hello";
    ck_assert_int_eq(mbuf_append(&b, s, 5), 0);

    ck_assert_int_eq(b.len, 5);
    ck_assert_mem_eq(b.data, "hello", 5);

    mbuf_free(&b);
}
END_TEST

START_TEST(test_reserve)
{
    mbuf b;
    mbuf_init(&b);

    ck_assert_int_eq(mbuf_reserve(&b, 100), 0);
    ck_assert_int_ge(b.cap, 100);

    mbuf_free(&b);
}
END_TEST

Suite *mbuf_suite(void)
{
    Suite *s = suite_create("mbuf");
    TCase *tc = tcase_create("core");

    tcase_add_test(tc, test_init);
    tcase_add_test(tc, test_append);
    tcase_add_test(tc, test_reserve);

    suite_add_tcase(s, tc);
    return s;
}

int main(void)
{
    int failed;
    Suite *s = mbuf_suite();
    SRunner *sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return failed == 0 ? 0 : 1;
}

