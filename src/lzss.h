//
// Data Compression Project -- CS 4310
//
// @author Alec Carpenter <alecgunnar@gmail.com>
// @date   April 3, 2016
//

#include <limits.h>

#ifndef lzss_H
#define lzss_H 1

#ifndef WINDOW_SIZE
    #define WINDOW_SIZE CHAR_MAX
#endif

#define BUFFER_SIZE           CHAR_MAX
#define MINIMUM_PREFIX_LENGTH 2
#define MATCH_ALLOC_STEP      16

typedef char byte;

void configured_start(int win_size, int buf_size);
void standard_start();
void shutdown();

int compress();
int decompress();

#endif
