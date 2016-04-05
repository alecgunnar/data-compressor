//
// Data Compression Project -- CS 4310
//
// @author Alec Carpenter <alecgunnar@wmich.edu>
// @date   April 3, 2016
//

#include <stdio.h>
#include <stdlib.h>

#include "error.h"

void error (char *msg) {
    perror(msg);
    exit(1);
}
