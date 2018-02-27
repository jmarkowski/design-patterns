#include <stdio.h>
#include <stdlib.h>

/**
 * Intent
 * - Ensure a class only has one instance, and provide a global point of access
 *   to it.
 */

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
typedef struct Singleton_s {
    int id;

    int (*getInstance)(struct Singleton_s *self);
    void (*operation)(struct Singleton_s *self);
} Singleton_t;

static void singletonOperation(Singleton_t *this)
{
    printf("Singleton %d: operation\n", this->getInstance(this));
}

static int singletonGetInstance(Singleton_t *this) {
    return this->id;
}

/* Private class constructor, should never be callable */
static Singleton_t * newSingleton(void)
{
    static int id;

    Singleton_t *singleton = (Singleton_t *) malloc(sizeof(Singleton_t));

    singleton->getInstance = singletonGetInstance;
    singleton->operation   = singletonOperation;
    singleton->id = id++;

    return singleton;
}

/* Static class method, hence, global */
Singleton_t * singletonInstance(void)
{
    static Singleton_t *instance;

    if (instance == 0) {
        instance = newSingleton();
    }

    return instance;
}

int main(void)
{
    Singleton_t *firstInstance = singletonInstance();

    firstInstance->operation(firstInstance);

    Singleton_t *secondInstance = singletonInstance();

    secondInstance->operation(secondInstance);

    return 0;
}
