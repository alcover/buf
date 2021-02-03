CC = gcc -std=gnu11 -Wall -g -O1
COMP = $(CC) -c $< -o $@
LINK = $(CC) $^ -o $@

out = buf.o

.PHONY: all check clean

all: $(out) unit example/forum

$(out): buf.c buf.h
	$(COMP)

unit: unit.c $(out)
	$(LINK)

check:
	@ ./unit

example/forum: example/forum.c $(out)
	$(LINK)

clean:
	@ rm -f $(out) unit example/forum