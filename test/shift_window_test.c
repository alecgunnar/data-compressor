#define WINDOW_SIZE 32

#include "test.h"
#include "../src/lzss.h"

extern byte *window;
extern byte *buffer;

extern int window_length;
extern int buffer_length;

extern void shift_window(int len);

void test_shift_underfilled() {
    PRINT_TEST_NAME("shift into underfilled window");

    char *window_data = "ABCDEFGHIJK";
    char *buffer_data = "LMNOPQRSTUV";

    memcpy(window, window_data, 11);
    memcpy(buffer, buffer_data, 11);

    window_length = 11;
    buffer_length = 11;

    shift_window(buffer_length);

    ASSERT_STR_EQUAL(window, "ABCDEFGHIJKLMNOPQRSTUV", window_length);
    ASSERT_INT_EQUAL(window_length, 22);
    ASSERT_INT_EQUAL(buffer_length, 0);
}

void test_shift_overfilled () {
    PRINT_TEST_NAME("shift into overfilled window");

    char *window_data = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char *buffer_data = "1234567890";

    memcpy(window, window_data, 26);
    memcpy(buffer, buffer_data, 10);

    window_length = 26;
    buffer_length = 10;

    shift_window(10);

    ASSERT_STR_EQUAL(window, "EFGHIJKLMNOPQRSTUVWXYZ1234567890", window_length);
    ASSERT_STR_EQUAL(buffer, "", buffer_length);
    ASSERT_INT_EQUAL(window_length, WINDOW_SIZE);
    ASSERT_INT_EQUAL(buffer_length, 0);
}

void test_shift_partial () {
    PRINT_TEST_NAME("shift into underfilled window");

    char *window_data = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char *buffer_data = "1234567890";

    memcpy(window, window_data, 26);
    memcpy(buffer, buffer_data, 10);

    window_length = 26;
    buffer_length = 10;

    shift_window(5);

    ASSERT_STR_EQUAL(window, "ABCDEFGHIJKLMNOPQRSTUVWXYZ12345", window_length);
    ASSERT_STR_EQUAL(buffer, "67890", buffer_length);
    ASSERT_INT_EQUAL(window_length, 31);
    ASSERT_INT_EQUAL(buffer_length, 5);
}

void test_shift_empty () {
    PRINT_TEST_NAME("shift into empty window");

    char *window_data = "";
    char *buffer_data = "1234567890";

    memcpy(window, window_data, 0);
    memcpy(buffer, buffer_data, 10);

    window_length = 0;
    buffer_length = 10;

    shift_window(5);

    ASSERT_STR_EQUAL(window, "12345", window_length);
    ASSERT_STR_EQUAL(buffer, "67890", buffer_length);
    ASSERT_INT_EQUAL(window_length, 5);
    ASSERT_INT_EQUAL(buffer_length, 5);
}

void test_shift_complex () {
    PRINT_TEST_NAME("complex shift");

    char *window_data = "";
    char *buffer_data = "1234567890";

    memcpy(window, window_data, 0);
    memcpy(buffer, buffer_data, 10);

    window_length = 0;
    buffer_length = 10;

    shift_window(2);
    ASSERT_STR_EQUAL(window, "12", window_length);
    ASSERT_STR_EQUAL(buffer, "34567890", buffer_length);
    ASSERT_INT_EQUAL(window_length, 2);
    ASSERT_INT_EQUAL(buffer_length, 8);

    shift_window(3);
    ASSERT_STR_EQUAL(window, "12345", window_length);
    ASSERT_STR_EQUAL(buffer, "67890", buffer_length);
    ASSERT_INT_EQUAL(window_length, 5);
    ASSERT_INT_EQUAL(buffer_length, 5);
}

int main () {
    configured_start(WINDOW_SIZE, WINDOW_SIZE);

    test_shift_underfilled();
    test_shift_overfilled();
    test_shift_partial();
    test_shift_empty();
    test_shift_complex();

    shutdown();
}
