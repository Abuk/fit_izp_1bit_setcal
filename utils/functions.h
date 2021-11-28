//
// Created by Jakub Schenk on 28.11.2021.
//

#ifndef PROJECT_02_FUNCTIONS_H
#define PROJECT_02_FUNCTIONS_H

#include "types.h"

void set_empty(struct set_t set);

void set_card(struct set_t set);

void set_complement(struct set_t set);

void set_union(struct set_t set);

void set_intersect(struct set_t set_a, struct set_t set_b);

void set_minus(struct set_t set_a, struct set_t set_b);

void set_subseteq(struct set_t set_a, struct set_t set_b);

void set_subset(struct set_t set_a, struct set_t set_b);

void set_equals(struct set_t set_a, struct set_t set_b);

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

#endif //PROJECT_02_FUNCTIONS_H
