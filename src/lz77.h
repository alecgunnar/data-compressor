//
// Data Compression Project -- CS 4310
//
// @author Alec Carpenter <alecgunnar@wmich.edu>
// @date   April 3, 2016
//

#include <limits.h>

#ifndef LZ77_H
#define LZ77_H 1

#define VIEW_SIZE         CHAR_MAX
#define PHRASE_SIZE       CHAR_MAX
#define MIN_PHRASE_LENGTH 3
#define MATCH_ALLOC_STEP  16

typedef char byte;

typedef struct {
    byte cursor;
    byte start;
    byte length;
} match;

int compress();
int decompress();

#endif
