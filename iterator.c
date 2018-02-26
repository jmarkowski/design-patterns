#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * Intent
 * - Provide a way to access the elements of an aggregate object sequentially
 *   without exposing its underlying representation.
 */

/**
 * Use the Iterator pattern
 * - to access an aggregate object's contents without exposing its internal
 *   representation.
 * - to support multiple traversals of aggregate objects.
 * - to provide a uniform interface for traversing different aggregate
 *   structures (that is, to support polymorphic iteration)
 */

#define MAX_LIST_SIZE 10
typedef struct List_s {
    int el[MAX_LIST_SIZE];
    int count;

    void (*append)(struct List_s *, int);
    void (*remove)(struct List_s *, int);

    /*
     * In order for an aggregate class to support iteration, it must have the
     * ability to create an interator.
     */
    struct Iterator_s * (*createIterator)(struct List_s *);
} List_t;

/*
 * In this example, we have an aggregate object (e.g. List), but we don't want
 * to put the iteration mechanics in its class. Instead, we create a
 * ListIterator class that would perform the traversal for a list object. That
 * way, we don't need to modify the list object.
 */
typedef struct Iterator_s {
    int current;
    struct List_s *list;

    void (*first)(struct Iterator_s *);
    void (*next)(struct Iterator_s *);
    bool (*isDone)(struct Iterator_s *);
    int (*currentItem)(struct Iterator_s *);
} Iterator_t;

void iteratorFirst(Iterator_t *iter)
{
    iter->current = 0;
}

void iteratorNext(Iterator_t *iter)
{
    iter->current++;
}

bool iteratorIsDone(Iterator_t *iter)
{
    return iter->current >= iter->list->count;
}

int iteratorCurrentItem(Iterator_t *iter)
{
    if (!iter->isDone(iter)) {
        return iter->list->el[iter->current];
    } else {
        /* No exception handling. Simply denote -1 as out of bounds. */
        return -1;
    }
}

Iterator_t * newIterator(struct List_s *list)
{
    Iterator_t *iter = (Iterator_t *) malloc(sizeof(Iterator_t));

    iter->current = 0;
    iter->list = list;

    iter->first = iteratorFirst;
    iter->next = iteratorNext;
    iter->isDone = iteratorIsDone;
    iter->currentItem = iteratorCurrentItem;

    return iter;
}

/*
 * This is the aggregate class. In this example, it's a simple array list.
 *
 * The responsibilities of this class are simple. You may simply add an element
 * into it, remove an element, and count the number of elements. There is no
 * "traversal" or "iterating" mechanism built in. For that, we will create a
 * ListIterator subclass.
 */
typedef struct ListIterator_s {
    List_t *list;
    int current;

    void (*first)(struct Iterator_s *);
    void (*next)(struct Iterator_s *);
    bool (*isDone)(struct Iterator_s *);
    void *(*currentItem)(struct Iterator_s *);
} ListIterator_t;

void listAppend(List_t * list, int el)
{
    list->el[list->count++] = el;
}

void listRemove(List_t * list, int el)
{
    /* Remove the first instance of 'el' */
    for (int k = 0; k < list->count; k++) {
        if (list->el[k] == el) {
            /* Move the remaining elements 'back' */
            int j;
            for (j = 0; j < MAX_LIST_SIZE - 1; j++) {
                list->el[k+j] = list->el[k+j+1];
            }
            list->el[j] = 0;
            list->count--;

            break;
        }
    }
}

List_t * newList(void)
{
    List_t *list = (List_t *) malloc(sizeof(List_t));

    list->count = 0;
    list->append = listAppend;
    list->remove = listRemove;
    list->createIterator = newIterator;

    return list;
}

int main(void)
{
    List_t *list = newList();

    list->append(list, 0);
    list->append(list, 1);
    list->append(list, 2);
    list->append(list, 3);

    list->remove(list, 0);

    Iterator_t *listIterator = list->createIterator(list);

    /* Print all the list contents */
    int k = 0;
    for (listIterator->first(listIterator);
        !listIterator->isDone(listIterator);
         listIterator->next(listIterator)) {
        printf("list[%d] is '%d'\n", k++,
               listIterator->currentItem(listIterator));
    }

    return 0;
}
