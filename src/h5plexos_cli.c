#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "../lib/h5plexos.h"

int main(int argc, char* argv[argc+1]) {

    if (argc != 3) {
        fprintf(stderr, "Expected 2 arguments (input file, output file), but received %d\n", argc);
        return EXIT_FAILURE;
    }

    char* infile = argv[1];
    char* outfile = argv[2];

    printf("Input file: %s\n", infile);
    printf("Output file: %s\n", outfile);

    h5plexos(infile, outfile);

    return EXIT_SUCCESS;

}
