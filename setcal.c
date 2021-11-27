#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils/loader.h"
#include "utils/parser.h"

int main(int argc, char **argv) {
    FILE *file = NULL;
    char *line = NULL;
    struct program_params_t program_params;

    if (!program_params_construct(&program_params, argc, argv)) {
        print_program_usage();
        return EXIT_FAILURE;
    }

    if (!load_file(program_params, &file)) {
        return EXIT_FAILURE;
    }
    program_params_destruct(&program_params);

    while (get_line(file, &line)) {
        if(strlen(line) != 0) {
            printf("%s\n", line);
        }
    }



    fclose(file);
    free(line);
    line = NULL;

    return 0;
}
