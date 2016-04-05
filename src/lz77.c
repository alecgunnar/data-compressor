//
// Data Compression Project -- CS 4310
//
// @author Alec Carpenter <alecgunnar@wmich.edu>
// @date   April 3, 2016
//

#include <stdio.h>

#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "lz77.h"
#include "error.h"

byte window[WINDOW_SIZE];
byte buffer[BUFFER_SIZE];
byte phrase[WINDOW_SIZE];

match *matches;

int window_length = 0;
int phrase_length = 0;

void push_byte(byte push);
int check_phrase();
void shift_window();

int compress (int input) {
    int i, read_len = 0;

    while ((read_len = read(input, &buffer, BUFFER_SIZE)) > 0)
        for (i = 0; i < read_len; i++)
            push_byte(buffer[i]);

    if (read_len < 0)
        error("Could not read in data file.");

    return 1;
}

void push_byte (byte push) {
    phrase[phrase_length++] = push;
    phrase[phrase_length]   = 0;

    if (!check_phrase()) {
        shift_window();
        phrase_length = 0;
    }
}

int check_phrase () {
    static int running_match = 0;
    int offset = 0, index;

    printf("[%s] => [%s]\n", phrase, window);

    while ((offset + phrase_length - 1) < window_length) {
        for (index = 0; index < phrase_length; index++)
            if (phrase[index] != window[offset + index])
                break;

        if (index == phrase_length) {
            running_match = 1;
            return 1;
        }

        offset++;
    }

    if (running_match && phrase_length > MIN_MATCH_LENGTH) {
        phrase[0]     = phrase[phrase_length - 1];
        phrase_length = 1;
        running_match = 0;

        puts("MATCH!");

        return 1;
    }

    return 0;
}

void shift_window () {
    int i;

    if ((window_length + phrase_length) > WINDOW_SIZE) {
        if (write(STDOUT_FILENO, window, phrase_length) < 0)
            error("Cannot write");

        for (i = 0; i < window_length; i++)
            window[i] = window[i + phrase_length];

        window_length -= phrase_length;
    }

    memcpy(window + window_length, phrase, phrase_length);

    window_length += phrase_length;
    phrase_length  = 0;
}

int decompress (int input) {
    return 0;
}
