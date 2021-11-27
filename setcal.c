#include <stdio.h>
#include <stdlib.h>
#include "utils/loader.h"
#include "utils/parser.h"

int main(int argc, char** argv) {
    FILE* file = NULL;
    char* line = NULL;
    struct program_params_t program_params;

    if(program_params_construct(&program_params, argc, argv) == 0) {
        print_program_usage();
        return EXIT_FAILURE;
    }

    load_file(program_params, file);
    program_params_destruct(&program_params);

    while(get_line(file, line) != EOF) {
        printf("%s", line);
    }

    free(line);
    line = NULL;

    return 0;
}
