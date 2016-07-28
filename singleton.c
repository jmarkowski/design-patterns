#include <stdio.h>
#include <stdlib.h>

/**
 * Use the Singleton pattern when
 * - there must be exactly one instance of a class, and it must be accessible
 *   to clients from a well-known access point.
 * - when the sole instance should be extensible by subclassing, and clients
 *   should be able to use an extended instance without modifying their code.
 */

/**
 * - defines an Instance operation that lets clients access its unique instance.
 * - may be responsible for creating its own unique instance.
 */
typedef struct singleton_s {
    int id;

    int (*getInstance)(struct singleton_s *self);
    void (*operation)(struct singleton_s *self);
} singleton_t;

static void singletonOperation(singleton_t *this)
{
    printf("Singleton %d: operation\n", this->getInstance(this));
}

static int singletonGetInstance(singleton_t *this) {
    return this->id;
}

/* Private class constructor, should never be callable */
static singleton_t * newSingleton(void)
{
    static int id;

    singleton_t *singleton = (singleton_t *) malloc(sizeof(singleton_t));

    singleton->getInstance = singletonGetInstance;
    singleton->operation   = singletonOperation;
    singleton->id = id++;

    return singleton;
}

/* Static class method, hence, global */
singleton_t * singletonInstance(void)
{
    static singleton_t *instance;

    if (instance == 0) {
        instance = newSingleton();
    }

    return instance;
}

int main(void)
{
    singleton_t *firstInstance = singletonInstance();

    firstInstance->operation(firstInstance);

    singleton_t *secondInstance = singletonInstance();

    secondInstance->operation(secondInstance);

    return 0;
}
