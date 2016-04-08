#include "test.h"
#include "../src/lzss.h"

extern byte *window;
extern byte *buffer;

extern int window_length;
extern int buffer_length;
extern int last_offset;

extern int longest_prefix();

void test_longest_prefix_in_empty_window () {
    PRINT_TEST_NAME("longest prefix in empty window");

    char *buffer_data = "ABC";
    memcpy(buffer, buffer_data, 3);

    window_length = 0;
    buffer_length = 3;

    int longest = longest_prefix();

    ASSERT_INT_EQUAL(longest, -1);
}

void test_longest_prefix_found_amongst_many_smaller () {
    PRINT_TEST_NAME("longest prefix found amongst many smaller");

    char *window_data = "0123ABCD4567";
    char *buffer_data = "ABCDEF";

    memcpy(window, window_data, 12);
    memcpy(buffer, buffer_data, 6);

    window_length = 12;
    buffer_length = 6;

    int longest = longest_prefix();

    ASSERT_INT_EQUAL(longest, 4);
    ASSERT_INT_EQUAL(last_offset, 4);
}

int main () {
    standard_start();

    test_longest_prefix_in_empty_window();
    test_longest_prefix_found_amongst_many_smaller();

    shutdown();
}
