#include <stdio.h>
#include <stdlib.h>
#include <string.h>     /* for strcmp */

/**
 * Use the Abstract Factory pattern when
 * - A system should be independent of how its products are created, composed,
 *   and represented.
 * - A system should be configured with one of multiplie families of products.
 * - A family of related product objects is designed to be used together, and
 *   you need to enforce this constraint.
 * - You want to provide a class library of products, and you want to reveal
 *   just their interfaces, not their implementations.
 */

/*
 * Product-related functions
 */
const char *PRODUCT_A_ID = "ALPHA";
const char *PRODUCT_B_ID = "BETA";

typedef struct Product_s {
    const char *id;
    void (*action)(struct Product_s *product);
} Product_t;

void productAction(Product_t *product)
{
    printf("Product %s\n", product->id);
}

void destroyProduct(Product_t *product)
{
    if (product) {
        free(product);
    }
}

/*
 * Factory-related functions
 */
typedef struct Factory_s {
    Product_t *(*createProduct)(void);
} AbstractFactory_t;

Product_t *createProductA(void)
{
    Product_t *productA = (Product_t *) malloc(sizeof(Product_t));

    productA->id = PRODUCT_A_ID;

    productA->action = productAction;

    return productA;
}

Product_t *createProductB(void)
{
    Product_t *productB = (Product_t *) malloc(sizeof(Product_t));

    productB->id = PRODUCT_B_ID;

    productB->action = productAction;

    return productB;
}

AbstractFactory_t *newProductAFactory(void)
{
    AbstractFactory_t *factory
        = (AbstractFactory_t *) malloc(sizeof(AbstractFactory_t));

    factory->createProduct = createProductA;

    return factory;
}

AbstractFactory_t *newProductBFactory(void)
{
    AbstractFactory_t *factory
        = (AbstractFactory_t *) malloc(sizeof(AbstractFactory_t));

    factory->createProduct = createProductB;

    return factory;
}

/*
 * Passing in the product type is just a hack to select the "concrete subclass"
 * of the factory.
 */
AbstractFactory_t *newFactory(const char *product)
{
    AbstractFactory_t *factory = 0;

    if (strcmp(product, "Product A") == 0) {
        factory = newProductAFactory();
    } else if (strcmp(product, "Product B") == 0) {
        factory = newProductBFactory();
    }

    return factory;
}

void destroyFactory(AbstractFactory_t *factory)
{
    if (factory) {
        free(factory);
    }
}

int main(void)
{
    AbstractFactory_t *factory;
    Product_t *p;

    /*
     * First create Product A
     */
    factory = newFactory("Product A");
    p = factory->createProduct();

    p->action(p);

    destroyProduct(p);
    destroyFactory(factory);

    /*
     * Next create Product B
     */
    factory = newFactory("Product B");
    p = factory->createProduct();

    p->action(p);

    destroyProduct(p);
    destroyFactory(factory);

    return 0;
}
