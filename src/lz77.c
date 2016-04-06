//
// Data Compression Project -- CS 4310
//
// @author Alec Carpenter <alecgunnar@wmich.edu>
// @date   April 3, 2016
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lz77.h"
#include "error.h"

// External variables
extern FILE *input_file;

// Global variables
byte view[VIEW_SIZE];
byte code[VIEW_SIZE];
byte phrase[PHRASE_SIZE];

int view_length = 0;
int code_length = 0;
int cursor      = 0;

byte phrase_offset = 0;

// Prototypes
void process_view();
int longest_phrase();
int index_of();
long int input_size();

int compress () {
    while ((view_length = fread(view, sizeof(byte), VIEW_SIZE, input_file)) > 0)
        process_view();

    return 1;
}

void process_view () {
    int length;

    while (cursor < view_length) {
        length = longest_phrase();

        if (length >= MIN_PHRASE_LENGTH) {
            putchar(-1 * phrase_offset);
            putchar(length);
            cursor += length;
        } else {
            putchar(code[code_length++] = view[cursor++]);
        }

        fflush(stdout);
    }

    cursor      = 0;
    code_length = 0;
}

int longest_phrase () {
    int offset, length = 0;

    while (length < PHRASE_SIZE && (length < (view_length - cursor))) {
        phrase[length]   = view[cursor + length];
        phrase[++length] = 0;

        if ((offset = index_of()) < 0)
            break;

        phrase_offset = offset;
    }

    return --length;
}

int index_of () {
    char *loc = strstr(code, phrase);

    if (loc == NULL)
        return -1;

    return loc - code;
}

int decompress () {
    long int size = input_size();
    char c, n;

    while(ftell(stdin) < size) {
        c = getc(input_file);

        if (c < 0) {
            n = getc(input_file);

            memcpy(view + view_length, code + (-1 * c), n);
            view_length += n;
        } else {
            view[view_length++] = c;
            code[code_length++] = c;
        }

        if (view_length == VIEW_SIZE || ftell(stdin) == size) {
            printf("%s", view);
            fflush(stdout);

            memset(view, 0, VIEW_SIZE);
            view_length = code_length = 0;
        }
    }

    return 1;
}

long int input_size () {
    fseek(input_file, 0, SEEK_END);
    long int size = ftell(input_file);
    fseek(input_file, 0, SEEK_SET);
    return size;
}
