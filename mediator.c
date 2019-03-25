#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Intent
 * - Define an object that encapsulates how a set of objects interact. Mediator
 *   promotes loose coupling by keeping objects from referring to each other
 *   explicitly, and it lets you vary their interaction independently.
 *
 *   The Mediator encapsulates collective behavior. It is responsible for
 *   controlling and coordinating the interactions of a group of objects. These
 *   objects only know the mediator, thus reducing the number of
 *   interconnections.
 */

/**
 * Use the Mediator when
 * - A set of objects communicate in well-defined but complex ways. The
 *   resulting interdependencies are unstructured and difficult to understand.
 * - Reusing an object is difficult because it refers to and communicates with
 *   many other objects.
 * - A behavior that's distributed between several classes should be
 *   customizable without a lot of subclassing.
 */

/*
 * Mediator defines an interface for communicating with Colleague objects
 */
typedef struct Mediator_s Mediator_t;

/*
 * Colleague knows its Mediator object and communicates with its mediator
 * whenever it would have otherwise communicated with another Colleague.
 *
 * They send and receive requests from the Mediator.
 */
typedef struct Colleague_s Colleague_t;

#define MAX_COLLEAGUES 3
struct Mediator_s {
    Colleague_t *colleagues[MAX_COLLEAGUES];

    void (*addColleague)(Mediator_t *, Colleague_t *);
    void (*colleagueUpdate)(Mediator_t *, Colleague_t *);
};

#define MAX_ID_LEN 10
struct Colleague_s {
    char id[MAX_ID_LEN];
    Mediator_t *mediator; /* gets notified of a change */

    void (*update)(Colleague_t *c);
};

void mediatorAddColleague(Mediator_t *m, Colleague_t *c)
{
    for (int k = 0; k < MAX_COLLEAGUES; k++) {
        if (m->colleagues[k] == NULL) {
            m->colleagues[k] = c;

            printf("COLLEAGUE %s added to MEDIATOR\n", c->id);
            break;
        }
    }
}

void mediatorColleagueUpdate(Mediator_t *m, Colleague_t *c)
{
    printf("MEDIATOR received update from COLLEAGUE %s\n");

    /* Update all remaining colleagues... */
    for (int k = 0; k < MAX_COLLEAGUES; k++) {
        if (c == m->colleagues[k]) {
            for (int j = k + 1; j < MAX_COLLEAGUES; j++) {
                m->colleagues[j]->update(m->colleagues[j]);
                break;
            }
        }
    }
}

Mediator_t * newMediator(void)
{
    Mediator_t *m = (Mediator_t *) malloc(sizeof(Mediator_t));

    for (int k = 0; k < MAX_COLLEAGUES; k++) {
        m->colleagues[k] = NULL;
    }

    m->addColleague = mediatorAddColleague;
    m->colleagueUpdate = mediatorColleagueUpdate;

    return m;
}

void colleagueUpdate(Colleague_t *c)
{
    printf("COLLEAGUE %s updated\n", c->id);

    c->mediator->colleagueUpdate(c->mediator, c);
}

Colleague_t * newColleague(const char *id, Mediator_t *m)
{
    Colleague_t *c = (Colleague_t *) malloc(sizeof(Colleague_t));

    strncpy(c->id, id, MAX_ID_LEN);
    c->mediator = m;

    c->update = colleagueUpdate;

    m->addColleague(m, c);

    return c;
}

int main(void)
{
    Mediator_t *m = newMediator();

    Colleague_t *c1 = newColleague("C1", m);
    Colleague_t *c2 = newColleague("C2", m);
    Colleague_t *c3 = newColleague("C3", m);

    /*
     * When C1 updates --> C2 will update
     * When C2 updates --> C3 will update
     */
    printf("\nExpecting C3 to update ONLY\n");
    c3->update(c3);

    printf("\nExpecting C1, C2, and C3 to update\n");
    c1->update(c1);

    return 0;
}
