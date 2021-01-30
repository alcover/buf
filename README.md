# Buf
Experimental C buffer library.  

## Why ?

Because C strings are painful and unsafe.    

Say you're making a forum engine  
where a 'page' is a fixed-size buffer that you fill with posts.  

How would you do ?

### The hard way

```C
char page[PAGE_SZ];

// Keep track !
size_t page_len = 0;

while(1) {

    char* user = db_col("user");
    char* text = db_col("text");

    // Big enough ?
    #define POST_SIZE 100

    // Better malloc() + realloc() ?
    char post[POST_SIZE];  

    // may be truncated !
    snprintf (post, POST_SIZE, "<post>%s<p>%s</p></post>", user, text);

    // You type "strlen()" 100 times a day..
    const size_t post_len = strlen(post);

    // Check if it fits !
    // What about terminating null ? Will it fit ?
    if (page_len + post_len < PAGE_SZ) {    
        
        // Not sure..
        strncat (page, post, PAGE_SZ-page_len); 
        // ..about all this.
        page_len += post_len;

    } else {
        break;
    } 
}
```

### The **Buf** way

```C
Buf page = buf_new(PAGE_SZ);

while(1) {

    char* user = db_col("user");
    char* text = db_col("text");

    if (buf_append (page, "<post>%s<p>%s</p></post>", user, text)) {
        continue;
    }
}
```

# API

### buf_new
`Buf buf_new (const size_t cap);`

### buf_append
Append a string to `buf`, using printf() syntax.  
Returns: increase in length or zero if error or insufficient space.  

`int buf_append (Buf buf, const char* fmt, ...);`

### buf_write
Write string at the beginning of buf.
If the string's length exceeds capacity, nothing is written.  
Returns: new length or zero.

`int buf_write (Buf buf, const char* fmt, ...);`

Buf buf_copy (const Buf buf);
Buf buf_resize (Buf buf, const size_t newcap);
void buf_reset (Buf buf);

### Accessors
current length  
`size_t buf_getlen (const Buf buf);`
capacity  
`size_t buf_getcap (const Buf buf);`  
C string data  
`const char* buf_getdata (const Buf buf);`

## Quick sample

The *example* folder shows a simple pager.  
It reads lines from a file and fills an output 'page' of limited size.  
When the page cannot accept more lines, it is sent.  

Run `make && cd example && ./pager`

## Usage

<pre>
// app.c

#include "buf.h"

char name[] = "Bob";
Buf msg = buf_new(32);

buf_append (msg, "Hello! ");
buf_append (msg, "My name is %s", name);

puts (buf_getdata(msg));
</pre>

$ `gcc app.c buf.o -o app`

## Build

$ `make && make check`

## Goals

* zero dependencies
* useful & clean code
* speed

## TODO
* Add methods like split() ?