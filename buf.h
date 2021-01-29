#pragma once
#include <string.h>

typedef struct Buf_s *Buf;

Buf buf_new (const size_t cap);

/*
	Append a string to `buf`, using printf() syntax.
	Returns: increase in length or zero if error or insufficient space.
*/
int buf_append (Buf buf, const char* fmt, ...);

/*
	Writes string at beginning.
	If the string's length exceeds capacity, nothing is written.
	Returns: new length or zero.
*/
int buf_write (Buf buf, const char* fmt, ...);

Buf buf_copy (const Buf buf);
Buf buf_resize (Buf buf, const size_t newcap);
void buf_reset (Buf buf);
void buf_print (const Buf buf);

// Accessors
size_t buf_getcap (const Buf buf);
size_t buf_getlen (const Buf buf);
const char* buf_getdata (const Buf buf);