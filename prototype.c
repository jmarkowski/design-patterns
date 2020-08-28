#include <stdio.h>
#include <stdlib.h>

/**
 * Intent
 * - Specify the kinds of objects to create using a prototypical instance, and
 *   create new objects by copying this prototype.
 */

/**
 * Use the Prototype pattern when a system should be independent of how its
 * products are created, composed, and represented; and
 * - When the classes to instantiate are specified at run-time, for example, by
 *   dynamic loading; or
 * - To avoid building a class heirarchy of factories that parallels the class
 *   hierarchy of products; or
 * - When instances of a class can have one of only a few different combinations
 *   of state. It may be more convenient to install a corresponding number of
 *   prototypes and clone them rather than instantiating the class manually,
 *   each time with the appropriate state.
 */

/*
 * The Prototype declares an interface for cloning itself.
 */
typedef struct Prototype_s Prototype_t;

struct Prototype_s {
    Prototype_t *self;

    /*
     * Cloning will actually return a copy of itself
     */
    Prototype_t *(*clone)(Prototype_t *);

    void (*operation)(Prototype_t *);
};

Prototype_t *newPrototype(void (*o)(Prototype_t *));

Prototype_t *prototypeClone(Prototype_t *original)
{
    Prototype_t *clonedPrototype = newPrototype(original->operation);

    return clonedPrototype;
}

Prototype_t *newPrototype(void (*o)(Prototype_t *))
{
    Prototype_t *p = (Prototype_t *) malloc(sizeof(Prototype_t));

    p->self = p;

    p->clone = prototypeClone;
    p->operation = o;

    return p;
}

void operation(Prototype_t *prototype)
{
    Prototype_t *p = prototype->clone(prototype);

    p->operation(p);
}

void op1(Prototype_t *p) {
    printf("Prototype 1 (address = %x) operation\n", p);
}

void op2(Prototype_t *p) {
    printf("Prototype 2 (address = %x) operation\n", p);
}

int main(void)
{
    /*
     * We have to concrete prototypes (p1 and p2) that do different things.
     * These are the tools that we have available to do various operations.
     */
    Prototype_t *p1 = newPrototype(op1);
    Prototype_t *p2 = newPrototype(op2);

    /*
     * Let's do some work with concrete prototype p1, followed by p2, notice
     * that the operation only cares that it's working on a prototype type.
     */
    operation(p1);
    operation(p2);
    operation(p2);
    operation(p2);

    printf("Notice that the address for p2 is different each time\n");

    return 0;
}
