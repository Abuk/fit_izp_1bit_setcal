#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils/loader.h"
#include "utils/parser.h"
#include "utils/types.h"

struct program_params_t program_params;
struct universe_t universe;
struct set_t *sets;
struct relation_t *relations;

int set_count;
int relation_count;

int main(int argc, char **argv) {
    FILE *file = NULL;
    char *line = NULL;

    if (!program_params_construct(argc, argv)) {
        print_program_usage();
        return EXIT_FAILURE;
    }

    if (!load_file(program_params, &file)) {
        return EXIT_FAILURE;
    }
    program_params_destruct();
    universe_construct();

    while (get_line(file, &line)) {
        if (strlen(line) != 0) {
            if(!parse_line(line)) {
                return EXIT_FAILURE;
            }
        }
    }

    // deallocate universe
    universe_destruct(&universe);

    // deallocate sets
    for(int i = 0; i < set_count; ++i) {
        set_destruct(&sets[i]);
    }
    free(sets);
    sets = NULL;

    // deallocate relations
    for(int i = 0; i < relation_count; ++i) {
        relation_destruct(&relations[i]);
    }
    free(relations);
    relations = NULL;

    fclose(file);
    file = NULL;
    free(line);
    line = NULL;

    return 0;
}
