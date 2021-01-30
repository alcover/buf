/*
    Unit tests of public methods
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <stdarg.h>

#include "buf.h"

#define LOG(s) do{ printf("%s\n", s); fflush(stdout);} while(0)

int main()
{
    const char* foo = "foo";
    const char* bar = "bar";
    const char* big = "loooooooooooooooong";
    const int i = 1;
    const size_t cap = 10;
    Buf buf;
    char cat[100];
    int rc;

    #define check(buf, cap, data) do { \
        assert (buf_getcap(buf) == cap); \
        assert (buf_getlen(buf) == strlen(data)); \
        assert (!strcmp(buf_getdata(buf), data)); \
    } while(0)

    /**** init ****/    

    buf = buf_new(cap); 
    check (buf, cap, "");

    /**** basic append ****/

    rc = buf_append (buf, foo);
    assert (rc == strlen(foo));
    check (buf, cap, foo);

    rc = buf_append (buf, bar);
    assert (rc == strlen(bar));
    sprintf (cat, "%s%s", foo, bar);
    check (buf, cap, cat);
    
    /**** reset ****/

    buf_reset(buf);
    check (buf, cap, "");

    /**** format append ****/

    buf = buf_new(cap);
    rc = buf_append (buf, "%s%s%d", foo, bar, i);
    sprintf (cat, "%s%s%d", foo, bar, i);
    assert (rc == strlen(cat));
    check (buf, cap, cat);

    /**** append too large ****/

    buf = buf_new (strlen(big)-1);
    rc = buf_append (buf, big);
    assert (rc == 0);
    assert (buf_getlen(buf) == 0);
    
    /**** write ****/

    buf = buf_new(cap);
    rc = buf_write (buf, foo);
    assert (rc = strlen(foo));
    check (buf, cap, foo);

    /**** format write ****/

    buf_write (buf, "%s%s%d", foo, bar, i);
    sprintf (cat, "%s%s%d", foo, bar, i);
    assert (rc = strlen(cat));
    check (buf, cap, cat);

    /**** write too large ****/

    assert (strlen(foo)+strlen(big) > cap);
    buf = buf_new(cap);
    buf_write (buf, foo);
    buf_write (buf, big);
    check (buf, cap, foo);

    /**** copy ****/

    buf = buf_new(cap);
    buf_append (buf, foo);
    Buf cpy = buf_copy(buf);
    check (cpy, cap, foo);

    /**** resize ****/

    const size_t needsz = strlen(foo)+strlen(big);
    assert (needsz > cap);
    buf = buf_new(cap);
    buf_append (buf, foo);
    buf = buf_resize (buf, needsz);
    buf_append (buf, big);
    sprintf (cat, "%s%s", foo, big);
    check (buf, needsz, cat);


    printf ("buf tests OK\n");
    #undef check

    return 0;
}