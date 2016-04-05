//
// Data Compression Project -- CS 4310
//
// @author Alec Carpenter <alecgunnar@wmich.edu>
// @date   April 3, 2016
//

#ifndef LZ77_H
#define LZ77_H 1

#define WINDOW_SIZE      512
#define BUFFER_SIZE      512
#define MIN_MATCH_LENGTH 3

typedef char byte;

typedef struct {
    byte jump;
    byte length;
} match;

int compress(int input);
int decompress(int input);

#endif
