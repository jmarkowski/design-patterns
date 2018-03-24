#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Intent
 * - Avoid coupling the sender of a request to its receiver by giving more than
 *   one object a chance to handle the request. Chain the receiving objects and
 *   pass the request along the chain until an object handles it.
 */

/**
 * Use Chain of Responsibility when
 * - More than one object may handle a request, and the handler isn't known
 *   a priori. The handler should be ascertained automatically.
 * - You want to issue a request to one of several objects without specifying
 *   the receiver explicitly.
 * - The set of objects that can handle a request should be specified
 *   dynamically.
 */

/*
 * The Handler handles requests it is responsible for, and it can access its
 * successor. If the concrete handler can handle the request, it does so;
 * otherwise it forwards the request to its successor.
 */
typedef struct Handler_s Handler_t;

struct Handler_s {
    const char *id;
    const Handler_t *successor;
    const char *responsibility;

    void (*handleRequest)(const Handler_t *, const char *);
};

void handlerHandleRequest(const Handler_t *h, const char *request)
{
    if (strcmp(h->responsibility, request) == 0) {
        /* If the request is this handler's responsibility, then handle it */
        printf("Request '%s' handled by '%s'\n", request, h->id);
    } else if (h->successor) {
        /* Else, just pass it off to the successor, if it has one */
        h->successor->handleRequest(h->successor, request);
    } else {
        /* Finally, I have no successor, we can't handle the request! */

        printf("No handler for the request '%s'!\n", request);
    }
}

Handler_t *newHandler(const char *id,
                      const char *responsibility,
                      const Handler_t *successor)
{
    Handler_t *this = (Handler_t *) malloc(sizeof(Handler_t));

    this->id = id;
    this->responsibility = responsibility;
    this->successor = successor;

    this->handleRequest = handlerHandleRequest;

    return this;
}

int main(void)
{
    /* Handler chain: a -> b -> c */
    Handler_t *thirdHandler  = newHandler("Third handler", "ghi", NULL);
    Handler_t *secondHandler = newHandler("Second Handler", "def", thirdHandler);
    Handler_t *firstHandler  = newHandler("First Handler", "abc", secondHandler);

    Handler_t *handler = firstHandler;

    /* Handled by the 'Second Handler' */
    handler->handleRequest(handler, "def");

    /* Handled by the 'Third Handler' */
    handler->handleRequest(handler, "ghi");

    /* Handled by the 'First Handler' */
    handler->handleRequest(handler, "abc");

    /* Handled by no one! */
    handler->handleRequest(handler, "xyz");

    return 0;
}
