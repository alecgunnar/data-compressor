//
// Data Compression Project -- CS 4310
//
// @author Alec Carpenter <alecgunnar@wmich.edu>
// @date   April 3, 2016
//

#ifndef LZ77_H
#define LZ77_H 1

#define VIEW_SIZE        512
#define MIN_MATCH_LENGTH 3

int compress(int input);
int decompress(int input);

#endif
