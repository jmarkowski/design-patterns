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
DP_STRUCTURAL += adapter
DP_STRUCTURAL += bridge
DP_STRUCTURAL += composite
DP_STRUCTURAL += decorator
DP_STRUCTURAL += facade
DP_STRUCTURAL += flyweight
#DP_STRUCTURAL += proxy

# Behavioural design patterns
#DP_BEHAVIORAL += chain-of-responsibility
DP_BEHAVIORAL += command
#DP_BEHAVIORAL += interpreter
DP_BEHAVIORAL += iterator
DP_BEHAVIORAL += mediator
#DP_BEHAVIORAL += memento
DP_BEHAVIORAL += observer
DP_BEHAVIORAL += state
DP_BEHAVIORAL += strategy
DP_BEHAVIORAL += template-method
DP_BEHAVIORAL += visitor

DP_ALL = $(DP_CREATIONAL) $(DP_STRUCTURAL) $(DP_BEHAVIORAL)

all: $(DP_ALL)

creational: $(DP_CREATIONAL)

structural: $(DP_STRUCTURAL)

behavioural: $(DP_BEHAVIORAL)

abstract-factory: abstract-factory.c
	gcc $(CFLAGS) -o $@ $<

builder: builder.c
	gcc $(CFLAGS) -o $@ $<

factory-method: factory-method.c
	gcc $(CFLAGS) -o $@ $<

prototype: prototype.c
	gcc $(CFLAGS) -o $@ $<

singleton: singleton.c
	gcc $(CFLAGS) -o $@ $<

adapter: adapter.c
	gcc $(CFLAGS) -o $@ $<

bridge: bridge.c
	gcc $(CFLAGS) -o $@ $<

composite: composite.c
	gcc $(CFLAGS) -o $@ $<

decorator: decorator.c
	gcc $(CFLAGS) -o $@ $<

facade: facade.c
	gcc $(CFLAGS) -o $@ $<

flyweight: flyweight.c
	gcc $(CFLAGS) -o $@ $<

proxy: proxy.c
	gcc $(CFLAGS) -o $@ $<

chain-of-responsibility: chain-of-responsibility.c
	gcc $(CFLAGS) -o $@ $<

command: command.c
	gcc $(CFLAGS) -o $@ $<

interpreter: interpreter.c
	gcc $(CFLAGS) -o $@ $<

iterator: iterator.c
	gcc $(CFLAGS) -o $@ $<

mediator: mediator.c
	gcc $(CFLAGS) -o $@ $<

memento: memento.c
	gcc $(CFLAGS) -o $@ $<

observer: observer.c
	gcc $(CFLAGS) -o $@ $<

state: state.c
	gcc $(CFLAGS) -o $@ $<

strategy: strategy.c
	gcc $(CFLAGS) -o $@ $<

template-method: template-method.c
	gcc $(CFLAGS) -o $@ $<

visitor: visitor.c
	gcc $(CFLAGS) -o $@ $<

clean:
	rm -rf $(DP_ALL)

.PHONY: all creational structural behavioral clean
