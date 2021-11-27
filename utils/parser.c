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

void print_program_usage() {
    fprintf(stderr, "usage:\n./setcal FILE\n");
    fprintf(stderr, "FILE: a relative or absolute path to your file with sets or relations\n");
}

int program_params_construct(struct program_params_t *params, int argc, char **argv) {
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

    params->program_path = (char *) malloc(sizeof(char) * program_path_size);
    params->file_path = (char *) malloc(sizeof(char) * file_path_size);

    if (params->program_path == NULL || params->file_path == NULL) {
#ifdef DEBUG
        fprintf(stderr, "fopen: could not allocate memory for program params");
#endif
        return 0;
    }

    strcpy(params->program_path, argv[0]);
    strcpy(params->file_path, argv[1]);

    return 1;
}

void program_params_destruct(struct program_params_t* params) {
    free(params->file_path);
    free(params->program_path);
    params->file_path = NULL;
    params->program_path = NULL;
}