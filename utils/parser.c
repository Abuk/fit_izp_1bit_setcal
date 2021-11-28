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

const char *keywords[] = {
        "U"
};

void print_program_usage() {
    fprintf(stderr, "usage:\n./setcal FILE\n");
    fprintf(stderr, "FILE: a relative or absolute path to your file with sets or relations\n");
}

int program_params_construct(int argc, char **argv) {
    size_t program_path_size, file_path_size;

    // return failure if there is more or less args
    if (argc != ARG_COUNT) {
#ifdef DEBUG
        fprintf(stderr, "programs_params_construct: insufficient number of args\n");
#endif
        return 0;
    }

    program_path_size = strlen(argv[0]) + 1;
    file_path_size = strlen(argv[1]) + 1;

    program_params.program_path = (char *) malloc(sizeof(char) * program_path_size);
    program_params.file_path = (char *) malloc(sizeof(char) * file_path_size);

    if (program_params.program_path == NULL || program_params.file_path == NULL) {
#ifdef DEBUG
        fprintf(stderr, "fopen: could not allocate memory for program params\n");
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
    register uint16_t current_line_number = 1;
    switch (line[0]) {
        case 'U': {
            if (universe.size != 0) {
                fprintf(stderr, "parser: you cannot have more than 1 universe\n");
                return 0;
            }
            if (!parse_universe(line)) {
#ifdef DEBUG
                fprintf(stderr, "universe: an error has occurred while parsing on line %d\n",
                        current_line_number);
                return 0;
#endif
            }
            break;
        }
        case 'S': {
            sets = (struct set_t *) realloc(sets, sizeof(struct set_t) * (set_count + 1));
            if (sets == NULL) {
#ifdef DEBUG
                fprintf(stderr, "malloc: allocation error\n");
#endif
                return 0;
            }
            if (!parse_set(set_count, line)) {
#ifdef DEBUG
                fprintf(stderr, "set: an error has occurred while parsing on line %d\n",
                        current_line_number);
                return 0;
#endif
            }
            set_count++;

            break;
        }
        case 'R': {
            relations = (struct relation_t *) realloc(relations, sizeof(struct relation_t) * (relation_count + 1));
            if (relations == NULL) {
#ifdef DEBUG
                fprintf(stderr, "malloc: allocation error\n");
#endif
                return 0;
            }
            if (!parse_relation(relation_count, line)) {
#ifdef DEBUG
                fprintf(stderr, "relation: an error has occurred while parsing on line %d\n",
                        current_line_number);
#endif
                return 0;
            }
            relation_count++;

            break;
        }
//        case 'C': {
//            if (universe.size != 0) {
//                fprintf(stderr, "parser: you cannot have more than 1 universe");
//                return EXIT_FAILURE;
//            }
//            parse_universe(line);
//            break;
//        }
    }

    current_line_number++;
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
        if (get_universe_member_id_by_name(universe, substr) != -1) {
#ifdef DEBUG
            fprintf(stderr, "universe: universe member [%s] already exists and cannot be redeclared twice", substr);
#endif
            return 0;
        }
        universe_push(&universe, member);
        substr = strtok(NULL, " ");
    } while (substr != NULL);

    return 1;
}

int parse_set(int set_pos, char *line) {
    char *substr;
    substr = strtok(line, " ");
    sets[set_pos] = set_construct();
    do {
        if (!strcmp(substr, "S")) {
            substr = strtok(NULL, " ");
            continue;
        }

        set_push(&sets[set_pos], get_universe_member_id_by_name(universe, substr));
        substr = strtok(NULL, " ");
    } while (substr != NULL);


    return 1;
}

int parse_relation(int relation_pos, char *line) {
    char *substr;
    substr = strtok(line, " ()");
    relations[relation_pos] = relation_construct();
    do {
        if (!strcmp(substr, "S")) {
            substr = strtok(NULL, " ()");
            continue;
        }

        int x, y;
        substr = strtok(NULL, " (");
        if (substr == NULL) {
            break;
        }
        x = get_universe_member_id_by_name(universe, substr);

        substr = strtok(NULL, " )");
        if (substr == NULL) {
#ifdef DEBUG
            fprintf(stderr, "relation: pair has only one member");
#endif
            return 0;
        }
        y = get_universe_member_id_by_name(universe, substr);


        if (x == -1 || y == -1) {
#ifdef DEBUG
            fprintf(stderr, "relation: relation is trying to use non existent element! x: %d y: %d\n", x, y);
            fprintf(stderr, "relation: failing substr [%s]\n", substr);
#endif
            return 0;
        }

        relation_push(&relations[relation_pos], new_pair(x, y));
    } while (substr != NULL);


    return 1;
}