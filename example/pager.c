#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "../buf.h"

#define RECORD_FMT "{name:%s, age:%d}\n"
#define PAGE_SIZE 64

int pagenum = 1;

void send(Buf page) 
{ 
	printf ("page %d [%d/%d]\n%s\n", 
		pagenum++, buf_getlen(page), PAGE_SIZE, buf_getdata(page)); 
}

int main()
{
	FILE* file = fopen("people.txt", "r");
	if (!file) exit(EXIT_FAILURE);

	char* line = NULL;
	size_t len = 0;

	Buf page = buf_new(PAGE_SIZE);
   
    while (getline(&line, &len, file) != -1) 
    {
    	char name[100];
    	int age;

    	sscanf (line, "%s" "%d", name, &age);

        if (! buf_append (page, RECORD_FMT, name, age)) {
        	// record won't fit.
        	// so flush page 
        	send(page);
        	buf_reset(page);
        	// then re-add record.
        	buf_append (page, RECORD_FMT, name, age);
        }
    }

	// page not empty, send it.
	if (buf_getlen(page)) send(page);
	
	return 0;
}