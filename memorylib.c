#include <stdio.h>
#include <stdlib.h>
#ALLOC_BUFFSIZE 10000

char *alloc(int n); /* return pointer to next block of memory with n free characters */
void afree(char *p); /* free storage pointed to by p */

int main()
{

    return 0;
}

static char allocbuf[ALLOC_BUFFSIZE];   /* storage space for alloc */
static char *allocp = allocbuff;        /* pointer to next free position */

char *alloc(int n){
    if(allocbuff + ALLOC_BUFFSIZE - allocp >= n){ /* if there is enough free memory */
        allocp += n;
        return allocp - n; /* old p */
    }else{ /* not enough room */
        return 0;
    }
}

void afree(char *p){
    if (p >= allocbuf && p < allocbuff+ALLOC_BUFFSIZE){
        allocp = p;
    }
}

