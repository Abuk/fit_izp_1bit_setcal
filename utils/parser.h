//
// Created by Jakub Schenk on 27.11.2021.
//

#ifndef PROJECT_02_PARSER_H
#define PROJECT_02_PARSER_H

struct program_params_t {
    char *program_path;
    char *file_path;
};

void print_program_usage();
int program_params_construct(struct program_params_t* params, int argv, char** argc);
void program_params_destruct(struct program_params_t* params);


#endif //PROJECT_02_PARSER_H
