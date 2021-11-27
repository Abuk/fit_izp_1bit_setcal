//
// Created by Jakub Schenk on 27.11.2021.
//

#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG

// universe functions
void universe_construct(struct universe_t *universe) {
    universe->elements = NULL;
    universe->size = 0;
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
    if(universe->elements == NULL) {
#ifdef DEBUG
        fprintf(stderr, "realloc: allocation error");
#endif
        return 0;
    }

    universe->size++;
    universe->elements[universe->size - 1] = member;
    return 1;
}

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
