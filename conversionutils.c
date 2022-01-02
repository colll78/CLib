#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#define MAXLINE 1000

double arrayToFloat(char s[]);
void intToArray(int n, char s[]);
void intToBaseB(int n, char s[], int b);
void intToPaddedArray(int n, char s[], int min);
void reverse(char s[]);
int arrayToInteger(char *s);

int main(){
    char line[MAXLINE];
    intToPaddedArray(500, line, 3);
    //printf("%s\n", line);
    return 0;
}

/* reverse: reverses string s in place */
void reverse(char s[]){
    int c, i, j;

    for (i = 0, j = strlen(s)-1; i<j; i++, j--){
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

/* convert a string into a double */
double arrayToFloat(char s[]){
        double val, power;
        int i, sign;

        /* skip whitespace */
        for (i = 0; isspace(s[i]); i++);

        sign = (s[i] == '-') ? -1 : 1;

        if(s[i]  == '+' || s[i] == '-'){
            i++;
        }

        for(val = 0.0; isdigit(s[i]); i++){
            val = 10.0 * val + (s[i] - '0');
        }

        if(s[i] == '.'){
            i++;
        }
        for(power = 1.0; isdigit(s[i]); i++){
            val = 10.0 * val + (s[i] - '0');
            power *= 10.0;
        }
        return sign * val / power;
}

/* intToArray: convert n to characters in s */
void intToArray(int n, char s[]){
    int i, sign;
    unsigned int pn = n;

    if ((sign = n) < 0){    /* record sign */
        pn = -n;             /* make n positive */
    }

    i = 0;
    do {    /* generate the digits in reverse order */
        s[i++] = (pn % 10)  + '0';  /* convert to char */
    } while ((pn /= 10) > 0);     /* delete it */
    if (sign < 0){
        s[i++] = '-';
    }
    s[i] = '\0';
    reverse(s);
}

/*
intToBaseB:
    converts the integer n into its base b character representation in the string s,
    handles up to base-62.
 */
void intToBaseB(int n, char s[], int b){
    static char baseMap[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int i, sign;

    if ((sign = n) < 0){
        n = -n;
    }
    i = 0;
    do {
        s[i++] = baseMap[n%b];
    }while((n /= b) > 0);

    if (sign < 0){
        s[i++] = '-';
    }
    s[i] = '\0';
    reverse(s);
}

/* intToPaddedArray: */
void intToPaddedArray(int n, char s[], int min){
    int sign, c, i = 0;
    unsigned int un = n;
    if ((sign = n) < 0){
        un = -n;
    }

    do {
        s[i++] = (un % 10) + '0';
    } while ((un /= 10) > 0);

    for (i; i < (min - c); i++){
        s[i] = '0';
    }

    if (sign < 0){
        s[i++] = '-';
    }

    s[i] = '\0';
    reverse(s);
}

/* arrayToInteger: convert s to integer */
int arrayToInteger(char *s){
    int i, n;
    n = 0;

    for(i = 0; s[i] >= '0' && s[i] <= '9'; ++i){
        n = 10 * n + (s[i] - '0');
    }
    return n;
}





