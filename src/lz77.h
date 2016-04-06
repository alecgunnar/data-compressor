//
// Data Compression Project -- CS 4310
//
// @author Alec Carpenter <alecgunnar@wmich.edu>
// @date   April 3, 2016
//

#ifndef LZ77_H
#define LZ77_H 1

#define VIEW_SIZE        256
#define MIN_MATCH_LENGTH 3
#define MATCH_ALLOC_STEP 16

typedef struct {
    unsigned char cursor;
    unsigned char start;
    unsigned char length;
} match;

int compress(int input);
int decompress(int input);

#endif
