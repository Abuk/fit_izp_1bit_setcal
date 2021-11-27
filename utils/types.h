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
    element_t *elements;
    size_t size;
};

struct relation_t {
    struct pair_t *pairs;
    size_t size;
};

// universe functions
void universe_construct(struct universe_t *universe);

void universe_destruct(struct universe_t *universe);

int universe_push(struct universe_t *universe, struct universe_member_t member);

// set functions
int set_construct(struct set_t *set);

void set_destruct(struct set_t *set);

int set_push(struct set_t *set, element_t element);

// relation functions
int relation_construct(struct relation_t *relation, size_t size);

void relation_destruct(struct relation_t *relation);

int relation_push(struct relation_t *relation, struct pair_t pair);

// pair functions
struct pair_t new_pair(element_t x, element_t y);

// universe member functions
struct universe_member_t new_universe_member(element_t id, char *name);

#endif //PROJECT_02_TYPES_H
