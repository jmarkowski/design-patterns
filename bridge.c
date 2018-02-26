#include <stdio.h>
#include <stdlib.h>

/**
 * Intent
 * - Decouple an abstraction from its implementation so that the two can vary
 *   independently.
 */

/**
 * Use the Bridge pattern when
 * - You want to avoid a permanent binding between an abstraction and its
 *   implementation. This might be the case, for example, when the
 *   implementation must be selected or switched at run-time.
 * - Both the abstractions and their implementations should be extensible by
 *   subclassing. In this case, the Bridge pattern lets you combine the
 *   different abstractions and implementations and extend them independently.
 * - Changes in the implmentation of an abstraction should have no impact on its
 *   clients; that is, their code should not have to be recompiled.
 */

void impAOperation(void)
{
    printf("Implementation A's concrete operation\n");
}

void impBOperation(void)
{
    printf("Implementation B's concrete operation\n");
}

/*
 * Implementor
 */
typedef struct Implementor_s {
    void (*operation)(void);
    void (*setImplementation)(struct Implementor_s *imp,
                              struct Implementor_s *concreteImp);
} Implementor_t;

void setImplementation(Implementor_t *this, Implementor_t *concreteImp)
{
    this->operation = concreteImp->operation;
}

Implementor_t *newImplementor(void (*operation)(void))
{
    Implementor_t *imp = (Implementor_t *) malloc(sizeof(Implementor_t));

    imp->operation = operation;
    imp->setImplementation = setImplementation;

    return imp;
}

/*
 * Abstraction
 */
typedef struct Abstraction_s {
    Implementor_t *imp;
    void (*operation)(struct Abstraction_s *abs);
} Abstraction_t;

/*
 * Notice that the abstraction does not care about how the operation gets
 * implemented.
 */
void absOperation(Abstraction_t *abs)
{
    Implementor_t *imp = abs->imp;

    /*
     * Here it's goingt to call the implementation specific operation.
     */
    imp->operation();
}

Abstraction_t *newAbstraction(Implementor_t *imp)
{
    Abstraction_t *abs
        = (Abstraction_t *) malloc(sizeof(Abstraction_t));

    /*
     * THIS IS THE BRIDGE
     *
     * The abstraction only has a reference to the implementation. When it calls
     * particular functions, it doesn't know the implementation specific
     * details.
     */
    abs->imp = imp;
    abs->operation = absOperation;

    return abs;
}

int main(void)
{
    Implementor_t *imp = newImplementor(NULL);
    Abstraction_t *abs = newAbstraction(imp);

    Implementor_t *concreteImpA = newImplementor(impAOperation);
    Implementor_t *concreteImpB = newImplementor(impBOperation);

    imp->setImplementation(imp, concreteImpA);
    abs->operation(abs);

    imp->setImplementation(imp, concreteImpB);
    abs->operation(abs);

    return 0;
}
