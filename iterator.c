#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*******************************************************************************
 *    ____________
 *    |           |
 *    | iterator  |
 *    |___________|___________________________
 *          |                                 \
 *          |firstPtr                          |currentPtr (for example)
 *    ______V________                   _______V________
 *    |              |                  |              |
 *    | iteratorItem |-- nextItemPtr -->| iteratorItem |-- nextItemPtr --> ...
 *    |______________|                  |______________|
 *      |          |                      |          |
 *      |          |                      |          |
 *      V          V                      V          V
 *  funcPtr     funcArg                funcPtr    funcArg
 *
 ******************************************************************************/
typedef struct iteratorItem_s {
    void (*funcPtr)();
    void *funcArg;
    struct iteratorItem_s *nextItemPtr;
} iteratorItem_t;

typedef struct Iterator_s {
    unsigned size;
    unsigned index;
    iteratorItem_t *firstPtr;
    iteratorItem_t *currentPtr;
    void (*add)(struct Iterator_s *this, void (*funcPtr)(), void *funcArg);
    void (*drop)(struct Iterator_s *this);
    void (*first)(struct Iterator_s *this);
    void (*next)(struct Iterator_s *this);
    bool (*isDone)(struct Iterator_s *this);
    void (*call)(struct Iterator_s *this);
    void (*traverse)(struct Iterator_s *this);
} Iterator_t;

static void add(Iterator_t *this, void (*funcPtr)(), void *funcArg)
{
    iteratorItem_t *itemPtr = calloc(1, sizeof(*itemPtr));

    if (itemPtr == NULL) {
        return;
    }

    itemPtr->funcPtr = funcPtr;
    itemPtr->funcArg = funcArg;

    if (this->size == 0) {
        if (this->firstPtr != NULL) {
            free(this->firstPtr);
        }
        this->firstPtr = itemPtr;
        this->first(this);
    } else {
        this->first(this);
        while (!this->isDone(this)) {
            this->next(this);
        }
        this->currentPtr->nextItemPtr = itemPtr;
        this->index++;
    }

    printf("Add item %d\n", this->size);
    this->size++;
}

static void drop(Iterator_t *this)
{
    unsigned dropIndex = this->index;
    iteratorItem_t *newNextItemPtr = this->currentPtr->nextItemPtr;
    iteratorItem_t *droppedItemPtr = this->currentPtr;

    if (dropIndex == 0) {
        this->firstPtr = newNextItemPtr;
    } else {
        unsigned k;
        this->first(this);
        for (k = 0; k < dropIndex-1; k++) {
            this->next(this);
        }
        this->currentPtr->nextItemPtr = newNextItemPtr;
    }

    free(droppedItemPtr);

    printf("Dropping index %d\n", dropIndex);
    this->size--;
}

static void first(Iterator_t *this)
{
    this->currentPtr = this->firstPtr;
    this->index = 0;
}

static void next(Iterator_t *this)
{
    if (this->index < this->size-1) {
        this->currentPtr = this->currentPtr->nextItemPtr;
        this->index++;
    }
}

static bool isDone(Iterator_t *this)
{
    return (this->index == this->size - 1);
}

static void call(Iterator_t *this)
{
    printf("Calling item %d: ", this->index);
    this->currentPtr->funcPtr(this->currentPtr->funcArg);
}

static void traverse(Iterator_t *this)
{
    if (this->size != 0) {
        printf("Traversing %d items\n", this->size);
        this->first(this);
        while (!this->isDone(this)) {
            this->call(this);
            this->next(this);
        }
        this->call(this);
    }
    else {
        printf("Nothing to traverse\n");
    }
}

static Iterator_t * newIterator(void)
{
    Iterator_t *it;
    /*
     * Allocate memory from the heap
     */
    it = (Iterator_t *) malloc(sizeof(Iterator_t));

    /*
     * Methods
     */
    it->add = add;
    it->drop = drop;
    it->first = first;
    it->next = next;
    it->isDone = isDone;
    it->call = call;
    it->traverse = traverse;

    /*
     * Data
     */
    it->size = 0;
    it->index = 0;

    return it;
}

static void func0(int x) { printf("->func0(%d)\n", x); }
static void func1(void) { printf("->func1\n"); }
static void func2(void) { printf("->func2\n"); }
static void func3(void) { printf("->func3\n"); }
static void func4(void) { printf("->func4\n"); }

int main(void)
{
    Iterator_t *it;

    it = newIterator();

    /*
     * Examples of using the iterator.
     */
    it->add(it, func0, (void *) 999);
    it->traverse(it);

    it->add(it, func1, NULL);
    it->traverse(it);

    it->add(it, func2, NULL);
    it->add(it, func3, NULL);
    it->add(it, func4, NULL);
    it->traverse(it);
    it->drop(it);
    it->traverse(it);

    printf("Go back to the first item and manually call\n");

    it->first(it);
    it->call(it);
    it->next(it);
    it->call(it);
    it->next(it);
    it->call(it);
    it->drop(it);
    it->traverse(it);

    it->first(it);
    it->drop(it);
    it->traverse(it);

    it->drop(it);
    it->traverse(it);

    it->drop(it);
    it->traverse(it);

    it->add(it, func4, NULL);
    it->add(it, func3, NULL);
    it->add(it, func2, NULL);
    it->traverse(it);

    free(it);

    return 0;
}
