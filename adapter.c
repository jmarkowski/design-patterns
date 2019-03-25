#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Intent
 * - Convert the interface of a class into another interface clients expect.
 *   Adapter lets classes work together that couldn't otherwise because of
 *   incompatible interfaces.
 */

/**
 * Use the Adapter pattern when
 * - You want to use an existing class, and its interface does not match the one
 *   you need.
 * - You want to create a reusable class that cooperates with unrelated or
 *   unforeseen classes, that is, classes that don't necessarily have compatible
 *   interfaces.
 * - (object adapter only) You need to use several existing subclasses, but it's
 *   impractical to adapt their interface by subclassing every one. An object
 *   adapter can adapt the interface of its parent class.
 */

typedef struct Adaptee_s Adaptee_t;
typedef struct Adapter_s Adapter_t;

/*
 * The Adaptee is the object that has an incompatible interface that needs
 * adapting for our interface.
 */
#define MAX_STR_LEN 20
struct Adaptee_s {
    char strbuf[MAX_STR_LEN];
    void (*storeText)(Adaptee_t *, const char *);
    void (*printText)(Adaptee_t *);
};

/*
 * The Adapter adapts the interface of the Adaptee for the client
 *
 * Technically, the adapter should be a subclass of a generic Target class that
 * defines the domain-specific interface that a client uses.
 *
 * Let's pretend that the client only understand how to use the 'request'
 * method. In this example, the 'request' method is to display text.
 */
struct Adapter_s {
    Adaptee_t *adaptee;

    void (*displayText)(Adapter_t *, const char *);
};
typedef Adapter_t Target_t;

void adapteeStoreText(Adaptee_t *adaptee, const char *text)
{
    strncpy(adaptee->strbuf, text, MAX_STR_LEN);
}

void adapteePrintText(Adaptee_t *adaptee)
{
    printf("%s\n", adaptee->strbuf);
}

Adaptee_t *newAdaptee(void)
{
    Adaptee_t *adaptee = (Adaptee_t *) malloc(sizeof(Adaptee_t));

    adaptee->storeText = adapteeStoreText;
    adaptee->printText = adapteePrintText;

    return adaptee;
}

void adapterDisplayText(Adapter_t *adapter, const char * text)
{
    Adaptee_t *adaptee = adapter->adaptee;

    printf("text: ");
    adaptee->storeText(adaptee, text);
    adaptee->printText(adaptee);
}

void unadaptedDisplayText(Target_t *target, const char * text)
{
    printf("text: %s\n", text);
}

Target_t *newTarget(void)
{
    Target_t *target = (Target_t *) malloc(sizeof(Target_t));

    target->displayText = unadaptedDisplayText;;
    target->adaptee = NULL;

    return target;
}

Adapter_t *newAdapter(Adaptee_t *adaptee)
{
    Adapter_t *adapter = (Adapter_t *) malloc(sizeof(Adapter_t));

    adapter->displayText = adapterDisplayText;
    adapter->adaptee = adaptee;

    return adapter;
}

int main(void)
{
    Adaptee_t *adaptee = newAdaptee();
    Target_t *unadaptedTarget = (Target_t *) newTarget();
    Target_t *adaptedTarget = (Target_t *) newAdapter(adaptee);

    unadaptedTarget->displayText(unadaptedTarget, "UNADAPTED TARGET");

    /*
     * Pretending that the client only understands how to do a 'request', which
     * is a method the Adaptee doesn't have.
     */
    adaptedTarget->displayText(adaptedTarget, "ADAPTED TARGET");

    return 0;
}
