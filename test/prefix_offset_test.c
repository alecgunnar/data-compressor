#include "test.h"
#include "../src/lzss.h"

extern byte *window;
extern byte *buffer;

extern int window_length;
extern int buffer_length;

extern int prefix_offset(int len);

void test_prefix_offset_in_empty_window () {
    PRINT_TEST_NAME("prefix offset in empty window");

    char *buffer_data = "ABC";
    memcpy(buffer, buffer_data, 3);

    ASSERT_INT_EQUAL(prefix_offset(3), -1);
}

void test_prefix_in_window () {
    PRINT_TEST_NAME("prefix in window");

    char *window_data = "123ABC456DEF";
    char *buffer_data = "ABCD";

    memcpy(window, window_data, 12);
    memcpy(buffer, buffer_data, 4);

    window_length = 12;
    buffer_length = 4;

    ASSERT_INT_EQUAL(prefix_offset(3), 3);
}

void test_prefix_not_in_window () {
    PRINT_TEST_NAME("prefix not in window");

    char *window_data = "123ABC456DEF";
    char *buffer_data = "789";

    memcpy(window, window_data, 12);
    memcpy(buffer, buffer_data, 3);

    window_length = 12;
    buffer_length = 3;

    ASSERT_INT_EQUAL(prefix_offset(3), -1);
}

int main () {
    standard_start();

    test_prefix_offset_in_empty_window();
    test_prefix_in_window();
    test_prefix_not_in_window();

    shutdown();
}
