#include "test.h"
#include "../src/boyer_moore.h"

extern int offset_of_next(char search, char *string, int len);

void test_find_next_when_it_does_not_exists () {
    PRINT_TEST_NAME("find next when it does not exists");

    char *str   = "ABCD4";
    char search = 'Q';
    int length  = 5;

    ASSERT_INT_EQUAL(offset_of_next(search, str, length), -1);
}

void test_find_next_when_it_does_exist () {
    PRINT_TEST_NAME("find next when it does exist");

    char *str  = "ABC";
    int length = 3;

    ASSERT_INT_EQUAL(offset_of_next('A', str, length), 0);
    ASSERT_INT_EQUAL(offset_of_next('B', str, length), 1);
}

void test_when_text_is_empty () {
    PRINT_TEST_NAME("when text is empty");

    char *text = "";
    char *str  = "ABC";

    ASSERT_INT_EQUAL(get_offset(text, 0, str, 3), -1);
}

void test_when_text_is_short () {
    PRINT_TEST_NAME("when text is short");

    char *text = "AB";
    char *str  = "ABC";

    ASSERT_INT_EQUAL(get_offset(text, 2, str, 3), -1);
}

void test_when_string_in_text () {
    PRINT_TEST_NAME("when string in text");

    char *text = "012ABC345DEF678";
    char *str  = "ABC";

    ASSERT_INT_EQUAL(get_offset(text, 12, str, 3), 3);
}

int main () {
    test_find_next_when_it_does_not_exists();
    test_find_next_when_it_does_exist();
    test_when_text_is_empty();
    test_when_text_is_short();
    test_when_string_in_text();
}
