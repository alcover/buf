/*
    Buf v0.1
    Copyright (C) 2021 - Francois Alcover <francois@alcover.fr>
    NO WARRANTY EXPRESSED OR IMPLIED.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <stdarg.h>
// #include <stdbool.h>

#define LOG(s) do{ printf("%s\n", s); fflush(stdout);} while(0)

#include "buf.h"

typedef struct Buf_s {
    uint32_t cap;       // capacity
    uint32_t len;       // current length
    unsigned char data[];   // null-terminated string
} Buf_s;

// mem size of a Buffer with capacity cap
// header + data + sentinel 
#define BUF_CHR (sizeof ((Buf_s){0}).data[0])
#define BUF_MEM(cap) (sizeof(Buf_s) + (cap+1)*BUF_CHR)

/*
    Allocate a new Buffer of capacity `cap`.
    Returns: Buf|NULL
*/
Buf buf_new (const size_t cap)
{
    Buf buf = malloc(BUF_MEM(cap));

    if (buf) {
        buf->cap = cap;
        buf->len = 0;
        buf->data[0] = 0;
        buf->data[cap] = 0;
    }
    
    return buf;
}

/*
    Append a formatted c-string to `buf`.
    If new data would exceed capacity, `buf` stays unmodified.
    Returns: change in length.
    Ex: buf_append (buf, str);
        buf_append (buf, "Hello");
        buf_append (buf, "%s has %d apples", "Mary", 10);
*/
int buf_append (Buf buf, const char* fmt, ...)
{
    if (!fmt) return 0;

    const size_t spc = buf->cap - buf->len;
    
    if (!spc) return 0;

    // get potential write length
    va_list args;
    va_start(args, fmt);
    const int len = vsnprintf (NULL, 0, fmt, args); //rem: end null not counted
    va_end(args);

    if (len > spc) return 0;

    char* end = buf->data + buf->len;

    errno = 0;
    va_start(args, fmt);
    int written = vsnprintf (end, spc+1, fmt, args);
    va_end(args);
     
    if (written < 0) {
        perror("buf_append");
        *end = 0; // just in case..
        return 0;
    }

    // truncated - useless?
    if (written > spc) {
        *end = 0; 
        return 0;
    }

    buf->len += written;

    return written;
}


/*
    Write a formatted c-string at beginning of `buf`.
    If new data would exceed capacity, `buf` stays unmodified.
    Returns: new length or zero on failure.
*/
int buf_write (Buf buf, const char* fmt, ...)
{
    if (!fmt) return 0;

    const size_t cap = buf->cap;

    if (!cap) return 0;

    // get potential write length
    va_list args;
    va_start(args, fmt);
    const int len = vsnprintf (NULL, 0, fmt, args);
    va_end(args);

    if (len > cap) return 0;

    errno = 0;
    va_start(args, fmt); 
    const int written = vsnprintf (buf->data, cap+1, fmt, args);
    va_end(args);
    
    if (written < 0) {
        perror("buf_write");
        return 0;
    }

    buf->len = written;

    return written;
}


bool buf_equal (const Buf a, const Buf b) 
{
    if (!a && !b) return true; //?
    if (!a || !b) return false; //?

    const size_t lena = a->len;
    const size_t lenb = b->len;

    if (lena != lenb) return false;

    return !memcmp (a->data, b->data, lena);
}

// todo: faster w/o calling new()
Buf buf_dup (const Buf buf)
{
    Buf ret = buf_new(buf->cap);

    if (ret) {
        // copies only up to current length
        memcpy (ret, buf, BUF_MEM(buf->len));
    }

    return ret;
}


bool buf_resize (Buf* pbuf, const size_t newcap)
{
    Buf buf = *pbuf;
    
    if (newcap == buf->cap) return true;
    
    Buf tmp = realloc (buf, BUF_MEM(newcap));    

    if (!tmp) {
        fprintf (stderr, "buf_resize failed\n");
        return false;
    }
    
    // truncated
    if (newcap < buf->len) {
        tmp->data[newcap] = 0;
        tmp->len = newcap;
    }

    buf->cap = newcap;
    *pbuf = buf;
    
    return true;
}


/* Accessors */
size_t buf_cap (const Buf buf) { return buf->cap; }
size_t buf_len (const Buf buf) { return buf->len; }
const char* buf_data (const Buf buf) { return buf->data; }


void buf_reset (Buf buf)
{
    buf->len = 0;
    buf->data[0] = 0;
}

// debugging utility
void buf_print (const Buf buf)
{
    printf ("Buf %d/%d '%s'\n", buf_len(buf), buf_cap(buf), buf_data(buf));
}
