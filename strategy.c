#include <stdio.h>
#include <stdlib.h>

/**
 * Intent
 * - Define a family of algorithms, encapsulate each one, and make them
 *   interchangeable. Strategy lets the algorithm vary indepndently from clients
 *   that use it.
 */

/**
 * Use the Strategy pattern when
 * - many related classes differ only in their behaviour. Strategies provide a
 *   way to configure a class with one of many behaviours.
 * - you need different variants of an algorithm. For example, you might define
 *   algorithms reflecting different space/time trade-offs. Strategies can be
 *   used when these variants are implemented as a class heirarchy of
 *   algorithms.
 * - an algorithm uses data that clients shouldn't know about. Use the Strategy
 *   pattern to avoid exposing complex, algorithm-specific data structures.
 * - a class defines many behaviours, and these appear as multiple conditional
 *   statements in its operations. Instead of many conditionals, move related
 *   conditional branches into their own Strategy class.
 */

#define MAX_ALGORITHM_NAME_LEN 24

typedef struct Strategy_s {
    /*
     * This is the interface to the algorithm
     */
    unsigned (*algorithm)(unsigned value1, unsigned value2);

    char name[MAX_ALGORITHM_NAME_LEN];
} Strategy_t;

Strategy_t * newStrategy(unsigned (*algorithm)(unsigned, unsigned),
                         const char * const name)
{
    Strategy_t *strategy = (Strategy_t *) malloc(sizeof(Strategy_t));

    strategy->algorithm = algorithm;

    snprintf(strategy->name, MAX_ALGORITHM_NAME_LEN, name);

    return strategy;
}

unsigned algorithmA(unsigned value1, unsigned value2)
{
    return value1 + value2;
}

unsigned algorithmB(unsigned value1, unsigned value2)
{
    return value1 * value2;
}

/*
 * The context is the class that has a strategy. It can be any type of object,
 * as long as it implements a strategy. It also has an interface with which to
 * run the strategy.
 */
typedef struct Context_s {
    Strategy_t *strategy;

    void (*setStrategy)(struct Context_s *self, Strategy_t *strategy);
    void (*runAlgorithm)(struct Context_s *self);
    void (*show)(struct Context_s *self);

    unsigned value;

    char algorithmStr[MAX_ALGORITHM_NAME_LEN];
} Context_t;

void contextSetStrategy(Context_t *this, Strategy_t *strategy)
{
    this->strategy = strategy;

    snprintf(this->algorithmStr, MAX_ALGORITHM_NAME_LEN, strategy->name);

    printf("Set strategy to %s\n", this->algorithmStr);
}

void contextRunAlgorithm(Context_t *this)
{
    this->value = this->strategy->algorithm(this->value, 2);
}

void contextShow(Context_t *this)
{
    printf("Context: Algorithm=%.*s, Value=%u\n",
           MAX_ALGORITHM_NAME_LEN,
           this->algorithmStr, this->value);
}

Context_t *newContext(void)
{
    Context_t *context = (Context_t *) malloc(sizeof(Context_t));

    /*
     * Methods
     */
    context->setStrategy  = contextSetStrategy;
    context->runAlgorithm = contextRunAlgorithm;
    context->show         = contextShow;

    /*
     * Data
     */
    sprintf(context->algorithmStr, "");
    context->value = 0;

    return context;
}

int main(void)
{
    /*
     * These are the concrete strategies
     */
    Strategy_t *strategyA = newStrategy(algorithmA, "add");
    Strategy_t *strategyB = newStrategy(algorithmB, "multiply");

    Context_t *context = newContext();

    /* Add strategy */
    context->setStrategy(context, strategyA);

    context->runAlgorithm(context);
    context->show(context);

    context->runAlgorithm(context);
    context->show(context);

    context->runAlgorithm(context);
    context->show(context);

    /* Multiply strategy */
    context->setStrategy(context, strategyB);

    context->runAlgorithm(context);
    context->show(context);

    context->runAlgorithm(context);
    context->show(context);

    return 0;
}
