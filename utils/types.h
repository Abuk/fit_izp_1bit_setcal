//
// Created by Jakub Schenk on 27.11.2021.
//

#ifndef PROJECT_02_TYPES_H
#define PROJECT_02_TYPES_H

#include <stdio.h>

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

extern const char *keywords[];

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

#endif //PROJECT_02_TYPES_H
