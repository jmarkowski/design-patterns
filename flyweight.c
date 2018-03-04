#include <stdio.h>
#include <stdlib.h>

/**
 * Intent
 * - Use sharing to support large numbers of fine-grained objects efficiently.
 */

/**
 * The Flyweight pattern's effectiveness depends heavily on how and where it's
 * used. Apply the Flyweight pattern when *all* of the following are true:
 * - An application uses a large number of objects.
 * - Storage costs are high because of the sheer quantity of objects.
 * - Most object state can be made extrinsic.
 * - Many groups of objects may be replaced by relatively few shared objects
 *   once extrinsic state is removed.
 * - The application doesn't depend on object identity. Since flyweight objects
 *   may be shared, identity tests will return true for conceptually distinct
 *   objects.
 */

/*
 * The Flyweight declares an interface through which flyweights can receive and
 * act on extrinsic state.
 *
 * Adds storage for intrinsic state, if any.
 */
typedef struct Flyweight_s Flyweight_t;

/*
 * The FlyweightFactory creates and manages flyweight objects, and ensures that
 * flyweights are shared properly. When a client requests a flyweight, the
 * FlyweightFactory object supplies an existing instance or creates one, if none
 * exists.
 */
typedef struct FlyweightFactory_s FlyweightFactory_t;

struct Flyweight_s {
    int key;
    int intState;
    int extState;

    void (*operation)(Flyweight_t *, int extState);
};

void flyweightOperation(Flyweight_t *fw, int extState)
{
    fw->extState = extState;

    /* The intrinsic state differs from the extrinsic state */
    fw->intState = 2*fw->extState;
}

Flyweight_t * newFlyweight(int key)
{
    Flyweight_t *fw = (Flyweight_t *) malloc(sizeof(Flyweight_t));

    fw->key = key;

    fw->operation = flyweightOperation;

    return fw;
}

#define FLYWEIGHT_POOL_SIZE 5
struct FlyweightFactory_s {
    int numFlyweights;
    Flyweight_t *pool[FLYWEIGHT_POOL_SIZE];

    Flyweight_t *(*getFlyweight)(FlyweightFactory_t *, int);
};

Flyweight_t * getFlyweight(FlyweightFactory_t *f, int key)
{
    if (f->pool[key]) {
        /* flyweight exists */
        return f->pool[key];
    } else if (key < FLYWEIGHT_POOL_SIZE) {
        /* flyweight does not yet exist, create it */
        Flyweight_t *fw = newFlyweight(key);
        f->pool[key] = fw;

        f->numFlyweights++;

        return f->pool[key];
    } else {
        return NULL;
    }
}

FlyweightFactory_t * newFlyweightFactory(void)
{
    FlyweightFactory_t *f = (FlyweightFactory_t *) malloc(sizeof(FlyweightFactory_t));

    f->getFlyweight = getFlyweight;

    return f;
}

#define MAX_FLYWEIGHTS 10
int main(void)
{
    FlyweightFactory_t *f = newFlyweightFactory();

    Flyweight_t *fw[MAX_FLYWEIGHTS];

    for (int k = 0; k < MAX_FLYWEIGHTS; k++) {
        int key = k % (MAX_FLYWEIGHTS / 2); /* half as many objects */

        /* In the first half of the loop, flyweights will get created. In the
         * next half of the loop, existing flyweights will be retrieved */
        fw[k] = f->getFlyweight(f, key);

        /*
         * We are going to pass in k as the 'operation' on the flyweight,
         * pretending this is the 'extrinsic state'.
         */
        fw[k]->operation(fw[k], k);
    }

    printf("Flyweights created: %d\n", f->numFlyweights);

    printf("Flyweights in list:\n");

    /*
     * Display all the flyweights that are referenced in the fw array
     */
    for (int k = 0; k < f->numFlyweights; k++) {
        printf("Flyweight[%d]: key=%d, intrinsic state=%d, extrinsic state=%d\n",
               k, fw[k]->key, fw[k]->intState, fw[k]->extState);
    }

    return 0;
}
