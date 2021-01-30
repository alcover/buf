CC = gcc -Wno-pedantic -g -O1
COMP = $(CC) -c $< -o $@
LINK = $(CC) $^ -o $@

out = buf.o

.PHONY: all check clean

all: $(out) unit test example/pager README.html

$(out): buf.c buf.h
	$(COMP)

unit: unit.c $(out)
	$(LINK)

check:
	@ ./unit

example/pager: example/pager.c $(out)
	$(LINK)

README.html: README.md
	@markdown $< > $@

test: test.c $(out)
	$(LINK)

clean:
	@ rm -f $(all)