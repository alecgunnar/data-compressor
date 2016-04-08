#include <stdio.h>
#include <string.h>

#ifndef TEST_H
#define TEST_H 1

#define PRINT_TEST_NAME(name) \
    fprintf(stderr, "\nRunning test: %s\n\n", name);

void ASSERT_STR_EQUAL(char *given, char *expected, int len) {
    char *op;

    if (strncmp(given, expected, len) != 0) {
        fprintf(stderr, "\tFAILED!\n\t\"");
        op = "!=";
    } else {
        fprintf(stderr, "\tPASSED!\n\t\"");
        op = "==";
    }

    fwrite(given, sizeof(char), len, stderr);
    fprintf(stderr, "\" %s \"", op);
    fwrite(expected, sizeof(char), len, stderr);
    fprintf(stderr, "\"\n\n");
}

void ASSERT_INT_EQUAL(int given, int expected) {
    if (given != expected) {
        fprintf(stderr, "\tFAILED!\n\t%d != %d\n\n", given, expected);
    } else {
        fprintf(stderr, "\tPASSED!\n\t%d == %d\n\n", given, expected);
    }
}

#endif
