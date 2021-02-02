# Buf
Experimental string buffer library.  

## Why ?

Because C strings can be painful, unsafe and slow.    

Say you're making a forum engine  
where a 'page' is a fixed-size buffer that you fill with posts.  

How would you safely build a page without truncating posts ?

### The hard way

```C
char page[PAGE_SZ];

// Keep track !
size_t page_len = 0;

while(1) {

    char* user = db_col("user");
    char* text = db_col("text");

    // Big enough ?
    #define POST_SZ 100

    // Better malloc and realloc ?
    char post[POST_SZ];  

    // May be truncated !
    snprintf (post, POST_SZ, "<div>%s<p>%s</p></div>", user, text);

    // Typing "strlen()" 100 times a day..
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

### The Buf way

```C
Buf page = buf_new(PAGE_SZ);

while(1) {

    char* user = db_col("user");
    char* text = db_col("text");

    if (!buf_append (page, "<div>%s<p>%s</p></div>", user, text)) {
        break;
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
If string length exceeds capacity, nothing is written.  
Returns: new length or zero.

`int buf_write (Buf buf, const char* fmt, ...);`

Buf buf_copy (const Buf buf);
Buf buf_resize (Buf buf, const size_t newcap);
void buf_reset (Buf buf);

### Accessors
current length  
`size_t buf_len (const Buf buf);`
capacity  
`size_t buf_cap (const Buf buf);`  
C string data  
`const char* buf_data (const Buf buf);`

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
Buf msg = buf_new(100);

buf_append (msg, "Hello! ");
buf_append (msg, "My name is %s", name);

puts (buf_data(msg));
</pre>

`gcc app.c buf.o -o app`

## Build & unit-test

`make && make check`

## Goals

* zero dependencies
* useful & clean code
* speed

## TODO
* utf8 ?
* Add methods like split() ?
* Make separate FlexBuf lib with auto-resize ?