//
// Data Compression Project -- CS 4310
//
// @author Alec Carpenter <alecgunnar@gmail.com>
// @date   April 8, 2016
//

#include "boyer_moore.h"

int offset_of_next(char search, char *string, int length);

int get_offset (char *text, int text_length, char *string, int string_length) {
    if (text_length < string_length)
        return -1;

    int offset = 0, index, next;

    while ((offset + string_length) < text_length) {
        for (index = string_length - 1; index >= 0; index--)
            if (text[offset + index] != string[index])
                break;

        if (index == -1)
            return offset;

        if ((next = offset_of_next(text[offset + index], string, index + 1)) > -1) {
            offset += string_length - offset_of_next(text[offset + index], string, index + 1); // Add one to index because it counts from zero, not one.
        } else {
            offset += string_length;
        }
    }

    return -1;
}

int offset_of_next (char search, char *string, int length) {
    int offset;

    for (offset = length - 1; offset >= 0; offset--)
        if (string[offset] == search)
            break;

    return offset;
}
