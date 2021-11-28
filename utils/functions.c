//
// Created by Jakub Schenk on 28.11.2021.
//

#include "functions.h"
#include "types.h"
#include "parser.h"
#include <stdlib.h>
#include <stdio.h>

struct universe_t universe;

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
};

void set_minus(struct set_t set_a, struct set_t set_b) {
    if (set_a.size == 0 && set_b.size == 0) {
        printf("true");
        return;
    }
    printf("false");
};;

void set_subseteq(struct set_t set_a, struct set_t set_b) {
    if (set_a.size == 0 && set_b.size == 0) {
        printf("true");
        return;
    }
    printf("false");
};;

void set_subset(struct set_t set_a, struct set_t set_b) {
    if (set_a.size == 0 && set_b.size == 0) {
        printf("true");
        return;
    }
    printf("false");
};;

void set_equals(struct set_t set_a, struct set_t set_b) {
    if (set_a.size == 0 && set_b.size == 0) {
        printf("true");
        return;
    }
    printf("false");
};;

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
};

void rel_symmetric(struct relation_t relation) {
    if (relation.size == 0) {
        printf("true");
        return;
    }
    printf("false");
};;

void rel_antisymmetric(struct relation_t relation) {
    if (relation.size == 0) {
        printf("true");
        return;
    }
    printf("false");
};;

void rel_transitive(struct relation_t relation) {
    if (relation.size == 0) {
        printf("true");
        return;
    }
    printf("false");
};;

void rel_function(struct relation_t relation) {
    if (relation.size == 0) {
        printf("true");
        return;
    }
    printf("false");
};;

void rel_domain(struct relation_t relation) {
    if (relation.size == 0) {
        printf("true");
        return;
    }
    printf("false");
};;

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
};

void rel_surjective(struct relation_t relation, struct set_t set_a, struct set_t set_b) {
    if (relation.size == 0 && set_a.size == 0 && set_b.size == 0) {
        printf("true");
        return;
    }
    printf("false");
};;

void rel_bijective(struct relation_t relation, struct set_t set_a, struct set_t set_b) {
    if (relation.size == 0 && set_a.size == 0 && set_b.size == 0) {
        printf("true");
        return;
    }
    printf("false");
};;