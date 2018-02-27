#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* for strncpy */

/**
 * Intent
 * - Allow an object to alter its behaviour when its internal state changes. The
 *   object will appear to change its class.
 */

/**
 * Use the State pattern when
 * - An object's behaviour depends on its state, and it must change its
 *   behaviour at run-time depending on that state.
 * - Operations have large, multipart conditional statements that depend on the
 *   object's state. This state is usually represented by one or more enumerated
 *   constants. Often, several operations will contain this same conditional
 *   structure. The State pattern puts each branch of the conditional in a
 *   separate class. This lets you treat the object's state as an object in its
 *   own right that can var independently from other objects.
 */

#define MAX_STATE_ID 10

/*
 * The state class describes all the possible things that it can do within a
 * given context.
 *
 * However, as the states are changed, we only "activate" particular behaviours.
 */
typedef struct State_s {
    void (*doA)(struct State_s *);
    void (*doB)(struct State_s *);

    char id[MAX_STATE_ID];
} State_t;

State_t *newState(char *id, void (doA)(State_t *),
                            void (doB)(State_t *))
{
    State_t *s = (State_t *) malloc(sizeof(State_t));

    s->doA = doA;
    s->doB = doB;

    strncpy(s->id, id, MAX_STATE_ID);

    return s;
}

void stateX_doA(State_t *s) { printf("X doing A\n"); }
void stateX_doB(State_t *s) { printf("X can't do B!\n"); }

void stateY_doA(State_t *s) { printf("Y can't do A!\n"); }
void stateY_doB(State_t *s) { printf("Y doing B\n"); }

void stateZ_doA(State_t *s) { printf("Z doing A\n"); }
void stateZ_doB(State_t *s) { printf("Z doing B\n"); }

/*
 * Configure context (i.e. whatever it is that is subject to states)
 */
typedef struct Context_s {
    void (*changeState)(struct Context_s *, struct State_s *);

    void (*doA)(struct Context_s *);
    void (*doB)(struct Context_s *);

    State_t *currentState;
} Context_t;

void contextChangeState(Context_t *c, State_t *newState)
{
    State_t *oldState = c->currentState;
    printf("Changing from %s to %s...\n", oldState->id, newState->id);
    c->currentState = newState;
}

void contextDoA(Context_t *c)
{
    if (c->currentState) {
        c->currentState->doA(c->currentState);
    }
}

void contextDoB(Context_t *c)
{
    if (c->currentState) {
        c->currentState->doB(c->currentState);
    }
}

Context_t *newContext(State_t *initialState)
{
    Context_t *c = (Context_t *) malloc(sizeof(Context_t));

    c->changeState = contextChangeState;
    c->doA = contextDoA;
    c->doB = contextDoB;

    c->currentState = initialState;

    return c;
}

int main(void)
{
    /*
     * These are the concrete states
     */
    State_t *stateX = newState("X", stateX_doA, stateX_doB);
    State_t *stateY = newState("Y", stateY_doA, stateY_doB);
    State_t *stateZ = newState("Z", stateZ_doA, stateZ_doB);

    Context_t *c = newContext(stateX); /* initialize at state X */

    /* State X can only do A */
    c->doA(c);
    c->doB(c);

    /* State Y can only do B */
    c->changeState(c, stateY);
    c->doA(c);
    c->doB(c);

    /* State Z can do both A and B */
    c->changeState(c, stateZ);
    c->doA(c);
    c->doB(c);

    /*
     * Above we manually change the state transitions. However, in practice this
     * would likely be handled either inside of Context_t, or perhaps even
     * inside the state class. If it's the latter, the State_t class would need
     * to know about Context_t.
     */

    return 0;
}
