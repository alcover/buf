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

#define LOG(s) do{ printf("%s\n", s); fflush(stdout);} while(0)

#include "buf.h"

typedef struct Buf_s {
    uint32_t cap; 		// capacity
    uint32_t len; 		// current length
    unsigned char data[];  	// null-terminated string
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
	In-place append a string to Buffer `buf`, using printf() syntax.
	If not enough space, buf length stays unmodified and zero is returned.
	Ex:	buf_append (buf, "Hello");
		buf_append (buf, "%s is %d years old", "John", 40);
	Returns: increase in length or zero if error or insufficient space.
*/
int buf_append (Buf buf, const char* fmt, ...)
{
	if (!fmt) return 0;

	const size_t spc = buf->cap - buf->len;

	if (!spc) return 0;

	char* end = buf->data + buf->len;

    va_list args;
    va_start(args, fmt);
	int written = vsnprintf (end, spc+1, fmt, args);
	va_end(args);
	
	// Error 
	if (written < 0) {
		fprintf (stderr, "buf_append");
		*end = 0; 
		return 0;
	}

	// Would truncate
	if (written > spc) {
		*end = 0; 
		return 0;
	}

	buf->len += written;

	return written;
}


// todo: faster w/o calling new()
Buf buf_copy (const Buf buf)
{
	Buf ret = buf_new(buf->cap);

	if (ret) {
		// copies only up to current length
		memcpy (ret, buf, BUF_MEM(buf->len));
	}

	return ret;
}

// problem: dangling buf pointer if foo = resize(buf)
Buf buf_resize (Buf buf, const size_t newcap)
{
	const size_t cap = buf->cap;
	const size_t len = buf->len;

	if (newcap < cap) {
		
		if (newcap < len) {
			buf->data[newcap] = 0;
			buf->len = newcap;
		}

		buf->cap = newcap;
	
	} else if (newcap > cap) {
	
		void* tmp = realloc (buf, BUF_MEM(newcap));
		
		if (!tmp) {
			fprintf (stderr, "buf_resize failed\n");
		} else {
			buf = tmp;
			buf->cap = newcap;
		}
	}
	
	return buf;
}

/*
	Writes formatted string at beginning.
	If the string's length exceeds capacity, nothing is written.
	Returns: new length or zero.
*/
int buf_write (Buf buf, const char* fmt, ...)
{
 	if (!fmt) return 0;

	const size_t cap = buf->cap;

	if (!cap) return 0;

    va_list args;
    va_start(args, fmt);
	const int len = vsnprintf (buf->data, 0, fmt, args);
	va_end(args);
	// printf("%d\n",len);

	// Would truncate
	if (len > cap) return 0;

	va_start(args, fmt); // ?
	const int written = vsnprintf (buf->data, cap+1, fmt, args);
	va_end(args);
	
	// Error 
	if (written < 0) {
		fprintf (stderr, "buf_write");
		return 0;
	}

	buf->len = written;

	return written;
}



/* Accessors */
size_t buf_getcap (const Buf buf) { return buf->cap; }
size_t buf_getlen (const Buf buf) { return buf->len; }
const char* buf_getdata (const Buf buf) { return buf->data; }


void buf_reset (Buf buf)
{
	buf->len = 0;
	buf->data[0] = 0;
}

// debugging utility
void buf_print (const Buf buf)
{
	printf ("Buf %d/%d '%s'\n", buf_getlen(buf), buf_getcap(buf), buf_getdata(buf));
}
