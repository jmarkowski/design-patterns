#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Intent
 * - Represent an operation to be performed on the elements of an object
 *   structure. Visitor lets you define a new operation without changing the
 *   classes of the elements on which it operates.
 */

/**
 * Use the Visitor pattern when
 * - An object structure contains many classes of objects with differing
 *   interfaces, and you want to perform operations on these objects that depend
 *   on their concrete classes.
 * - Many distinct and unrelated operations need to be performed on objects in
 *   an object structure, and you want to avoid "polluting" their classes with
 *   these operations. Visitor lets you keep related operations together by
 *   defining them in one class. When the object structure is shared by many
 *   applications, use Visitor to put operations in just those applications that
 *   need them.
 * - The classes defining the object structure rarely change, but you often want
 *   to define new operations over the structure. Changing the object structure
 *   classes requires redefining the interface to all visitors, which is
 *   potentially costly. If the object structure classes change often, then it's
 *   probably better to define the operations in those classes.
 */

#define SIZE 10

typedef struct ElementA_s ElementA_t;
typedef struct ElementB_s ElementB_t;
typedef struct Visitor_s Visitor_t;

struct ElementA_s {
    int num[SIZE];
    int (*accept)(ElementA_t *, Visitor_t *);
};

struct ElementB_s {
    char string[SIZE];
    int (*accept)(ElementB_t *, Visitor_t *);
};

struct Visitor_s {
    int (*visitElementA)(Visitor_t *, ElementA_t *);
    int (*visitElementB)(Visitor_t *, ElementB_t *);

    /*
     * This function will change what the visitor does (poor man's subclassing)
     */
    int (*operation)(int *);
};

int acceptVisitorElementA(ElementA_t *elemA, Visitor_t *v)
{
    return v->visitElementA(v, elemA);
}

ElementA_t * newElementA(void)
{
    ElementA_t *elemA = (ElementA_t *) malloc(sizeof(ElementA_t));

    for (int k = 0; k < SIZE; k++) {
        elemA->num[k] = k;
    }

    elemA->accept = acceptVisitorElementA;

    return elemA;
}

int acceptVisitorElementB(ElementB_t *elemB, Visitor_t *v)
{
    return v->visitElementB(v, elemB);
}

ElementB_t * newElementB(void)
{
    ElementB_t *elemB = (ElementB_t *) calloc(SIZE, sizeof(ElementB_t));

    strncpy(elemB->string, "ABC", SIZE);

    elemB->accept = acceptVisitorElementB;

    return elemB;
}

int visitElementA(Visitor_t *v, ElementA_t *elemA)
{
    return v->operation(elemA->num);
}

int visitElementB(Visitor_t *v, ElementB_t *elemB)
{
    int arr[SIZE];

    for (int k = 0; k < SIZE; k++) {
        if (elemB->string[k] != '\0') {
            arr[k] = elemB->string[k] - 64;
        } else {
            arr[k] = 0;
        }
    }

    return v->operation(arr);
}

Visitor_t * newVisitor(int (*operation)(int *arr))
{
    Visitor_t *v = (Visitor_t *) malloc(sizeof(Visitor_t));

    v->visitElementA = visitElementA;
    v->visitElementB = visitElementB;
    v->operation = operation;

    return v;
}

int countEmpty(int *arr)
{
    int numEmpty = 0;

    for (int k = 0; k < SIZE; k++) {
        if (!arr[k]) {
            numEmpty++;
        }
    }

    return numEmpty;
}

int sum(int *arr)
{
    int sum = 0;

    for (int k = 0; k < SIZE; k++) {
        sum += arr[k];
    }

    return sum;
}

int display(int *arr)
{
    for (int k = 0; k < SIZE; k++) {
        printf("%d ", arr[k]);
    }
    printf("\n");
}

int main(void)
{
    int a, b;

    ElementA_t *elemA = newElementA();
    ElementB_t *elemB = newElementB();

    Visitor_t *displayVisitor = newVisitor(display);
    Visitor_t *countEmptyVisitor = newVisitor(countEmpty);
    Visitor_t *sumVisitor = newVisitor(sum);

    printf("The integer contents of ElementA: ");
    elemA->accept(elemA, displayVisitor);
    printf("The integer contents of ElementB: ");
    elemB->accept(elemB, displayVisitor);

    a = elemA->accept(elemA, countEmptyVisitor);
    b = elemB->accept(elemB, countEmptyVisitor);

    printf("The number of \"0\" spots in ElementA is %d\n", a);
    printf("The number of \"0\" spots in ElementB is %d\n", b);

    a = elemA->accept(elemA, sumVisitor);
    b = elemB->accept(elemB, sumVisitor);

    printf("The sum of all elements in ElementA is %d\n", a);
    printf("The sum of all elements in ElementB is %d\n", b);

    return 0;
}
