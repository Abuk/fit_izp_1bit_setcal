//
// Created by Jakub Schenk on 27.11.2021.
//

#ifndef PROJECT_02_PARSER_H
#define PROJECT_02_PARSER_H

#include "types.h"

extern struct program_params_t program_params;
extern struct universe_t universe;
extern struct set_t *sets;
extern struct relation_t *relations;

struct program_params_t {
    char *program_path;
    char *file_path;
};

void print_program_usage();

int program_params_construct(int argv, char **argc);

void program_params_destruct();

int parse_line(char *line);

int parse_universe(char *universe_string);

int parse_set(int set_pos, char *line);


#endif //PROJECT_02_PARSER_H
