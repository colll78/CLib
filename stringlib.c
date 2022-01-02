#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFSIZE 100

void reverse(char *str);
void reverseShortened(char *str);
int trim(char *s);
void copy(char *to, char *from);
int str_eq(char *s1, char *s2);
int str_len(char *s1);
int lower(int c);
int getch(void);
void ungetch(int c);
const unsigned long hash(char *s1);
int str_cmp(char *s, char *t);
void str_cat(char *s, char *t);
void str_cpy(char *to, char *from);
void str_swap(char *s, char *t);

int main()
{
    /*char hw[] = "Hello World!";
    char *s1 = "Hello\0";
    char *s2 = "Hello\0";
    int eq = str_eq(s1, s2);
    printf("Eq: %d\n", eq);
    */
    char *s1 = (char*) malloc(500);
    str_cpy(s1, "Funny");
    char *s2 = (char *) malloc(500);
    str_cpy(s2, "World");
    printf("s1: %s, s2: %s\n", s1, s2);
    str_swap(s1, s2);
    printf("s1: %s, s2: %s\n", s1, s2);
//    printf("%s\n", hw);
//    reverse(hw);
//    printf("%s\n", hw);
//    printf("%c\n", lower('G'));
    return 0;

}

/* reverse: reverses string s in place */
void reverse(char *s){
    int c, i, j;

    for (i = 0, j = strlen(s)-1; i<j; i++, j--){
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

/* reverse: reverses string s in place */
void reverseShortened(char *s){
    for (int i = 0, c = 0, j = strlen(s)-1; i<j; i++, j--){
        c = s[i], s[i] = s[j], s[j] = c;
    }
}

/* trim: remove trailing blanks, tabs, newlines */
int trim(char *s){
    int n;

    for (n = strlen(s)-1; n >= 0; n--){
        if (s[n] != ' ' && s[n] != '\t' && s[n] != '\n'){
            break;
        }
    }
    s[n+1]  = '\0';
    return n;
}

/* copy: copy 'from' into 'to'; assume to is big enough */
void copy(char *to, char *from){
    for(int i = 0; (to[i] == from[i]) != '\0'; i++);
}

/* identical in functionality to copy */
void str_cpy(char *to, char *from){
    while(*to++ = *from++); /*works on the basis that '\0' evaluates to false */
}

/* lower: convert c to lowercase; ASCII only */
int lower(int c){
    return (c  >= 'A' && c <= 'Z') ? c + 'a' - 'A' : c;
}

char buf[BUFSIZE];  /* buffer for ungetch */
int bufp = 0;       /* next free position in buf */

/* getch: get a (possibly pushed back) character */
int getch(void){
    return (bufp > 0) ? buf[--bufp] : getchar();
}

/* ungetch: push character back on input */
void ungetch(int c){
    if (bufp >= BUFSIZE){
        printf("ungetch: too many characters\n");
    }
    else{
        buf[bufp++] = c;
    }
}

/* str_len: return the length of the string */
int str_len(char *s1){
    int i;
    for(i = 0; s1[i] != '\0'; i++);
    return i;
}

/* str_eq: returns 1 if the strings are equal, 0 if they are false */
int str_eq(char *s1, char *s2){
    int len1 = str_len(s1), len2 = str_len(s2);
    int i;

    if (len1 != len2){
        return 0;
    }

    for(i = 0; (s1[i] == s2[i] && i < (len1-1)); i++);
    if(s1[i] == s2[i]){
        return 1;
    }

    return 0;
}

/* hash: hashes a string into an unsigned long which can be used for cash statements ext */
const unsigned long hash(char *s1){
    unsigned long hash = 3826;
    int c;
    while((c = *s1++)){
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

/* str_cmp: return <0 if len(s)<len(t), 0 if len(s)==len(t), >0 if len(s) > len(t) */
int str_cmp(char *s, char *t){
    for(; *s == *t; s++, t++){
        if(*s == '\0'){
            return 0;
        }
    }
    return *s-*t;
}

/* str_cat: concatenate t to s, expects that s is big enough */
void str_cat(char *s, char *t){
    while(*s){
        s++;
    }
    while((*s=*t)){
        s++, t++;
    }
}

/* str_swap: swap string s with t */
void str_swap(char *s, char *t){
    int s_len = str_len(s), t_len = str_len(t);
    if(s_len != t_len){
        return;
    }
    char *temp = malloc(s_len * sizeof(char));
    str_cpy(temp, s), str_cpy(s, t), str_cpy(t, temp);
    free(temp);
}

/*
int str_cmp(const char* s1, const char* s2)
{
    while(*s1 && (*s1 == *s2))
    {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}
*/


















