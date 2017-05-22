#include <stdio.h>
#include <stdlib.h>

/**
 * Use the Composite pattern when
 * - you want to represent part-whole heirarchies of objects.
 * - you want clients to be able to ignore the difference between compositions
 *   of objects and individual objects. Clients will treat all objects in the
 *   composite structure uniformly.
 */

/* For this example, use a static array. Typically would use an iterator. */
#define MAX_CHILDREN 10

static unsigned compositeId;

/**
 * - defines behavior for components having children.
 * - stores child components.
 * - implements child-related operations in the Component interface.
 */
typedef struct Composite_s {
    void (*operation)(struct Composite_s *self);
    void (*operationAll)(struct Composite_s *self);
    void (*add)(struct Composite_s *self, struct Composite_s *c);
    void (*remove)(struct Composite_s *self, struct Composite_s *c);
    struct Composite_s * (*getChild)(struct Composite_s *self, int index);

    int id;
    char *type;

    struct Composite_s * child[MAX_CHILDREN];
} Composite_t;

static void compositeOperation(Composite_t *this)
{
    printf("%s %d: operation\n", this->type, this->id);
}

static void compositeOperationAll(Composite_t *this)
{
    printf("%s %d: operationAll\n", this->type, this->id);
    for (int k = 0; k < MAX_CHILDREN; k++) {
        if (this->child[k] != NULL) {
            this->child[k]->operation(this->child[k]);
            if (this->child[k]->operationAll) {
                this->child[k]->operationAll(this->child[k]);
            }
        }
    }
}

static void compositeAdd(Composite_t *this, Composite_t *addComposite)
{
    for (int k = 0; k < MAX_CHILDREN; k++) {
        if (this->child[k] == NULL) {
            printf("%s %d: add %s %d\n", this->type, this->id, addComposite->type, addComposite->id);
            this->child[k] = addComposite;
            break;
        }
    }
}

static void compositeRemove(Composite_t *this, Composite_t *removeComposite)
{
    for (int k = 0; k < MAX_CHILDREN; k++) {
        if (this->child[k]->id == removeComposite->id) {
            printf("%s %d: remove %s %d\n", this->type, this->id,
                   this->child[k]->type, this->child[k]->id);
            this->child[k] = NULL;
            break;
        }
    }
}

static Composite_t * compositeGetChild(Composite_t *this, int index)
{
    if (this->child[index] == NULL) {
        printf("%s %d: no child at index %d\n", this->type, this->id, index);
    } else {
        printf("%s %d: get child at index %d (id = %d)\n", this->type, this->id,
               index, this->child[index]->id);
    }

    return this->child[index];
}

Composite_t * newComposite(void)
{
    Composite_t *composite = (Composite_t *) malloc(sizeof(Composite_t));

    composite->operation    = compositeOperation;
    composite->operationAll = compositeOperationAll;
    composite->add          = compositeAdd;
    composite->remove       = compositeRemove;
    composite->getChild     = compositeGetChild;

    composite->id           = compositeId++;
    composite->type         = "composite";

    return composite;
}

/**
 * - represents leaf objects in the composition. A leaf has no children.
 * - defines behavior for primitive objects in the composition.
 */
typedef Composite_t Leaf_t;

Leaf_t * newLeaf(void)
{
    Leaf_t *leaf = (Leaf_t *) malloc(sizeof(Leaf_t));

    leaf->operation     = compositeOperation;;
    leaf->operationAll  = NULL;
    leaf->add           = NULL;
    leaf->remove        = NULL;
    leaf->getChild      = NULL;

    leaf->id            = compositeId++;
    leaf->type          = "leaf";

    return leaf;
}

/**
 * - declare the interface for objects in the composition
 * - implements default behavior for the interface common to all classes
 * - declares an interface for accessing and managing its child components
 */
typedef Composite_t component_t;

/**
 * - manipulates objects in the composition through the component interface
 */
typedef struct client_s {
    component_t *component;
} client_t;

int main(void)
{
    Composite_t *composite0 = newComposite();
    Composite_t *composite1 = newComposite();
    Composite_t *composite2 = newComposite();

    Leaf_t *leaf3 = newLeaf();
    Leaf_t *leaf4 = newLeaf();
    Leaf_t *leaf5 = newLeaf();

    composite0->operation(composite0);
    composite1->operation(composite1);
    composite2->operation(composite2);

    leaf3->operation(leaf3);
    leaf4->operation(leaf4);
    leaf5->operation(leaf5);

    composite0->add(composite0, leaf3);
    composite0->add(composite0, leaf4);

    composite0->remove(composite0, leaf3);
    composite0->add(composite0, leaf3);

    composite0->add(composite0, composite1);
    composite0->add(composite0, composite2);

    composite2->add(composite2, leaf5);

    Composite_t *tmp = composite0->getChild(composite0, 2);
    tmp->operation(tmp);

    composite0->operationAll(composite0);

    free(composite0);
    free(composite1);
    free(composite2);
    free(leaf3);
    free(leaf4);
    free(leaf5);

    return 0;
}
