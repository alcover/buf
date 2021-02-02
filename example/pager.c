/*
    Example Buf usage.

    We read lines from a file, format them and fill a page with them.
    When the page can't accept more lines, we send it 
    and begin filling a new page.
*/

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
		pagenum++, buf_len(page), PAGE_SIZE, buf_data(page)); 
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
        	// record won't fit, so flush page 
        	send(page);
        	// and re-add record.
        	buf_reset(page);
        	buf_append (page, RECORD_FMT, name, age);
        }
    }

	// page not empty, send it.
	if (buf_len(page)) send(page);
	
	return 0;
}