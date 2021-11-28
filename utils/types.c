//
// Created by Jakub Schenk on 27.11.2021.
//

#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG

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
