#include <stdio.h>
#include <stdlib.h>

/**
 * Intent
 * - Without violating encapsulation, capture and externalize an object's
 *   internal state so that the object can be restored to this state later.
 */

/**
 * Use the Memento pattern when
 * - A snapshot of (some portion of) an object's state must be saved so that it
 *   can be restored to that state later, *and*...
 * - A direct interface to obtaining the state would expose implementation
 *   details and break the object's encapsulation.
 */

/*
 * The Memento stores the internal state of the Originator object. It protects
 * against access by objects other than the originator.
 */
typedef struct Memento_s Memento_t;

/*
 * The Originator creates a memento containing a snapshot of its current
 * internal state, and it uses the memento to restore its internal state.
 */
typedef struct Originator_s Originator_t;

/*
 * The Caretake is responsible for the memento's safekeeping and never operates
 * on or examines the contents of the memento.
 */
typedef struct Caretaker_s Caretaker_t;

typedef char * State_t;

struct Memento_s {
    State_t state;

    State_t (*getState)(Memento_t *);
    void (*setState)(Memento_t *, State_t);
};

struct Originator_s {
    State_t state;

    void (*setState)(Originator_t *, State_t);
    Memento_t *(*createMemento)(Originator_t *);
    void (*setMemento)(Originator_t *, Memento_t *);
};

#define MAX_MEMENTO_HISTORY 5
struct Caretaker_s {
    Memento_t *mHistory[MAX_MEMENTO_HISTORY];
    int mIndex;
    Originator_t *originator;

    void (*snapshot)(Caretaker_t *);
    void (*rollback)(Caretaker_t *);
};

State_t mGetState(Memento_t *m)
{
    return m->state;
}

void mSetState(Memento_t *m, State_t s)
{
    m->state = s;
}

Memento_t * newMemento(void)
{
    Memento_t *m = (Memento_t *) malloc(sizeof(Memento_t));

    m->getState = mGetState;
    m->setState = mSetState;

    return m;
}

void oSetState(Originator_t *o, State_t state)
{
    o->state = state;

    printf("ORIGINATOR state: \"%s\"\n", o->state);
}

Memento_t * oCreateMemento(Originator_t *o)
{
    Memento_t *m = newMemento();

    printf("Creating MEMENTO for \"%s\"\n", o->state);

    /* save the state to the memento */
    m->setState(m, o->state);

    return m;
}

void oSetMemento(Originator_t *o, Memento_t *m)
{
    State_t state = m->getState(m);

    printf("Restoring MEMENTO for \"%s\"\n", state);

    /* restore the state from the memento */
    o->setState(o, state);
}

Originator_t * newOriginator(void)
{
    Originator_t *o = (Originator_t *) malloc(sizeof(Originator_t));

    o->state = NULL;

    o->setState = oSetState;

    o->createMemento = oCreateMemento;
    o->setMemento = oSetMemento;

    return o;
}

void cSnapshot(Caretaker_t *c)
{
    printf("CARETAKER snapshot: ");

    Memento_t *m = c->originator->createMemento(c->originator);

    c->mHistory[++c->mIndex] = m;
}

void cRollback(Caretaker_t *c)
{
    printf("CARETAKER rollback: ");

    Memento_t *m = c->mHistory[--c->mIndex];

    c->originator->setMemento(c->originator, m);
}

Caretaker_t * newCaretaker(Originator_t *o)
{
    Caretaker_t *c = (Caretaker_t *) malloc(sizeof(Caretaker_t));

    c->originator = o;
    c->mIndex = 0;

    c->snapshot = cSnapshot;
    c->rollback = cRollback;

    return c;
}

int main(void)
{
    Originator_t *o = newOriginator();
    Caretaker_t *c = newCaretaker(o);

    o->setState(o, "1st state");
    c->snapshot(c);

    o->setState(o, "2nd state");
    c->snapshot(c);

    o->setState(o, "3rd state");
    c->snapshot(c);

    c->rollback(c); /* expect 2nd state */
    c->rollback(c); /* expect 1st state */

    o->setState(o, "4th state");
    c->snapshot(c);

    o->setState(o, "5th state");
    printf("NO SNAPSHOT\n");

    c->rollback(c); /* expect 1st state */

    return 0;
}
