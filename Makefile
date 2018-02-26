CFLAGS = -ggdb -std=c11

DP_CREATIONAL =
DP_STRUCTURAL =
DP_BEHAVIORAL =

# Creational design patterns
DP_CREATIONAL += abstract-factory
#DP_CREATIONAL += builder
#DP_CREATIONAL += factory-method
#DP_CREATIONAL += prototype
DP_CREATIONAL += singleton

# Structural design patterns
#DP_STRUCTURAL += adapter
DP_STRUCTURAL += bridge
DP_STRUCTURAL += composite
DP_STRUCTURAL += decorator
#DP_STRUCTURAL += facade
#DP_STRUCTURAL += flyweight
#DP_STRUCTURAL += proxy

# Behavioural design patterns
#DP_BEHAVIORAL += chain-of-responsibility
#DP_BEHAVIORAL += command
#DP_BEHAVIORAL += interpreter
DP_BEHAVIORAL += iterator
#DP_BEHAVIORAL += mediator
#DP_BEHAVIORAL += memento
#DP_BEHAVIORAL += observer
DP_BEHAVIORAL += state
DP_BEHAVIORAL += strategy
#DP_BEHAVIORAL += template-method
#DP_BEHAVIORAL += visitor

DP_ALL = $(DP_CREATIONAL) $(DP_STRUCTURAL) $(DP_BEHAVIORAL)

all: $(DP_ALL)

creational: $(DP_CREATIONAL)

structural: $(DP_STRUCTURAL)

behavioural: $(DP_BEHAVIORAL)

abstract-factory:
	gcc $(CFLAGS) -o abstract-factory abstract-factory.c

builder:
	gcc $(CFLAGS) -o builder builder.c

factory-method:
	gcc $(CFLAGS) -o factory-method factory-method.c

prototype:
	gcc $(CFLAGS) -o prototype prototype.c

singleton:
	gcc $(CFLAGS) -o singleton singleton.c

adapter:
	gcc $(CFLAGS) -o adapter adapter.c

bridge:
	gcc $(CFLAGS) -o bridge bridge.c

composite:
	gcc $(CFLAGS) -o composite composite.c

decorator:
	gcc $(CFLAGS) -o decorator decorator.c

facade:
	gcc $(CFLAGS) -o facade facade.c

flyweight:
	gcc $(CFLAGS) -o flyweight flyweight.c

proxy:
	gcc $(CFLAGS) -o proxy proxy.c

chain-of-responsibility:
	gcc $(CFLAGS) -o chain-of-responsibility chain-of-responsibility.c

command:
	gcc $(CFLAGS) -o command command.c

interpreter:
	gcc $(CFLAGS) -o interpreter interpreter.c

iterator:
	gcc $(CFLAGS) -o iterator iterator.c

mediator:
	gcc $(CFLAGS) -o mediator mediator.c

memento:
	gcc $(CFLAGS) -o memento memento.c

observer:
	gcc $(CFLAGS) -o observer observer.c

state:
	gcc $(CFLAGS) -o state state.c

strategy:
	gcc $(CFLAGS) -o strategy strategy.c

template-method:
	gcc $(CFLAGS) -o template-method template-method.c

visitor:
	gcc $(CFLAGS) -o visitor visitor.c

clean:
	rm -rf $(DP_ALL)

.PHONY: all creational structural behavioral clean
.PHONY: $(DP_ALL) $(DP_CREATIONAL) $(DP_STRUCTURAL) $(DP_BEHAVIORAL)
