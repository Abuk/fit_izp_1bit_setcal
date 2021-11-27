//
// Created by Jakub Schenk on 27.11.2021.
//

#include "parser.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define ARG_COUNT 2

#ifndef DEBUG
#define DEBUG
#endif

struct program_params_t program_params;
struct universe_t universe;
struct set_t *sets = NULL;
struct relation_t *relations = NULL;

int set_count = 0;
int relation_count = 0;

void print_program_usage() {
    fprintf(stderr, "usage:\n./setcal FILE\n");
    fprintf(stderr, "FILE: a relative or absolute path to your file with sets or relations\n");
}

int program_params_construct(int argc, char **argv) {
    size_t program_path_size, file_path_size;

    // return failure if there is more or less args
    if (argc != ARG_COUNT) {
#ifdef DEBUG
        fprintf(stderr, "programs_params_construct: insufficient number of args");
#endif
        return 0;
    }

    program_path_size = strlen(argv[0]) + 1;
    file_path_size = strlen(argv[1]) + 1;
#ifdef DEBUG
    printf("file_path_size: %lu\n", file_path_size);
#endif

    program_params.program_path = (char *) malloc(sizeof(char) * program_path_size);
    program_params.file_path = (char *) malloc(sizeof(char) * file_path_size);

    if (program_params.program_path == NULL || program_params.file_path == NULL) {
#ifdef DEBUG
        fprintf(stderr, "fopen: could not allocate memory for program params");
#endif
        return 0;
    }

    strcpy(program_params.program_path, argv[0]);
    strcpy(program_params.file_path, argv[1]);

    return 1;
}

void program_params_destruct() {
    free(program_params.file_path);
    free(program_params.program_path);
    program_params.file_path = NULL;
    program_params.program_path = NULL;
}

int parse_line(char *line) {
    switch (line[0]) {
        case 'U': {
            if (universe.size != 0) {
                fprintf(stderr, "parser: you cannot have more than 1 universe");
                return 0;
            }
            parse_universe(line);
            break;
        }
        case 'S': {
            sets = (struct set_t *) realloc(sets, sizeof(struct set_t) * (set_count + 1));
            if (sets == NULL) {
#ifdef DEBUG
                fprintf(stderr, "malloc: allocation error");
#endif
                return 0;
            }
            //   printf("set size: %lu set_element[0]: %du", new_set.size, new_set.elements[0]);
            if(!parse_set(set_count + 1, line)) {
                set_count++;
            }
            break;
        }
//        case 'R': {
//            if (universe.size != 0) {
//                fprintf(stderr, "parser: you cannot have more than 1 universe");
//                return EXIT_FAILURE;
//            }
//            parse_universe(line);
//            break;
//        }
//        case 'C': {
//            if (universe.size != 0) {
//                fprintf(stderr, "parser: you cannot have more than 1 universe");
//                return EXIT_FAILURE;
//            }
//            parse_universe(line);
//            break;
//        }
    }
    return 1;
}

int parse_universe(char *universe_string) {
    char *substr;
    int i = 0;
    substr = strtok(universe_string, " ");
    do {
        if (!strcmp(substr, "U")) {
            substr = strtok(NULL, " ");
            continue;
        }
        struct universe_member_t member = new_universe_member(i++, substr);
        universe_push(&universe, member);
        substr = strtok(NULL, " ");
    } while (substr != NULL);

    printf("Universe[1]: %d, %s\n", universe.elements[1].id, universe.elements[1].name);
    return 0;
}

int parse_set(int set_pos, char *line) {
    char *substr;
    substr = strtok(line, " ");
    do {
        if (!strcmp(substr, "S")) {
            substr = strtok(NULL, " ");
            continue;
        }
        set_push(&sets[set_pos-1], get_universe_member_id_by_name(universe, substr));
        substr = strtok(NULL, " ");
    } while (substr != NULL);

    printf("Set[0]: %d\n", sets[0].elements[0]);
    return 1;
}