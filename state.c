#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* for strncpy */

/**
 * Intent
 * - Allow an object to alter its behavior when its internal state changes. The
 *   object will appear to change its class.
 */

/**
 * Use the State pattern when
 * - An object's behavior depends on its state, and it must change its
 *   behavior at run-time depending on that state.
 * - Operations have large, multipart conditional statements that depend on the
 *   object's state. This state is usually represented by one or more enumerated
 *   constants. Often, several operations will contain this same conditional
 *   structure. The State pattern puts each branch of the conditional in a
 *   separate class. This lets you treat the object's state as an object in its
 *   own right that can var independently from other objects.
 */

#define MAX_STATE_ID 10

typedef struct State_s State_t;
typedef struct Context_s Context_t;
/*
 * Each state has a set of actions it can do at that state, and for the given
 * context, it can change the state.
 */
struct State_s {
    char id[MAX_STATE_ID];

    void (*changeState)(Context_t *, State_t *);
    State_t *(*action)(Context_t *);
};

/*
 * Configure context (i.e. whatever it is that is subject to states)
 */
struct Context_s {
    State_t *currentState;
    State_t *nextState;
    State_t *stateX;
    State_t *stateY;
    State_t *stateZ;

    void (*changeState)(Context_t *, State_t *);
    void (*begin)(Context_t *);
    void (*end)(Context_t *);
    void (*step)(Context_t *);
};

void stateChangeState(Context_t *c, State_t *s)
{
    State_t *oldState = c->currentState;
    State_t *nextState = s->action(c);
    State_t *newState = s;

    c->currentState = s;
    c->nextState = nextState;

    printf("\tChanged from %s to %s (next is %s)\n",
           oldState->id,
           newState->id,
           nextState->id);

}

State_t *newState(char *id, State_t *(*action)(Context_t *))
{
    State_t *s = (State_t *) malloc(sizeof(State_t));

    strncpy(s->id, id, MAX_STATE_ID);

    s->changeState = stateChangeState;
    s->action = action;

    return s;
}

State_t * actionX(Context_t *c)
{
    printf("Doing action X");
    return c->stateY;
}

State_t * actionY(Context_t *c)
{
    printf("Doing action Y");
    return c->stateZ;
}

State_t * actionZ(Context_t *c)
{
    printf("Doing action Z");
    return c->stateX;
}

void contextChangeState(Context_t *c, State_t *newState)
{
    c->currentState->changeState(c, newState);
}

void contextBegin(Context_t *c)
{
    printf("BEGIN:\t");
    c->changeState(c, c->stateX);
}

void contextStep(Context_t *c)
{
    printf("STEP:\t");
    c->changeState(c, c->nextState);
}

void contextEnd(Context_t *c)
{
    printf("END:\t");
    c->changeState(c, c->stateZ);
}

Context_t *newContext(State_t *stateX,
                      State_t *stateY,
                      State_t *stateZ)
{
    Context_t *c = (Context_t *) malloc(sizeof(Context_t));

    c->currentState = stateX;
    c->stateX = stateX;
    c->stateY = stateY;
    c->stateZ = stateZ;

    c->changeState = contextChangeState;
    c->begin = contextBegin;
    c->step = contextStep;
    c->end = contextEnd;

    return c;
}

int main(void)
{
    /*
     * These are the concrete states
     */
    State_t *stateX = newState("X", actionX);
    State_t *stateY = newState("Y", actionY);
    State_t *stateZ = newState("Z", actionZ);

    Context_t *c = newContext(stateX, stateY, stateZ);

    c->begin(c); /* Start at State X */

    c->step(c);  /* State Y */
    c->step(c);  /* State Z */
    c->step(c);  /* State X */
    c->step(c);  /* State Y */
    c->step(c);  /* State Z */
    c->step(c);  /* State X */

    c->end(c);  /* End at State Z */

    return 0;
}
