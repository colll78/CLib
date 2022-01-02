#include <stdio.h>
#include <stdlib.h>
#define MAXLINE 1000
#define TRUE 1
#define FALSE 0

int getLine(char str[], int max);
int strIndex(char source[], char searchfor[]);
int rStrIndex(char source[], char target[]);
void expand(char regex[], char expanded[]);
int isAlpha(char c);
int isNum(char c);
int isAlphaNum(char c);
int expandable(char prev, char next);

/* simple program that return all lines containing a given substring, and the nums */
int main(){
    char str[] = "0-9a-zA-Z\0";
    char exp[MAXLINE];

    expand(str, exp);
    printf("%s", exp);
    /*
    char pattern[] = "ould";
    char line[MAXLINE];
    int found = 0;

    while(getLine(line, MAXLINE) > 0){
        if(strIndex(line, pattern) != -1){
            //printf("%s", line);
            found++;
        }
        printf("%d", rStrIndex(line, pattern));
    }
    return found;
    */
    return 0;
}

int getLine(char s[], int lim){
    int i, c;
    i = 0;
    while(--lim > 0 && (c=getchar()) != EOF && c != '\n'){
        s[i++] = c;
    }
    if (c == '\n'){
        s[i++] = '\n';
    }
    s[i] = '\0';
    return i;
}

int rStrIndex(char source[], char target[]){
    int i, j, k, m;
    m = -1;

    for (i = 0; source[i] != '\0'; i++){
        for (j = i, k = 0; target[k] != '\0' && source[j] == target[k]; j++, k++);
        if (k > 0 && target[k] == '\0'){
            m = i;
        }
    }
    return m;
}

int strIndex(char source[], char search[]){
    int i,j,k;

    for(i = 0; source[i] != '\0'; i++){
        for(j = i, k = 0; search[k] != '\0' && source[j] == search[k]; j++, k++);
        if (k > 0 && search[k] == '\0'){
            return i;
        }
    }
    return -1;
}

int isAlpha(char c){
    return ((c >= 65 && c <= 90) || (c >= 97 && c <= 122)) ? TRUE : FALSE;
}

int isNum(char c){
    return ((c >= 48 && c <= 57)) ? TRUE : FALSE;
}

int isAlphaNum(char c){
    return (isAlpha(c) || isNum(c));
}

int expandable(char prev, char next){
    return ((isAlpha(prev) && isAlpha(next)) || (isNum(prev) && isNum(next)));
}

/*
expand: expands shorthand a-z in regex[] into the equivalent complete list abc...xyz in expanded[].
*/
void expand(char regex[], char expanded[]){
    // adj[0] is prev index, adj[1] is next index.
    int adj[] = {0,0};
    int j, x;
    j = 0;
    for (int i=0; (regex[i] != '\0'); i++){
        adj[1] = regex[i+1];
        if (regex[i] == '-' && expandable(adj[0], adj[1])){
            for (x=0; x < (adj[1] - adj[0]); j++, x++){
                expanded[j] = adj[0] + x;
            }
        }
        else if (isAlphaNum(regex[i]) && adj[1] == '-'){
            ;
        }
        else{
            expanded[j++] = regex[i];
        }
        adj[0] = regex[i];
    }
    expanded[j] = '\0';
}
