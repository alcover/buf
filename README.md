# Buf
Experimental C buffer library.  

## Why ?

Because strings in C are painful and unsafe :  

You're making a forum engine.  
A `page` is a fixed-size buffer.  
You fill it with posts from the DB.  

###The Classical way :

<pre>
#define PAGE_SIZE 1024

// your bet !
#define POST_SIZE 100

char page[PAGE_SIZE];

// keep track !
size_t page_len = 0;

bool add_post (char* user, char* text) 
{
    // too bad if not big enough !
    char post[POST_SIZE];  

    // may be truncated !
    vsnprintf (post, POST_SIZE, "user %s said %s\n", user, text);

    // 10000th time you type such a line...
    const size_t post_len = strlen(post);

    // check if it fits !
    // what about terminating null ? Will it fit ?
    if (page_len + post_len < PAGE_SIZE) {    
        strcat (page, post); 
        // keep track !
        page_len += post_len;
        return true;
    } else {
        return false;
    } 
}
</pre>

### The **Buf** way :

<pre>
#define PAGE_SIZE 1024

Buf page = buf_new(PAGE_SIZE);

bool add_post (char* user, char* text) 
{
    return buf_append (page, "user %s said %s\n", user, text);
}
</pre>

## Quick sample

The *example/* folder shows a simple pager.  
It reads lines from `people.txt` and fills an output 'page' of limited size.  
When a page cannot accept more lines, it is sent.  

Run `make && cd example && ./pager`

## Usage

<pre>
// app.c

#include "buf.h"

char name[] = "Bob";
Buf msg = buf_new(32);

buf_append (msg, "Hello! ");
buf_append (msg, "My name is %s", name);

puts (msg);
</pre>

  $ gcc app.c buf.o -o app

## Goals

* zero dependencies
* useful & clean code
* speed

## TODO
* Add methods like split() ?