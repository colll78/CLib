#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TRUE 1
#define FALSE 0
#define BUFSIZE 100

typedef enum {
    tk_Begin, tk_Eq, tk_If, tk_Else, tk_While, tk_Print, tk_Lbrace, tk_Rbrace, tk_Lparen,
    tk_Rparen, tk_Semi, tk_Comma, tk_Identifier, tk_Integer, tk_String, tk_SingleQuote,
    tk_DoubleQuote, tk_DFA_kw, tk_NFA_kw, tk_DFA_var, tk_NFA_var, tk_putc, tk_union,
    tk_intersec, tk_illegal, tk_EOF
} TokenType;


static char tk_to_text[][100] = {"Begin", "=", "if", "else", "while",
                              "print", "{", "}", "(", ")", ";", ",", "identifier",
                              "integer", "string", "\'", "\"", "DFA_kw",
                              "NFA_kw", "DFA_var", "NFA_var", "putc", "union", "intersec",
                              "illegal", "EOF"};
typedef struct{
    TokenType tok_type;
    int err_ln, err_col;
    union{
        int n;          /* value for constants */
        char *text;     /* text for idents */
    };
}Token;

Token get_tok();
Token expect(char symbol, TokenType if_yes, TokenType if_no, int err_line, int err_col);
int isAlpha(char c);
int isNum(char c);
int isAlphaNum(char c);
int str_eq(char *s1, char *s2);
int str_len(char *s1);
int getch(void);
void ungetch(int c);
int arrayToInteger(char *s);
int get_char();
TokenType get_ident_type(const char *s);

static FILE *source_fp, *dest_fp;
static int line = 1, col = 0, curr_ch = ' ';
static Token toks[100000];
static int tok_index;
static int next_type = FALSE;

int main(){
    Token tok = get_tok();
    printf("%s\n", tk_to_text[tok.tok_type]);
    toks[0] = tok;
    for (tok_index = 1; ((tok = get_tok()).tok_type != tk_EOF); tok_index++){
        toks[tok_index] = tok;
        printf("%s\n", tk_to_text[tok.tok_type]);
    }
}

int get_char(){
    int c = getch();
    col++;
    if(c == '\n'){
        col = 0;
        line++;
    }
    return c;
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

/* arrayToInteger: convert s to integer */
int arrayToInteger(char *s){
    int i, n;
    n = 0;

    for(i = 0; isNum(s[i]); ++i){
        n = 10 * n + (s[i] - '0');
    }
    return n;
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

static int kwd_cmp(const void *p1, const void *p2){
    return strcmp(*(char**)p1, *(char**)p2);
}

TokenType get_ident_type(const char *s){
    int num_keywords;
    static struct{
        char *keyword;
        TokenType t_type;
    } kwds[] = {
        {"DFA", tk_DFA_kw},
        {"NFA", tk_NFA_kw},
        {"else", tk_Else},
        {"if", tk_If},
        {"print", tk_Print},
        {"putc", tk_putc},
        {"while", tk_While},
    }, *kwp;

    num_keywords = sizeof(kwds)/sizeof(kwds[0]);
    return (kwp = bsearch(&s, kwds, num_keywords, sizeof(kwds[0]), kwd_cmp)) == NULL ? tk_Identifier : kwp->t_type;
}

Token get_ident_token(const char *s){
    int num_keywords;
    int type;

    static struct{
        char *keyword;
        TokenType t_type;
    } kwds[] = {
        {"DFA", tk_DFA_kw},
        {"NFA", tk_NFA_kw},
        {"else", tk_Else},
        {"if", tk_If},
        {"print", tk_Print},
        {"putc", tk_putc},
        {"while", tk_While},
    }, *kwp;

    num_keywords = sizeof(kwds)/sizeof(kwds[0]);

    kwp = bsearch(&s, kwds, num_keywords, sizeof(kwds[0]), kwd_cmp);

    if (kwp == NULL){
        type = tk_Identifier;
        if(next_type){
            type = next_type;
            next_type = FALSE;
        }
    }
    else{
        type = kwp -> t_type;
        if(next_type){
            /* expected variable got type keyword */
            next_type = FALSE;
            return (Token){tk_illegal, line, col, {0}};
        }
        switch(type){
            case tk_DFA_kw:
                next_type = tk_DFA_var;
                break;
            case tk_NFA_kw:
                next_type = tk_NFA_var;
                break;
            default:
                next_type = tk_Identifier;
                break;
        }
    }

    return (Token){type, line, col, {.text = s}};;
}

Token get_identifier_or_int(){
    char *data = malloc(52 * sizeof(char));
    int i;
    if (isNum(curr_ch)){
        data[0] = curr_ch;
        for(i = 1; isNum(curr_ch = get_char()); i++){
            data[i] = curr_ch;
        }
        data[i] = '\0';
        ungetch(curr_ch);
        return (Token){tk_Integer, line, col, {.n = arrayToInteger(data)}};
    }

    if(isAlphaNum(curr_ch) || curr_ch == '_'){
        data[0] = curr_ch;
        for(i = 1; isAlphaNum(curr_ch = get_char()) || curr_ch == '_'; i++){
            data[i] = curr_ch;
        }
    }else{
        /* Illegal token */
        return (Token){tk_illegal, line, col, {0}};
    }
    data[i] = '\0';
    ungetch(curr_ch);
    return (Token) get_ident_token(data);
}

Token get_tok(){
    do{
        curr_ch = get_char();
    }
    while(curr_ch == ' ' || curr_ch == '\t' || curr_ch == '\n');

    switch(curr_ch){
        case '{':
            return (Token){tk_Lbrace, line, col, {0}};
        case '}':
            return (Token){tk_Rbrace, line, col, {0}};
        case ';':
            return (Token){tk_Semi, line, col, {0}};
        case '(':
            return (Token){tk_Lparen, line, col, {0}};
        case ')':
            return (Token){tk_Rparen, line, col, {0}};
        case EOF:
            return (Token){tk_EOF, line, col, {0}};
        default:
            return get_identifier_or_int();
    }
}
