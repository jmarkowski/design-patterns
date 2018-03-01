#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Intent
 * - Define a one-to-many dependency between objects so that when one object
 *   changes state, all its dependents are notified and updated automatically.
 */

/**
 * Use the Observer pattern when
 * - An abstraction has two aspects, one dependent on the other. Encapsulating
 *   these aspects in separate objects lets you vary and reuse them
 *   independently.
 * - A change to one object requires changing others, and you don't know how
 *   many objects need to be changed.
 * - An object should be able to notify other objects without making assumptions
 *   about who these objects are. In other words, you don't want these objects
 *   tightly coupled.
 */

typedef enum {
    EVENT_1 = 1,
    EVENT_2,
    EVENT_3,
} Event_t;

typedef struct Subject_s Subject_t;
typedef struct Observer_s Observer_t;

#define MAX_OBSERVERS 3
struct Subject_s {
    Observer_t *obs[MAX_OBSERVERS];

    void (*attach)(Subject_t *, Observer_t *);
    void (*detach)(Subject_t *, Observer_t *);
    void (*notify)(Subject_t *, Event_t e);
};

#define MAX_OBSERVER_ID_LEN 10
struct Observer_s {
    char id[MAX_OBSERVER_ID_LEN];
    Event_t myEvent;

    void (*update)(Observer_t *, Event_t e);
};

void subjectAttach(Subject_t *subject, Observer_t *obs)
{
    for (int k = 0; k < MAX_OBSERVERS; k++) {
        if (subject->obs[k] == NULL) {
            printf("\tSUBJECT attaching OBSERVER_%s\n", obs->id);

            subject->obs[k] = obs;
            break;
        }
    }
}

void subjectDetach(Subject_t *subject, Observer_t *obs)
{
    for (int k = 0; k < MAX_OBSERVERS; k++) {
        if (subject->obs[k] == obs) {
            printf("\tSUBJECT detaching OBSERVER_%s\n", obs->id);

            subject->obs[k] = NULL;
            break;
        }
    }
}

void subjectNotify(Subject_t *subject, Event_t e)
{
    printf("\tSUBJECT notifying EVENT %u\n", (unsigned) e);

    for (int k = 0; k < MAX_OBSERVERS; k++) {
        Observer_t *obs = subject->obs[k];

        if (obs) {
            obs->update(obs, e);
        }
    }
}

Subject_t * newSubject(void)
{
    Subject_t *subject = (Subject_t *) malloc(sizeof(Subject_t));

    for (int k = 0; k < MAX_OBSERVERS; k++) {
        subject->obs[k] = NULL;
    }

    subject->attach = subjectAttach;
    subject->detach = subjectDetach;
    subject->notify = subjectNotify;

    return subject;
}

void observerUpdate(Observer_t *obs, Event_t e)
{
    /* This gets called from the subject */
    if (obs->myEvent == e) {
        printf("\t\tOBSERVER_%s has received the update for EVENT %d\n",
               obs->id, e);
    }
}

Observer_t * newObserver(const char *id, Subject_t *subject, Event_t e)
{
    Observer_t *observer = (Observer_t *) malloc(sizeof(Observer_t));

    strncpy(observer->id, id, MAX_OBSERVER_ID_LEN);
    observer->myEvent = e;

    observer->update = observerUpdate;

    subject->attach(subject, observer);

    return observer;
}

int main(void)
{
    /* This is the subject of interest to the observers */
    Subject_t *subject = newSubject();

    Observer_t *observerA = newObserver("A", subject, EVENT_1);
    Observer_t *observerB = newObserver("B", subject, EVENT_2);

    printf("Expecting OBSERVER_A to be notified for EVENT_1 (twice)\n");
    printf("Expecting OBSERVER_B to be notified for EVENT_2\n");

    subject->notify(subject, EVENT_1);
    subject->notify(subject, EVENT_1);
    subject->notify(subject, EVENT_2);

    subject->detach(subject, observerA);

    printf("Expecting OBSERVER_B to be notified for EVENT_2\n");
    subject->notify(subject, EVENT_1);
    subject->notify(subject, EVENT_2);

    return 0;
}
