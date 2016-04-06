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

byte window[WINDOW_SIZE];
byte buffer[BUFFER_SIZE];
byte phrase[WINDOW_SIZE];

match **matches;

int window_length     = 0;
int phrase_length     = 0;
int matches_length    = 0;
long int coded_length = 0;
long int cursor       = 0;
off_t footer_offset   = 0;

void push_byte(byte push);
int check_phrase();
void add_match(byte index, byte length);
void shift_window();
void write_compressed_data();
off_t read_coded_length(int input);
void read_matches(int input, off_t offset);
void write_from_coded(int input, byte index, byte length);

int compress (int input) {
    int i, read_len = 0;

    while ((read_len = read(input, &buffer, BUFFER_SIZE)) > 0)
        for (i = 0; i < read_len; i++)
            push_byte(buffer[i]);

    write_compressed_data();

    if (read_len < 0)
        error("Could not read in data file.");

    return 1;
}

void push_byte (byte push) {
    phrase[phrase_length++] = push;

    if (!check_phrase())
        shift_window();
}

int check_phrase () {
    static int running_offset = 0;
    int offset = 0, index;

    while ((offset + phrase_length - 1) < window_length) {
        for (index = 0; index < phrase_length; index++)
            if (phrase[index] != window[offset + index])
                break;

        if (index == phrase_length)
            return (running_offset = offset);

        offset++;
    }

    if (running_offset && phrase_length >= MIN_MATCH_LENGTH) {
        add_match(running_offset, phrase_length - 1);

        // DEBUG
        // window[window_length] = 0;
        // puts(window);
        // int i;
        // for(i = 0; i < running_offset; i++)
        //     printf("-");
        // phrase[phrase_length] = 0;
        // puts(phrase);
        // DEBUG

        phrase[0]      = phrase[phrase_length - 1];
        phrase_length  = 1;
        running_offset = 0;

        return 1;
    }

    return 0;
}

void shift_window () {
    if ((window_length + phrase_length) > WINDOW_SIZE) {
        int i,
            less = phrase_length - (WINDOW_SIZE - window_length);

        if (write(STDOUT_FILENO, &window, less) < 0)
            error("Could not write from window");

        for (i = 0; i < window_length; i++)
            window[i] = window[i + less];

        window_length -= less;
    }

    memcpy(window + window_length, phrase, phrase_length);

    window_length += phrase_length;
    coded_length  += phrase_length;
    phrase_length  = 0;
}

void write_compressed_data () {
    shift_window();

    if (write(STDOUT_FILENO, window, window_length) < 0)
        error("Cannot write window");

    int i;

    for (i = 0; i < matches_length; i++) {
        if (write(STDOUT_FILENO, matches[i], sizeof(match)) < 0)
            error("Cannot write match");

        free(matches[i]);
    }

    free(matches);

    if (write(STDOUT_FILENO, &coded_length, sizeof(long int)) < 0)
        error("Cannot write coded length");
}

int decompress (int input) {
    read_matches(input, read_coded_length(input));

    int i;

    for (i = 0; i < matches_length; i++) {
        if (cursor < matches[i]->index) {
            write_from_coded(input, cursor, (matches[i]->index + matches[i]->length) - cursor);
            cursor = matches[i]->index + matches[i]->length;
        }

        write_from_coded(input, matches[i]->index, matches[i]->length);
    }

    if (cursor < coded_length)
        write_from_coded(input, cursor, (coded_length - cursor));

    return 1;
}

off_t read_coded_length (int input) {
    if ((footer_offset = lseek(input, -1 * sizeof(long int), SEEK_END)) < 0)
        error("Could not seek to coded length");

    if (read(input, &coded_length, sizeof(long int)) < 0)
        error("Could not read coded length");

    off_t offset = 0;

    if ((offset = lseek(input, coded_length * sizeof(byte), SEEK_SET)) < 0)
        error("Could not seek to matches");

    return offset;
}

void read_matches (int input, off_t offset) {
    byte index, length;

    while (offset < footer_offset) {
        if (read(input, &index, sizeof(byte)) < 0)
            error("Could not read in index value");

        if (read(input, &length, sizeof(byte)) < 0)
            error("Could not read in length value");

        add_match(index, length);

        offset += 2;
    }
}

void write_from_coded (int input, byte index, byte length) {
    printf("[%d, %d] -> '", index, length);
    fflush(stdout);

    if (lseek(input, index, SEEK_SET) < 0)
        error("Could not seek through coded part");

    if (read(input, &buffer, length) < 0)
        error("Could not read data from coded part");

    if (write(STDOUT_FILENO, &buffer, length) < 0)
        error("Could not write data from coded part.");

    puts("'");
}

void add_match (byte index, byte length) {
    if (matches_length % MATCH_ALLOC_STEP == 0)
        matches = realloc(matches, (matches_length + MATCH_ALLOC_STEP) * sizeof(match));

    match *new_match  = malloc(sizeof(match));
    new_match->index   = index;
    new_match->length = length;

    matches[matches_length++] = new_match;
}
