//
// Created by Jakub Schenk on 28.11.2021.
//

#include "functions.h"
#include "types.h"
#include <stdio.h>

void set_empty(struct set_t set) {
    if (set.size == 0) {
        printf("true");
        return;
    }
    printf("false");
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

void rel_reflexive(struct relation_t relation) {
    if (relation.size == 0) {
        printf("true");
        return;
    }
    printf("false");
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