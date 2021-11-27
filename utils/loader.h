//
// Created by Jakub Schenk on 27.11.2021.
//

#ifndef PROJECT_02_LOADER_H
#define PROJECT_02_LOADER_H

#include <stdio.h>
#include "parser.h"

int load_file(struct program_params_t params, FILE **file);

int get_line(FILE *file, char **line);

#endif //PROJECT_02_LOADER_H
