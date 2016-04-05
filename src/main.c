//
// Data Compression Project -- CS 4310
//
// @author Alec Carpenter <alecgunnar@wmich.edu>
// @date   April 3, 2016
//

#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>

#include "lz77.h"
#include "error.h"

const char *EXEC_NAME;

int input_file;

int do_compressor();
int do_decompressor();
int help();
int run_cmd(char *cmd);

int main (int argc, char **argv) {
    EXEC_NAME = argv[0];

    if (argc < 2)
        exit(help());

    input_file = STDIN_FILENO;

    if (argc == 3 && (input_file = open(argv[2], O_RDONLY, S_IREAD)) < 0)
        error("Could not open input file");

    exit(run_cmd(argv[1]) == 0);
}

int run_cmd (char *cmd) {
    switch (cmd[0]) {
        case 'c':
            return compress(input_file);
        case 'd':
            return decompress(input_file);
    }

    if (input_file != STDIN_FILENO)
        close(input_file);

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
