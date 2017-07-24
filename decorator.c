#include <stdio.h>
#include <stdlib.h>

/**
 * Use the Decorator
 * - to add responsibilities to individual objects dynamically and
 *   transparently, that is, without affecting other objects
 * - for responsibilities taht can be withdrawn.
 * - when extension by subclassing is impractical. Sometimes a large number of
 *   independent extensions are possible and owuld produce an explosion of
 *   subclasses to support every combination. Or a class definition may be hidden
 *   or otherwise unavailable for subclassing.
 *
 * The Decorator pattern may also be referred to as a Wrapper.
 */

typedef struct Component_s {
    void (*operation)(const char * const str);
} Component_t;

static void componentOperation(const char * const str)
{
    printf("%s: operation\n", str);
}

Component_t * newComponent(void)
{
    Component_t *component = (Component_t *) malloc(sizeof(Component_t));

    component->operation = componentOperation;

    return component;
}

static void decoratorAddedOperation(const char * const str)
{
    printf("%s: added operation\n", str);
}

typedef struct Decorator_s {
    /* Methods inherited from Component_t */
    void (*operation)(const char * const str);

    /* New attributes/methods */
    void (*addedOperation)(const char * const str);
} Decorator_t;

Decorator_t * newDecorator(Component_t *component)
{
    Decorator_t *decorator = (Decorator_t *) malloc(sizeof(Decorator_t));

    decorator->operation = component->operation;
    decorator->addedOperation = decoratorAddedOperation;
}

int main(void)
{
    Component_t *component = newComponent();

    component->operation("Component");

    Decorator_t *decoratedComponent = newDecorator(component);

    decoratedComponent->operation("Decorated Component");
    decoratedComponent->addedOperation("Decorated Component");

    return 0;
}
