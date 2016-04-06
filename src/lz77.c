//
// Data Compression Project -- CS 4310
//
// @author Alec Carpenter <alecgunnar@wmich.edu>
// @date   April 3, 2016
//

#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "lz77.h"
#include "error.h"

char view[VIEW_SIZE];
char code[VIEW_SIZE];
char phrase[VIEW_SIZE];

int view_length = 0;
int code_length = 0;
int cursor  = 0;

int offset = 0;

void compress_view();
int longest_match();
int check_phrase(int length);

int compress (int input) {
    while ((view_length = read(input, &view, VIEW_SIZE)) > 0)
        compress_view();

    if (view_length < 0)
        error("Could not read in data file.");

    return 1;
}

void compress_view () {
    int len;

    while (cursor < view_length) {
        if ((len = longest_match()) > MIN_MATCH_LENGTH) {
            // DEBUG
            // puts(code);
            // int i;
            // for (i = 0; i < offset; i++)
            //     printf(" ");
            // fflush(stdout);
            // write(STDOUT_FILENO, view + cursor, len);
            // printf("<%d, %d, %d>\n", code_length - 1, offset, len);
            // DEBUG

            cursor += len;
        } else {
            code[code_length++] = view[cursor++];
        }
    }
}

int longest_match () {
    int length = 0;

    while (check_phrase(++length) > 0);

    return length;
}

int check_phrase (int length) {
    static int check_offset = 0;
    int index;

    if (length == 1)
        check_offset = 0;

    while ((check_offset + length) < code_length) {
        for (index = 0; index < length; index++)
            if (code[check_offset + index] != view[cursor + index])
                break;

        if (index == length) {
            if (code[check_offset + index] != view[cursor + index])
                return 0;

            return (offset = check_offset);
        }

        check_offset++;
    }

    return 0;
}

int decompress (int input) {
    return 0;
}
