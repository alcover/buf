CC = gcc -std=gnu11 -Wno-pedantic -g -O1
COMP = $(CC) -c $< -o $@
LINK = $(CC) $^ -o $@

out = buf.o

.PHONY: all check clean

all: $(out) unit example/forum README.html

$(out): buf.c buf.h
	$(COMP)

unit: unit.c $(out)
	$(LINK)

check:
	@ ./unit

example/forum: example/forum.c $(out)
	$(LINK)

README.html: README.md
	@markdown $< > $@

clean:
	@ rm -f $(all)