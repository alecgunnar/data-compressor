//
// Data Compression Project -- CS 4310
//
// @author Alec Carpenter <alecgunnar@gmail.com>
// @date   April 3, 2016
//

#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>

#include "lzss.h"
#include "error.h"

const char *EXEC_NAME;

FILE *input_file;

int do_compressor();
int do_decompressor();
int help();
int run_cmd(char *cmd);

int main (int argc, char **argv) {
    EXEC_NAME = argv[0];

    if (argc < 2)
        exit(help());

    input_file = stdin;

    if (argc == 3 && (input_file = fopen(argv[2], "r")) == NULL)
        error("Could not open input file");

    int status = run_cmd(argv[1]) == 0;

    if (input_file != stdin)
        fclose(input_file);

    exit(status);
}

int run_cmd (char *cmd) {
    switch (cmd[0]) {
        case 'c':
            return compress();
        case 'd':
            return decompress();
    }

    return help();
}

int help () {
    puts("Data Compression Project -- CS 4310");
    puts("Created by: Alec Carpenter\n");

    puts("Usage:");
    printf("\t%s help\n", EXEC_NAME);
    printf("\t%s command [input file|< input file] [> output file]\n\n", EXEC_NAME);

    puts("Commands:");
    puts("\tcompress    -  Compress input data.");
    puts("\tdecompress  -  Decompress input data.");

    return 1;
}
