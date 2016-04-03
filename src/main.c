//
// Data Compression Project -- CS 4310
//
// @author Alec Carpenter <alecgunnar@wmich.edu>
// @date   April 3, 2016
//

#include <stdio.h>

int do_help (char *exec_name);
int run_cmd (char *cmd, char *file);

int main (int argc, char **argv) {
    if (argc < 3)
        return do_help(argv[0]);

    return run_cmd(argv[2], argv[3]);
}

int run_cmd (char *cmd, char *file) {
    return 0;
}

int do_help (char *exec_name) {
    puts("Data Compression Project -- CS 4310");
    puts("Created by: Alec Carpenter\n");

    puts("Usage:");
    printf("\t%s help\n", exec_name);
    printf("\t%s command [file name]\n\n", exec_name);

    puts("Commands:");
    puts("\tcompress    -  Compress input data.");
    puts("\tdecompress  -  Decompress input data.");

    return 0;
}
