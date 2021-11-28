//
// Created by Jakub Schenk on 27.11.2021.
//

#ifndef PROJECT_02_PARSER_H
#define PROJECT_02_PARSER_H
#define DEBUG

#include "types.h"

struct program_params_t {
    char *program_path;
    char *file_path;
};

struct function_t {
    const char *name;
    const char *alias;

    void (*p_func)();

    int n_args;
};

extern struct function_t set_function_table[];
extern struct function_t relation_function_table[];

void print_program_usage();

int program_params_construct(int argv, char **argc);

void program_params_destruct();

int parse_line(char *line);

int parse_universe(char *universe_string);

int parse_set(int set_pos, char *line);

int parse_relation(int relation_pos, char *line);

int parse_command(char *line);

#endif //PROJECT_02_PARSER_H
