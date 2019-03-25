#include <stdio.h>
#include <stdlib.h>

/**
 * Intent
 * - Define the skeleton of an algorithm in an operation, deferring some steps
 *   to subclasses. Template Method lets subclasses redefine certain steps of an
 *   algorithm without changing the algorithm's structure.
 */

/**
 * Use the Template Method pattern
 * - To implement the invariant parts of an algorithm once and leave it up to
 *   subclasses to implement the behavior that can vary.
 * - When common behavior among subclasses should be factored and localized in
 *   a common class to avoid code duplication. This is a good example of
 *   "refactoring to generalize". You first identify the differences in the
 *   existing code and then separate the differences into new operations.
 *   Finally, you replace the differing code with a template method that calls
 *   one of these new operations.
 * - To control subclasses extensions. You can define a template method that
 *   calls "hook" operations at specific points, thereby permitting extensions
 *   only at those points.
 */

/*
 * An abstract instance of this class defines the abstract primitive operations
 * that concrete subclasses define to implement steps of an algorithm. It
 * implements a template method defining the skeleton of an algorithm, and the
 * template method in turn calls primitive operations.
 *
 * A concrete instance of this class implements the primitive operations to
 * carry out subclass-specific steps of the algorithm.
 */
typedef struct Class_s Class_t;

struct Class_s {
    void (*templateMethod)(Class_t *);

    void (*operation1)(void);
    void (*operation2)(void);
};

Class_t * newClass(void (*templateMethod)(Class_t *),
                   void (*operation1)(void),
                   void (*operation2)(void))
{
    Class_t *c = (Class_t *) malloc(sizeof(Class_t));

    c->templateMethod = templateMethod;
    c->operation1 = operation1;
    c->operation2 = operation2;

    return c;
}

void templateMethod(Class_t *c)
{
    printf("Running template method: \n");
    c->operation1();
    c->operation2();
}

void a1(void) { printf("Primitive Operation A1\n"); }
void a2(void) { printf("Primitive Operation A2\n"); }
void b1(void) { printf("Primitive Operation B1\n"); }
void b2(void) { printf("Primitive Operation B2\n"); }

int main(void)
{
    Class_t *abstractClass = newClass(templateMethod, NULL, NULL);

    Class_t *concreteClassA = newClass(NULL, a1, a2);
    Class_t *concreteClassB = newClass(NULL, b1, b2);

    abstractClass->templateMethod(concreteClassA);

    abstractClass->templateMethod(concreteClassB);

    return 0;
}
