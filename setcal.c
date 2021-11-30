#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG
#define ARG_COUNT 2
#define MAX_LINE_COUNT 1000

// parser
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

typedef unsigned int element_t;

struct pair_t {
    element_t x;
    element_t y;
};

struct universe_member_t {
    element_t id;
    char *name;
};

struct universe_t {
    struct universe_member_t *elements;
    size_t size;
};


struct set_t {
    size_t size;
    element_t *elements;
};

struct relation_t {
    struct pair_t *pairs;
    size_t size;
};

const char *keywords[] = {
        "U", "S", "R", "C", "true", "false", "empty", "card", "complement", "union", "intersect", "minus", "subseteq",
        "subset", "equals", "reflexive", "symmetric", "antisymmetric", "transitive",
        "function", "domain", "injective", "surjective", "bijective"
};

struct program_params_t program_params;
struct universe_t universe;
struct set_t *sets;
struct relation_t *relations;

int set_count;
int relation_count;

// universe functions
struct universe_t universe_construct();

void universe_destruct(struct universe_t *universe);

int universe_push(struct universe_t *universe, struct universe_member_t member);

int get_universe_member_id_by_name(struct universe_t universe, char *name);

char *get_universe_member_name_by_id(struct universe_t universe, element_t id);


// set functions
struct set_t set_construct();

void set_destruct(struct set_t *set);

int set_push(struct set_t *set, element_t element);

// relation functions
struct relation_t relation_construct();

void relation_destruct(struct relation_t *relation);

int relation_push(struct relation_t *relation, struct pair_t pair);

// pair functions
struct pair_t new_pair(element_t x, element_t y);

// universe member functions
struct universe_member_t new_universe_member(element_t id, char *name);

void print_program_usage();

int program_params_construct(int argv, char **argc);

void program_params_destruct();

int parse_line(char *line);

int parse_universe(char *universe_string);

int parse_set(int set_pos, char *line);

int parse_relation(int relation_pos, char *line);

int parse_command(char *line);

// loader
int load_file(struct program_params_t params, FILE **file);

int get_line(FILE *file, char **line);

// functions
void set_empty(struct set_t set);

void set_card(struct set_t set);

void set_complement(struct set_t set);

void set_union(struct set_t set);

void set_intersect(struct set_t set_a, struct set_t set_b);

void set_minus(struct set_t set_a, struct set_t set_b);

void set_subseteq(struct set_t set_a, struct set_t set_b);

void set_subset(struct set_t set_a, struct set_t set_b);

void set_equals(struct set_t set_a, struct set_t set_b);

// relations
void rel_reflexive(struct relation_t relation);

void rel_symmetric(struct relation_t relation);

void rel_antisymmetric(struct relation_t relation);

void rel_transitive(struct relation_t relation);

void rel_function(struct relation_t relation);

void rel_domain(struct relation_t relation);

void rel_codomain(struct relation_t relation);

void rel_injective(struct relation_t relation, struct set_t set_a, struct set_t set_b);

void rel_surjective(struct relation_t relation, struct set_t set_a, struct set_t set_b);

void rel_bijective(struct relation_t relation, struct set_t set_a, struct set_t set_b);

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

int main(int argc, char **argv) {
    FILE *file = NULL;
    char *line = NULL;
    int line_parser_result;

    if (!program_params_construct(argc, argv)) {
        print_program_usage();
        return EXIT_FAILURE;
    }

    if (!load_file(program_params, &file)) {
        return EXIT_FAILURE;
    }
    program_params_destruct();
    universe_construct();

    while ((line_parser_result = get_line(file, &line)) != 0) {
        if (parse_line(line) == 0) {
            return EXIT_FAILURE;
        }
        if (line_parser_result == EOF) {
            break;
        }
    }

    // deallocate universe
    universe_destruct(&universe);

    // deallocate sets
    for (int i = 0; i < set_count; ++i) {
        set_destruct(&sets[i]);
    }
    free(sets);
    sets = NULL;

    // deallocate relations
    for (int i = 0; i < relation_count; ++i) {
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

// universe functions
struct universe_t universe_construct() {
    struct universe_t universe;

    universe.elements = NULL;
    universe.size = 0;

    return universe;
}

void universe_destruct(struct universe_t *universe) {
    for (size_t i = 0; i < universe->size; ++i) {
        free(universe->elements[i].name);
        universe->elements[i].name = NULL;
    }

    free(universe->elements);
    universe->elements = NULL;
}

int universe_push(struct universe_t *universe, struct universe_member_t member) {
    universe->elements = (struct universe_member_t *) realloc(universe->elements,
            sizeof(struct universe_member_t) * (universe->size + 1));
    if (universe->elements == NULL) {
#ifdef DEBUG
        fprintf(stderr, "realloc: allocation error");
#endif
        return 0;
    }

    universe->size++;
    universe->elements[universe->size - 1].name = (char *) malloc(sizeof(char) * (strlen(member.name) + 1));
    if (universe->elements[universe->size - 1].name == NULL) {
#ifdef DEBUG
        fprintf(stderr, "realloc: allocation error");
#endif
        return 0;
    }
    strcpy(universe->elements[universe->size - 1].name, member.name);
    universe->elements[universe->size - 1].id = universe->size;

    return 1;
}

int get_universe_member_id_by_name(struct universe_t universe, char *name) {
    for (size_t i = 0; i < universe.size; ++i) {
        if (!strcmp(universe.elements[i].name, name)) {
            return universe.elements[i].id;
        }
    }

    return -1;
}

char *get_universe_member_name_by_id(struct universe_t universe, element_t id) {
    for (size_t i = 0; i < universe.size; ++i) {
        if (universe.elements[i].id == id) {
            return universe.elements[i].name;
        }
    }

    return "";
}


// set functions
struct set_t set_construct() {
    struct set_t set;

    set.elements = (element_t *) malloc(0);
    set.size = 0;

    return set;
}

void set_destruct(struct set_t *set) {
    free(set->elements);
    set->elements = NULL;
}

int set_push(struct set_t *set, element_t element) {
    size_t new_allocation_size = sizeof(element_t) * (set->size + 1);
    set->elements = realloc(set->elements,
                            new_allocation_size);

    if (set->elements == NULL) {
#ifdef DEBUG
        fprintf(stderr, "realloc: allocation error");
#endif
        return 0;
    }

    set->elements[set->size++] = element;

    return 1;
}

// relation functions
struct relation_t relation_construct() {
    struct relation_t relation;

    relation.pairs = NULL;
    relation.size = 0;

    return relation;
}

void relation_destruct(struct relation_t *relation) {
    free(relation->pairs);
    relation->pairs = NULL;
}

int relation_push(struct relation_t *relation, struct pair_t pair) {
    relation->pairs = (struct pair_t *) realloc(relation->pairs,
            sizeof(struct pair_t) * (relation->size + 1));
    if (relation->pairs == NULL) {
#ifdef DEBUG
        fprintf(stderr, "realloc: allocation error");
#endif
        return 0;
    }

    relation->size++;
    relation->pairs[relation->size - 1] = pair;

    return 1;
}

// pair functions
struct pair_t new_pair(element_t x, element_t y) {
    struct pair_t pair;

    pair.x = x;
    pair.y = y;

    return pair;
}

// universe member functions
struct universe_member_t new_universe_member(element_t id, char *name) {
    struct universe_member_t member;

    member.id = id;
    member.name = (char *) malloc(sizeof(char) * (strlen(name) + 1));

    if (member.name == NULL) {
        fprintf(stderr, "malloc: allocation error");
    }

    strcpy(member.name, name);

    return member;
}



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
            break;
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
    if(char_buffer == EOF) {
        return EOF;
    }
    return 1;
}

void set_empty(struct set_t set) {
    if (set.size == 0) {
        printf("true\n");
        return;
    }
    printf("false\n");
}

void set_card(struct set_t set) {
    if (set.size == 0) {
        printf("true");
        return;
    }
    printf("false");
}

void set_complement(struct set_t set) {
    if (set.size == 0) {
        printf("true");
        return;
    }
    printf("false");
}

void set_union(struct set_t set) {
    if (set.size == 0) {
        printf("true");
        return;
    }
    printf("false");
}

void set_intersect(struct set_t set_a, struct set_t set_b) {
    if (set_a.size == 0 && set_b.size == 0) {
        printf("true");
        return;
    }
    printf("false");
}

void set_minus(struct set_t set_a, struct set_t set_b) {
    if (set_a.size == 0 && set_b.size == 0) {
        printf("true");
        return;
    }
    printf("false");
}

void set_subseteq(struct set_t set_a, struct set_t set_b) {
    if (set_a.size == 0 && set_b.size == 0) {
        printf("true");
        return;
    }
    printf("false");
}

void set_subset(struct set_t set_a, struct set_t set_b) {
    if (set_a.size == 0 && set_b.size == 0) {
        printf("true");
        return;
    }
    printf("false");
}

void set_equals(struct set_t set_a, struct set_t set_b) {
    if (set_a.size == 0 && set_b.size == 0) {
        printf("true");
        return;
    }
    printf("false");
}

int in_array(int value, int *arr, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        if (value == arr[i]) {
            return 1;
        }
    }
    return 0;
}

void rel_reflexive(struct relation_t relation) {
    element_t *contains = NULL;
    size_t contains_size = 0;
    for (size_t i = 0; i < relation.size; ++i) {
        if (!in_array(relation.pairs[i].x, (int *) contains, contains_size)) {
            contains = realloc(contains, sizeof(int) * (contains_size + 1));
            if (contains == NULL) {
#ifdef DEBUG
                fprintf(stderr, "malloc: allocation error");
#endif
            }
            contains[contains_size++] = relation.pairs[i].x;
        }
        if (!in_array(relation.pairs[i].y, (int *) contains, contains_size)) {
            contains = realloc(contains, sizeof(int) * (contains_size + 1));
            if (contains == NULL) {
#ifdef DEBUG
                fprintf(stderr, "malloc: allocation error");
#endif
            }
            contains[contains_size++] = relation.pairs[i].y;
        }
    }

    for (size_t i = 0; i < contains_size; ++i) {
        unsigned short relation_found = 0;
        for (size_t j = 0; j < relation.size; ++j) {
            if (contains[i] == relation.pairs[j].x && contains[i] == relation.pairs[j].y) {
                relation_found = 1;
                break;
            }
        }
        if(relation_found == 0) {
            printf("false\n");
            return;
        }
    }

    printf("true\n");
}

void rel_symmetric(struct relation_t relation) {
    if (relation.size == 0) {
        printf("true");
        return;
    }
    printf("false");
}

void rel_antisymmetric(struct relation_t relation) {
    if (relation.size == 0) {
        printf("true");
        return;
    }
    printf("false");
}

void rel_transitive(struct relation_t relation) {
    if (relation.size == 0) {
        printf("true");
        return;
    }
    printf("false");
}

void rel_function(struct relation_t relation) {
    if (relation.size == 0) {
        printf("true");
        return;
    }
    printf("false");
}

void rel_domain(struct relation_t relation) {
    if (relation.size == 0) {
        printf("true");
        return;
    }
    printf("false");
}

void rel_codomain(struct relation_t relation) {
    if (relation.size == 0) {
        printf("true");
        return;
    }
    printf("false");
};;

void rel_injective(struct relation_t relation, struct set_t set_a, struct set_t set_b) {
    if (relation.size == 0 && set_a.size == 0 && set_b.size == 0) {
        printf("true");
        return;
    }
    printf("false");
}

void rel_surjective(struct relation_t relation, struct set_t set_a, struct set_t set_b) {
    if (relation.size == 0 && set_a.size == 0 && set_b.size == 0) {
        printf("true");
        return;
    }
    printf("false");
}

void rel_bijective(struct relation_t relation, struct set_t set_a, struct set_t set_b) {
    if (relation.size == 0 && set_a.size == 0 && set_b.size == 0) {
        printf("true");
        return;
    }
    printf("false");
}