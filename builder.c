#include <stdio.h>
#include <stdlib.h>

/**
 * Intent
 * - Separate the construction of a complex object from its representation so
 *   that the same construction process can create different representations.
 */

/**
 * Use the Builder pattern when
 * - The algorithm for creating a complex object should be independent of the
 *   parts that make up the object and how they're assembled.
 * - The construction process must allow different representations for the
 *   object that's constructed.
 */

/*
 * The Director constructs an object using the Builder interface
 */
typedef struct Director_s Director_t;

/*
 * This is typically an abstract interface that specifies how to create parts of
 * a Product object.
 */
typedef struct Builder_s Builder_t;

typedef char * Product_t;

struct Builder_s {
    Product_t (*buildPartA)(Builder_t *);
    Product_t (*buildPartB)(Builder_t *);
    Product_t (*buildPartC)(Builder_t *);
};

struct Director_s {
    Builder_t *builder;
    Product_t partA;
    Product_t partB;
    Product_t partC;

    void (*construct)(Director_t *);
};

Product_t buildPartA(Builder_t *b) { return "PART A"; }
Product_t buildPartB(Builder_t *b) { return "PART B"; }
Product_t buildPartC(Builder_t *b) { return "PART C"; }

Builder_t * newBuilder(void)
{
    Builder_t *b = (Builder_t *) malloc(sizeof(Builder_t));

    b->buildPartA = buildPartA;
    b->buildPartB = buildPartB;
    b->buildPartC = buildPartC;

    return b;
}

void directorConstruct(Director_t *d)
{
    printf("DIRECTOR CONSTRUCTION\n");

    d->partA = d->builder->buildPartA(d->builder);
    d->partB = d->builder->buildPartB(d->builder);
    d->partC = d->builder->buildPartC(d->builder);
}

Director_t * newDirector(Builder_t *b)
{
    Director_t *d = (Director_t *) malloc(sizeof(Director_t));

    d->builder = b;
    d->partA = NULL;
    d->partB = NULL;
    d->partC = NULL;

    d->construct = directorConstruct;

    return d;
}

int main(void)
{
    Builder_t *builder = newBuilder();

    /*
     * When the director constructs, it follows a generic recipe of building all
     * the parts. All the details on how to build each part should be captured
     * in the concrete builder class.
     */
    Director_t *director = newDirector(builder);

    director->construct(director);

    printf("Product A from Builder: \"%s\"\n", director->partA);
    printf("Product B from Builder: \"%s\"\n", director->partB);
    printf("Product C from Builder: \"%s\"\n", director->partC);

    return 0;

}
