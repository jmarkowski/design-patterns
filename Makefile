CFLAGS = -ggdb -std=c11

DP_CREATIONAL =
DP_STRUCTURAL =
DP_BEHAVIORAL =

# Creational design patterns
DP_CREATIONAL += abstract-factory
DP_CREATIONAL += builder
DP_CREATIONAL += factory-method
DP_CREATIONAL += prototype
DP_CREATIONAL += singleton

# Structural design patterns
DP_STRUCTURAL += adapter
DP_STRUCTURAL += bridge
DP_STRUCTURAL += composite
DP_STRUCTURAL += decorator
DP_STRUCTURAL += facade
DP_STRUCTURAL += flyweight
DP_STRUCTURAL += proxy

# Behavioral design patterns
DP_BEHAVIORAL += chain-of-responsibility
DP_BEHAVIORAL += command
DP_BEHAVIORAL += interpreter
DP_BEHAVIORAL += iterator
DP_BEHAVIORAL += mediator
DP_BEHAVIORAL += memento
DP_BEHAVIORAL += observer
DP_BEHAVIORAL += state
DP_BEHAVIORAL += strategy
DP_BEHAVIORAL += template-method
DP_BEHAVIORAL += visitor

DP_ALL = $(DP_CREATIONAL) $(DP_STRUCTURAL) $(DP_BEHAVIORAL)

all: $(DP_ALL)

creational: $(DP_CREATIONAL)

structural: $(DP_STRUCTURAL)

behavioral: $(DP_BEHAVIORAL)

%:
	gcc $(CFLAGS) -o $@ $(addsuffix .c,$@)

clean:
	rm -rf $(DP_ALL)

.PHONY: all creational structural behavioral clean
