/*
    Example Buf usage.

    We read lines from a db, format them and fill a page with them.
    When the page can't accept more lines, we send it 
    and begin filling a new page.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "../buf.h"

#define POST_FMT "■ %s (%d points)\n%s\n"
#define PAGE_SZ 128

int pagenum = 1;

void send(Buf page) 
{ 
	printf ("--- PAGE %d [%dB] ---\n\n%s\n", pagenum++, buf_len(page), buf_data(page)); 
}

int main()
{
	FILE* db = fopen("posts.tsv", "r");

	if (!db) exit(EXIT_FAILURE);

	Buf page = buf_new(PAGE_SZ);
	char* row = NULL;
	size_t len = 0;

    puts ("Welcome to Buffy's forum !\n");
   
    while (getline(&row, &len, db) != -1) 
    {
    	int votes;
    	char user[20];
        char text[100];

    	sscanf (row, "%d" "%s" "\t" "%[^\n]", &votes, user, text);
        // sscanf (row, "%d,%s,%[^\n]", &votes, user, text);

        if (! buf_append (page, POST_FMT, user, votes, text)) {
        	// post won't fit, so flush page 
        	send(page);
        	// and re-add post.
        	buf_reset(page);
        	buf_append (page, POST_FMT, user, votes, text);
        }
    }

	// page not empty, send it.
	if (buf_len(page)) send(page);
	
	return 0;
}