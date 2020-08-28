#include <stdio.h>
#include <stdlib.h>

/**
 * Intent
 * - Provide a surrogate or placeholder for another object to control access to
 *   it.
 */

/**
 * Use the Proxy pattern when
 * - There is a need for a more versatile or sophisticated reference to an
 *   object than a simple pointer. Several common situations in which the Proxy
 *   pattern is applicable:
 *      1. *remote reference* provides a local representative for an object in
 *         a different address space.
 *      2. *virtual proxy* creates expensive objects on demand.
 *      3. *protection proxy* controls access to the original object. Protection
 *         proxies are useful when objects should have different access rights.
 *      4. *smart reference* is a replacement for a bare point that performs
 *         additional actions when an object is accessed. Typical uses include:
 *          - counting the number of references to the real object so that it
 *            can be freed automatically when there are no more references (also
 *            called 'smart pointers')
 *          - loading a persistent object into memory when it's first
 *            referenced.
 *          - checking that the real object is locked before it's accessed to
 *            ensure that no other object can change it.
 */

typedef struct Subject_s Subject_t;

struct Subject_s {
    struct Subject_s *realRef;

    void (*request)(Subject_t *);
};

void realRequest(Subject_t *subject)
{
    printf("Real request\n");
}

Subject_t * newSubject(void)
{
    Subject_t *s = (Subject_t *) malloc(sizeof(Subject_t));

    s->realRef = s; /* real subjects reference is itself */

    s->request = realRequest;

    return s;
}

void proxyRequest(Subject_t *proxy)
{
    printf("Proxy request\n");

    if (proxy->realRef == NULL) {
        printf("Real subject doesn't exist, created real subject...\n");
        proxy->realRef = newSubject();
    }

    proxy->realRef->request(proxy->realRef);
}

Subject_t * newProxy(Subject_t *realRef)
{
    Subject_t *proxy = (Subject_t *) malloc(sizeof(Subject_t));

    proxy->realRef = realRef;
    proxy->request = proxyRequest;

    return proxy;
}

int main(void)
{
    /*
     * The real object that a proxy represents.
     */
    Subject_t *realSubject = NULL;

    /*
     * The Proxy maintains a reference that lets the proxy access the real
     * subject. It also has an interface identical to realSubject, so that the
     * proxy can be substituted for the real subject.
     *
     * It controls access to the real subject and may be responsible for
     * creating and deleting it.
     */
    Subject_t *proxy = newProxy(realSubject);

    /* At this point, we haven't created any objects... It's not until we use
     * the proxy that we'll create the object */
    printf("FIRST REQUEST\n");
    proxy->request(proxy);

    /* No new objects get created, and we simply do the request again */
    printf("SECOND REQUEST\n");
    proxy->request(proxy);

    return 0;
}
