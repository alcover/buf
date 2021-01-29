CC = gcc -Wno-pedantic -std=gnu11 -g -O1
COMP = $(CC) -c $< -o $@
LINK = $(CC) $^ -o $@

out = buf.o

.PHONY: all check clean

all: $(out) unit example/pager

$(out): buf.c buf.h
	$(COMP)

unit: unit.c $(out)
	$(LINK)

check:
	@ ./unit

example/pager: example/pager.c $(out)
	$(LINK)

clean:
	@ rm -f $(all)