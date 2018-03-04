#include <stdio.h>
#include <stdlib.h>

/**
 * Intent
 * - Provide a unified interface to a set of interfaces in a subsystem. Facade
 *   defines a higher-level interface that makes the subsystem easier to use.
 */

/**
 * Use the Facade pattern when
 * - You want to provide a simple interface to a complex subsystem. Subsystems
 *   often get more complex as they evolve. Most patterns, when applied, result
 *   in more and smaller classes. This makes the subsystem more reusable and
 *   easier to customize, but it also becomes harder to use for clients that
 *   don't need to customize it. A facade can provide a simple default view of
 *   the subsystem that is good enough for most clients. Only clients needing
 *   more customizability will need to look beyond the facade.
 * - There are many dependencies between clients and the implementation classes
 *   of an abstraction. Introduce a facade to decouple the subsystem from
 *   clients and other subsystems, thereby promoting subsystem independence and
 *   portability.
 * - You want to layer your subsystems. Use a facade to define an entry point to
 *   each subsystem level. If subsystems are dependent, then you can simplify
 *   the dependencies between them by making them communicate with each other
 *   solely through their facades.
 */

/*
 * The Facade knows which subsystem classes are responsible for a request. It
 * delegates client requests to appropriate subsystem objects.
 */
typedef struct Facade_s Facade_t;

/*
 * The Subsystem class implements the subsystem functionality. It handles work
 * assigned by the Facade object, yet it does not have any knowledge of the
 * facade (i.e. they keep no references to it.
 */
typedef struct Subsystem_s Subsystem_t;

struct Facade_s {
    Subsystem_t *squarer;
    Subsystem_t *displayer;

    void (*squareAndDisplay)(Facade_t *, int);
};

struct Subsystem_s {
    int (*subsystemFn)(int arg);
};

void facadeSquareAndDisplay(Facade_t *facade, int num)
{
    int result;

    result = facade->squarer->subsystemFn(num);

    facade->displayer->subsystemFn(result);
}

Facade_t * newFacade(Subsystem_t *squarer, Subsystem_t *displayer)
{
    Facade_t *facade = (Facade_t *) malloc(sizeof(Facade_t));

    facade->squarer = squarer;
    facade->displayer = displayer;

    /*
     * The new reduce common interface.
     */
    facade->squareAndDisplay = facadeSquareAndDisplay;

    return facade;
}

Subsystem_t * newSubsystem(int (*subsystemFn)(int))
{
    Subsystem_t *subsystem = (Subsystem_t *) malloc(sizeof(Subsystem_t));

    subsystem->subsystemFn = subsystemFn;

    return subsystem;
}

int computeOperation(int arg)
{
    printf("Calling square operation\n");
    return arg * arg;
}

int displayOperation(int arg)
{
    printf("Calling display operation\n");
    printf("%d\n", arg);
}

/*
 * The client communicates with the subsystem by sending requests to the Facade.
 */
int main(void)
{
    /*
     * In this example, squarer will not use the displayer subsystem to display
     * the results, instead the facade will handle the high-level operation
     */
    Subsystem_t *squarer = newSubsystem(computeOperation);
    Subsystem_t *displayer = newSubsystem(displayOperation);

    Facade_t *facade = newFacade(squarer, displayer);

    int a = 7;
    printf("%d x %d = ...\n", a, a);

    facade->squareAndDisplay(facade, a);

    return 0;
}
