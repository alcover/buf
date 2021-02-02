#pragma once

#include <string.h>
#include <stdbool.h>

/* Underlying struct :
{
    uint32_t cap;       	// capacity
    uint32_t len;       	// current length
    unsigned char data[];   // null-terminated
}

Total memory size for n-bytes capacity : 8+n+1
*/

// Opaque pointer to struct
typedef struct Buf_s *Buf;

Buf buf_new (const size_t cap);

// Append a formatted c-string to `buf`.
// If new data would exceed capacity, `buf` stays unmodified.
// Returns: change in length, or zero on failure.
int buf_append (Buf buf, const char* fmt, ...);

// Write a formatted c-string at start of `buf`.
// If new data would exceed capacity, `buf` stays unmodified.
// Returns: new length, or zero on failure.
int buf_write (Buf buf, const char* fmt, ...);

bool buf_equal (const Buf a, const Buf b);
Buf buf_dup (const Buf buf);
bool buf_resize (Buf* pbuf, const size_t newcap);
void buf_reset (Buf buf);

// Accessors
size_t buf_cap (const Buf buf);
size_t buf_len (const Buf buf);
const char* buf_data (const Buf buf);

// Utility to print buf state : "Buf len/cap 'data'"
void buf_print (const Buf buf);