#include <stdio.h>
#include <stdlib.h>

/**
 * Intent
 * - Define an interface for creating an object, but let subclasses decide which
 *   class to instantiate. Factory Method lets a class defer instantiation to
 *   subclasses.
 */

/**
 * Use the Factory Method pattern when
 * - A class can't anticipate the class of objects it must create.
 * - A class wants its subclasses to specify the objects it creates.
 * - Classes delegate responsibility to one of several helper subclasses, and
 *   you want to localize the knowledge of which helper subclass is the
 *   delegate.
 */

/*
 * The Product defines the interface of objects the factory method creates, and
 * a concrete instance implements the Product interface.
 */
typedef struct Product_s Product_t;

/*
 * An abstract class of the Creator declares the factory method, which returns
 * an object of type Product, and a concrete version overrides the factory
 * method to return an instance of a concrete Product.
 */
typedef struct Creator_s Creator_t;

struct Product_s {
    void (*operation)(Product_t *);
};

struct Creator_s {
    Product_t *(*factoryMethod)(void);
};

Creator_t * newCreator(Product_t *(*factoryMethod)(void),
                       const char *productStr)
{
    Creator_t *creator = (Creator_t *) malloc(sizeof(Creator_t));

    creator->factoryMethod = factoryMethod;

    printf("Concrete creator set for Product %s\n", productStr);

    return creator;
}

void operationProductA(Product_t *product)
{
    printf("Product A operation\n");
}

Product_t * newProductA(void)
{
    Product_t *product = (Product_t *) malloc(sizeof(Product_t));

    product->operation = operationProductA;

    return product;
}

void operationProductB(Product_t *product)
{
    printf("Product B operation\n");
}

Product_t * newProductB(void)
{
    Product_t *product = (Product_t *) malloc(sizeof(Product_t));

    product->operation = operationProductB;

    return product;
}

int main(void)
{
    Creator_t *creator;
    Product_t *product;

    creator = newCreator(newProductA, "A");
    product = creator->factoryMethod();

    product->operation(product);

    free(creator);
    free(product);

    creator = newCreator(newProductB, "B");
    product = creator->factoryMethod();

    product->operation(product);

    free(creator);
    free(product);

    return 0;
}
