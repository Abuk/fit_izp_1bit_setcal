//
// Created by Jakub Schenk on 27.11.2021.
//

#include "parser.h"
#include "functions.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define ARG_COUNT 2
#define MAX_LINE_COUNT 1000

struct program_params_t program_params;
struct universe_t universe;
struct set_t *sets = NULL;
struct relation_t *relations = NULL;

int set_count = 0;
int relation_count = 0;

#define REGISTER(FUNC, ALIAS, N_ARGS) { #FUNC, ALIAS, &FUNC, N_ARGS }
struct function_t set_function_table[] = {
        REGISTER(set_empty, "empty", 1),
        REGISTER(set_card, "card", 1),
        REGISTER(set_complement, "complement", 1),
        REGISTER(set_union, "union", 2),
        REGISTER(set_intersect, "intersect", 2),
        REGISTER(set_minus, "minus", 2),
        REGISTER(set_subseteq, "subseteq", 2),
        REGISTER(set_subset, "subset", 2),
        REGISTER(set_equals, "equals", 2),
};

struct function_t relation_function_table[] = {
        REGISTER(rel_reflexive, "reflexive", 1),
        REGISTER(rel_symmetric, "symmetric", 1),
        REGISTER(rel_antisymmetric, "antisymmetric", 1),
        REGISTER(rel_transitive, "transitive", 1),
        REGISTER(rel_function, "function", 1),
        REGISTER(rel_domain, "domain", 1),
        REGISTER(rel_codomain, "codomain", 1),
        REGISTER(rel_injective, "injective", 3),
        REGISTER(rel_surjective, "surjective", 3),
        REGISTER(rel_bijective, "bijective", 3),
};
#undef REGISTER

enum {
    set_function_table_size = sizeof set_function_table / sizeof *set_function_table,
    rel_function_table_size = sizeof relation_function_table / sizeof *relation_function_table
};

const char *keywords[] = {
        "U", "S", "R", "C", "true", "false", "empty", "card", "complement", "union", "intersect", "minus", "subseteq",
        "subset", "equals", "reflexive", "symmetric", "antisymmetric", "transitive",
        "function", "domain", "injective", "surjective", "bijective"
};

int keywords_contain(const char *string) {
    for (size_t i = 0; i < (sizeof keywords / sizeof *keywords); ++i) {
        if (!strcmp(string, keywords[i])) {
            return 1;
        }
    }
    return 0;
}

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
    static unsigned short current_line_number = 1;

    if (strlen(line) == 0) {
        current_line_number++;
        return 1;
    }

    if (line[0] != 'C') {
        printf("%s\n", line);
    }

    if (current_line_number == MAX_LINE_COUNT + 1) {
#ifdef DEBUG
        fprintf(stderr, "setcal: line limit exceeded (maximum lines: %d)", MAX_LINE_COUNT);
#endif
    }
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
        case 'C': {
            if (!parse_command(line)) {
#ifdef DEBUG
                fprintf(stderr, "command: an error has occurred while parsing on line %d\n",
                        current_line_number);
#endif
                return 0;
            }
            break;
        }
        default: {
#ifdef DEBUG
            fprintf(stderr, "line_parser: syntax error on line %d\n", current_line_number);
#endif
            return 0;
        }
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
        if (keywords_contain(substr)) {
#ifdef DEBUG
            fprintf(stderr, "universe: member [%s] cannot be one of reserved keywords\n", substr);
#endif
            return 0;
        }
        struct universe_member_t member = new_universe_member(i++, substr);
        if (get_universe_member_id_by_name(universe, substr) != -1) {
#ifdef DEBUG
            fprintf(stderr, "universe: member [%s] already exists and cannot be redeclared twice\n", substr);
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
        if (keywords_contain(substr)) {
#ifdef DEBUG
            fprintf(stderr, "set: member [%s] cannot be one of reserved keywords\n", substr);
#endif
            return 0;
        }
        set_push(&sets[set_pos], get_universe_member_id_by_name(universe, substr));
        substr = strtok(NULL, " ");
    } while (substr != NULL);

    return 1;
}

int parse_relation(int relation_pos, char *line) {
    char *substr;
    strtok(line, " ");

    relations[relation_pos] = relation_construct();
    do {
        int x, y;
        substr = strtok(NULL, " (");
        if (substr == NULL) {
            break;
        }
        x = get_universe_member_id_by_name(universe, substr);

        substr = strtok(NULL, " )");
        if (substr == NULL) {
#ifdef DEBUG
            fprintf(stderr, "relation: pair has only one member\n");
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

void free_array(int **arr) {
    free(*arr);
    *arr = NULL;
}

int parse_command(char *line) {
    char *function;
    struct function_t *func;
    int func_type = 0;
    int *args = NULL;

    strtok(line, " ");
    function = strtok(NULL, " ");

    if (function == NULL) {
#ifdef DEBUG
        fprintf(stderr, "command: missing function name\n");
#endif
        return 0;
    }
    for (int i = 0; i < set_function_table_size; ++i) {
        if (strcmp(function, set_function_table[i].alias) == 0) {
            func = &set_function_table[i];
            func_type = 'S';
            break;
        }
    }

    for (int i = 0; i < rel_function_table_size; ++i) {
        if (strcmp(function, relation_function_table[i].alias) == 0) {
            func = &relation_function_table[i];
            func_type = 'R';
            break;
        }
    }

    if (func_type == 0) {
#ifdef DEBUG
        fprintf(stderr, "command: function %s does not exist\n", function);
#endif
        return 0;
    }

    char *arg = strtok(NULL, " ");
    int k = 0;
    while (arg != NULL) {
        args = realloc(args, sizeof(int) * (k + 1));
        if (args == NULL) {
#ifdef DEBUG
            fprintf(stderr, "command: malloc: allocation error\n");
#endif
            return 0;
        }
        args[k++] = strtol(arg, NULL, 10);
        if (args[k - 1] == 0) {
#ifdef DEBUG
            fprintf(stderr, "command: strtol: invalid argument value - expected number > 0\n");
#endif
        }
        arg = strtok(NULL, " ");
    }

    if (func->n_args != k) {
        free_array(&args);
        printf("relation: %d", args[0]);
#ifdef DEBUG
        fprintf(stderr, "command: function %s expects %d arguments (%d provided)\n", func->alias, func->n_args, k);
#endif
        return 0;
    }

    switch (func_type) {
        case 'S': {
            switch (func->n_args) {
                case 1: {
                    if (args[0] > set_count) {
                        free_array(&args);
#ifdef DEBUG
                        fprintf(stderr, "command_parser: argument referencing undefined set\n");
#endif
                        return 0;
                    }
                    (*func->p_func)(sets[args[0] - 1]);
                    return 1;
                }
                case 2: {
                    if (args[0] > set_count || args[1] > set_count) {
                        free_array(&args);
#ifdef DEBUG
                        fprintf(stderr, "command_parser: argument referencing undefined set\n");
#endif
                        return 0;
                    }
                    (*func->p_func)(sets[args[0] - 1], sets[args[1] - 1]);
                    break;
                }
            }
            break;
        }
        case 'R': {
            switch (func->n_args) {
                case 1: {
                    if (args[0] > relation_count) {
                        free_array(&args);
#ifdef DEBUG
                        fprintf(stderr, "command_parser: argument referencing undefined set\n");
#endif
                        return 0;
                    }
                    (*func->p_func)(relations[args[0] - 1]);
                    break;
                }
                case 3: {
                    if (args[0] > relation_count || args[1] > set_count || args[2] > set_count) {
                        free_array(&args);
#ifdef DEBUG
                        fprintf(stderr, "command_parser: argument referencing undefined set\n");
#endif
                        return 0;
                    }
                    (*func->p_func)(relations[args[0] - 1], sets[args[1] - 1], sets[args[2] - 1]);
                    break;
                }
            }
            break;
        }
    }

    free_array(&args);

    return 1;
}