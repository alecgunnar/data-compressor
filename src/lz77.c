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

match **matches;

int view_length    = 0;
int code_length    = 0;
int matches_length = 0;
int cursor         = 0;
int offset         = 0;

void compress_view();
int longest_match();
int check_phrase(int length);
void add_match(char cur, char idx, char len);
void add_match_direct(match *m);
void write_compressed_data();
void decompress_code();
void write_from_code(char start, char length);

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
            add_match(code_length - 1, offset, len);
            cursor += len;
        } else {
            code[code_length++] = view[cursor++];
        }
    }

    add_match(0, 0, 0);

    write_compressed_data();

    free(matches);
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

void write_compressed_data () {
    char coded_len = (char) code_length;

    if (write(STDOUT_FILENO, &coded_len, sizeof(char)) < 0)
        error("Could not write code length");

    if (write(STDOUT_FILENO, code, code_length) < 0)
        error("Could not write coded data");

    int i;

    for (i = 0; i < matches_length; i++) {
        if (write(STDOUT_FILENO, matches[i], sizeof(match)) < 0)
            error("Could not write match data");

        free(matches[i]);
    }

    matches_length = 0;
}

int decompress (int input) {
    while (1) {
        if (read(input, &code_length, sizeof(char)) <= 0)
            break;

        if (read(input, code, sizeof(char) * code_length) < 0)
            error("Could not read coded data");

        match *new_match;

        do { 
            new_match = malloc(sizeof(match));

            if (read(input, new_match, sizeof(match)) < 0)
                error("Could not read match data");

            add_match_direct(new_match);
        } while (new_match->length > 0);

        decompress_code();

        free(matches);
    }

    return 1;
}

void decompress_code () {
    int i;

    cursor = 0;

    for (i = 0; i < matches_length; i++) {
        if (!matches[i]->length)
            continue;

        if (cursor < matches[i]->cursor) {
            write_from_code(cursor, matches[i]->cursor - cursor + 1);
            cursor = matches[i]->cursor + 1;
        }

        write_from_code(matches[i]->start, matches[i]->length);

        free(matches[i]);
    }

    if (cursor < code_length)
        write_from_code(cursor, code_length - cursor);
}

void write_from_code (char start, char length) {
    // printf("<%d, %d>", start, length);
    if (write(STDOUT_FILENO, code + start, sizeof(char) * length) < 0)
        error("Could not write from code");
}

void add_match (char cur, char idx, char len) {
    match *new_match  = malloc(sizeof(match));
    new_match->cursor = cur;
    new_match->start  = idx;
    new_match->length = len;

    add_match_direct(new_match);
}

void add_match_direct (match *m) {
    if (matches_length % MATCH_ALLOC_STEP == 0)
        matches = realloc(matches, (matches_length + MATCH_ALLOC_STEP) * sizeof(match));

    matches[matches_length++] = m;
}
