#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * Intent
 * - Given a language, define a representation for its grammar along with an
 *   interpreter that uses the representation to interpret sentences in the
 *   language.
 */

/**
 * Use the Interpreter pattern when there is a language to interpret, and you
 * can represent statements in the language as abstract syntax trees. The
 * interpreter pattern works best when
 * - The grammar is simple. For complex grammers, the class heirarchy for the
 *   grammar becomes large and unmanageable. Tools such as parser generators are
 *   a better alternative in such cases. They can interpret expressions without
 *   building abstract syntax trees, which can save space and possibly time.
 * - Efficiency is not a critical concern. The most efficient interpreters are
 *   usually *not* implemented by interpreting parse trees directly but by first
 *   translating them into another form. For example, regular expressions are
 *   often transformed into state machines. But even then, the *translator* can
 *   be implemented by the Interpreter pattern, so the pattern is still
 *   applicable.
 */

/*
 * The expression is an abstract class that declares an abstract Interpret
 * operation that is common to all nodes in the abstract syntax tree.
 */

/*
 * The concrete TerminalExpression class implements an Interpret operation
 * associated with terminal symbols in the grammar.
 */
typedef struct TerminalExpression_s TerminalExpression_t;

/*
 * The concrete NonterminalExpression class implements an Interpret operation
 * for nonterminal symbols in the grammar. Interpret calls itself recursively
 * on the variables representing the rules (R1 .. Rn).
 *
 * One class of this is required for every rule R.
 */
typedef struct NonterminalExpression_s NonterminalExpression_t;

/*
 * Contains information that is global to the interpreter
 */
typedef struct Context_s Context_t;

/*
 * The AbstractSyntaxTree is usually built up from the client, which is
 * assembled by instances of TerminalExpression and NonterminalExpression.
 */
typedef struct AbstractSyntaxTree_s AbstractSyntaxTree_t;

struct TerminalExpression_s {
    void (*interpret)(TerminalExpression_t *, Context_t *);
};

struct NonterminalExpression_s {
    void (*interpret)(NonterminalExpression_t *, Context_t *);
};

struct Context_s {
    bool upper;
};

void teInterpret(TerminalExpression_t *te, Context_t *c)
{
    if (c->upper) {
        printf("TERMINAL EXPRESSION\n");
    } else {
        printf("terminal expression\n");
    }
}

TerminalExpression_t *newTerminalExpression(void)
{
    TerminalExpression_t *te
        = (TerminalExpression_t *) malloc(sizeof(TerminalExpression_t));

    te->interpret = teInterpret;

    return te;
}

void nteInterpret(NonterminalExpression_t *nte, Context_t *c)
{
    if (c->upper) {
        printf("NONTERMINAL EXPRESSION\n");
    } else {
        printf("nonterminal expression\n");
    }
}

NonterminalExpression_t *newNonterminalExpression(void)
{
    NonterminalExpression_t *nte
        = (NonterminalExpression_t *) malloc(sizeof(NonterminalExpression_t));

    nte->interpret = nteInterpret;

    return nte;
}

typedef enum {
    TERMINAL,
    NONTERMINAL,
} Expression_t;

#define MAX_EXPRESSIONS 4
struct AbstractSyntaxTree_s {
    void *expression[MAX_EXPRESSIONS];
    Expression_t type[MAX_EXPRESSIONS];
    int numExpressions;

    void (*add)(AbstractSyntaxTree_t *abs, Expression_t type, void *e);
};

void absAdd(AbstractSyntaxTree_t *abs, Expression_t type, void *te)
{
    abs->expression[abs->numExpressions] = (void *) te;
    abs->type[abs->numExpressions] = type;

    abs->numExpressions++;
}

AbstractSyntaxTree_t *newAbstractSyntaxTree(void)
{
    AbstractSyntaxTree_t *abs
        = (AbstractSyntaxTree_t *) malloc(sizeof(AbstractSyntaxTree_t));

    abs->numExpressions = 0;

    abs->add = absAdd;

    return abs;
}

void interpret(Expression_t type, void *expression, Context_t *context)
{
    TerminalExpression_t *te;
    NonterminalExpression_t *nte;

    switch (type) {
    case TERMINAL:
        te = (TerminalExpression_t *) expression;
        te->interpret(te, context);
        break;
    case NONTERMINAL:
        nte = (NonterminalExpression_t *) expression;
        nte->interpret(nte, context);
        break;
    }
}

int main(void)
{
    /* Usually a tree, but for simplicity it behaves as a list */
    AbstractSyntaxTree_t *abs = newAbstractSyntaxTree();

    abs->add(abs, TERMINAL, (void *) newTerminalExpression());
    abs->add(abs, NONTERMINAL, (void *) newNonterminalExpression());
    abs->add(abs, TERMINAL, (void *) newTerminalExpression());
    abs->add(abs, TERMINAL, (void *) newTerminalExpression());

    printf("CONTEXT A\n");
    Context_t contextA = { .upper = true };
    for (int k = 0; k < abs->numExpressions; k++) {
        interpret(abs->type[k], abs->expression[k], &contextA);
    }

    printf("\ncontext b\n");
    Context_t contextB = { .upper = false };
    for (int k = 0; k < abs->numExpressions; k++) {
        interpret(abs->type[k], abs->expression[k], &contextB);
    }

    return 0;
}
