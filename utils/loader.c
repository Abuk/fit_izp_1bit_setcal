//
// Created by Jakub Schenk on 27.11.2021.
//

#include "loader.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG

int load_file(struct program_params_t params, FILE **file) {
    *file = fopen(params.file_path, "r");
    if (*file == NULL) {
        fprintf(stderr, "fopen: failed to open file %s\n", params.file_path);
        return 0;
    }

    return 1;
}

int line_allocator(char **line_buffer, size_t alloc_size) {
    *line_buffer = (char *) malloc(sizeof(char) * alloc_size);
    if (*line_buffer == NULL) {
#ifdef DEBUG
        fprintf(stderr, "line_allocator: malloc failed\n");
#endif
        return 0;
    }
    return 1;
}

int line_reallocator(char **line_buffer, size_t alloc_size) {
    *line_buffer = (char *) realloc(*line_buffer, sizeof(char) * alloc_size);
    if (*line_buffer == NULL) {
#ifdef DEBUG
        fprintf(stderr, "line_reallocator: realloc failed\n");
#endif
        return 0;
    }
    return 1;
}

int get_line(FILE *file, char **line) {
    size_t allocation_chunk = 128;
    size_t current_allocation_size = allocation_chunk;
    size_t char_count;

    char *line_buffer = NULL;
    char char_buffer;

    if (line_allocator(&line_buffer, allocation_chunk) == 0) {
        return 0;
    }

    for (char_count = 0;; ++char_count) {
        char_buffer = fgetc(file);
        if (char_buffer == '\n') {
            line_buffer[char_count++] = '\0';
            break;
        }
        if(char_buffer == EOF) {
            line_buffer[char_count++] = '\0';
            return 0;
        }
        if (char_count == current_allocation_size - 1) {
            if (!line_reallocator(&line_buffer, (current_allocation_size += allocation_chunk))) {
                return 0;
            }

        }
        line_buffer[char_count] = char_buffer;
    }

    if (!line_reallocator(&line_buffer, char_count)) {
        return 0;
    }

    *line = (char *) malloc(sizeof(char) * char_count);
    if(*line == NULL) {
        fprintf(stderr, "loader: allocation error");
        return 0;
    }

    strcpy(*line, line_buffer);

    // dealloc buffer
    free(line_buffer);
    line_buffer = NULL;

    return 1;
}