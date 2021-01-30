#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <stdarg.h>

#include "buf.h"

#define LOG(s) do{ printf("%s\n", s); fflush(stdout);} while(0)

int main() {

    #define PAGE_SZ 1024

    // char page[PAGE_SZ];
    
    // size_t page_len = 0;

    // while(1) {

    //     char* user = "user";
    //     char* text = "text";

    //     #define POST_SIZE 100
    
    //     char post[100];  

    //     snprintf (post, POST_SIZE, "<post><b>%s</b><p>%s</p></post>", user, text);

    //     const size_t post_len = strlen(post);
    
    //     if (page_len + post_len < PAGE_SZ) {    
    //         strncat (page, post, PAGE_SZ-page_len); 
    //         page_len += post_len;
    //     } else {
    //         break;
    //     } 
    // }
    // LOG(page);



    puts("\n");

    Buf page2 = buf_new(PAGE_SZ);

    while(1) {

        char* user = "user";
        char* text = "text";

        if (buf_append (page2, "<post>%s<p>%s</p></post>", user, text)) {
            continue;
        }
    }
    LOG(page2);
}