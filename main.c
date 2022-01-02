#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "uthash.h"
#define TRUE 1
#define FALSE 0
#define BUFSIZE 100


typedef struct Visited {
    char *name;             /* key (string is WITHIN the structure) */
    int is_visited;
    UT_hash_handle hh;         /* makes this structure hashable */
}Visited;

typedef struct Node{
    char *operation;
    char *data;
    struct Node *next;
}Node;

typedef struct Queue{
    Node *head;
    Node *tail;
    int size;
    int limit;
} Queue;


int enqueue(Queue *queue, char *operation, char* data){
    if(queue->size >= queue->limit){
        return FALSE;
    }

    if((queue == NULL) || (data == NULL)){
        printf("enqueue: bad parameter");
        return FALSE;
    }

    Node *temp = (Node*) malloc(sizeof(Node));
    temp->data = data;
    temp->operation = operation;

    if(queue->size == 0){
        queue->size++;
        temp->next = NULL;
        queue->head = queue->tail = temp;
        //printf("enqueue item: %s", queue->tail->state->name);
        return TRUE;
    }

    queue->tail->next = temp;
    queue->tail = temp;
    queue->size++;
    return TRUE;
}

Node *dequeue(Queue *queue){
    if(queue->size == 0){
        return NULL;
    }

    Node *temp = queue->head;
    //printf("Dequeue state name: %s", temp->state->name);
    queue->head = queue->head->next;

    if(queue->head == NULL){
        queue->tail = NULL;
    }
    queue->size--;
    return temp;
}

Queue *constructQueue(int limit) {
    Queue *queue = (Queue*) malloc(sizeof (Queue));
    if (queue == NULL) {
        return NULL;
    }
    if (limit <= 0) {
        limit = 65535;
    }
    queue->limit = limit;
    queue->size = 0;
    queue->head = NULL;
    queue->tail = NULL;

    return queue;
}

void destructQueue(Queue *queue){
    Node *tmp;
    while(queue->size > 0){
        tmp = dequeue(queue);
        free(tmp);
    }
    free(queue);
}


void handle_temps();

/* HashMap Functions */
void set_bit(int visited, char *state_name);
void print_visited();
void free_table();
int get_bit(char *name);

/* HashMap Main */
Visited *v_states = NULL;

static int input_num = -1;
static int output_num = -1;
static int num_temp = 0;

int main(int argc, char *argv[]){
    Queue *queue = constructQueue(10000);
    FILE *fp = fopen(argv[1],"r");

    char buf[1000];

    char *data;

    int multiplexer_num = 0;
    while( fscanf(fp, "%s", buf) != EOF ){
        if(strcmp(buf, "MULTIPLEXER") == 0){
            fscanf(fp, "%s", buf);
            multiplexer_num = atoi(buf);
            data = malloc(30 * multiplexer_num);

            fscanf(fp, "%s", buf);
            handle_temps(buf);
            strcpy(data, buf);

            for(int i = 0; i < multiplexer_num-1; i++){
                fscanf(fp, "%s", buf);
                handle_temps(buf);
                strcat(data, " ");
                strcat(data, buf);
            }
            enqueue(queue, "MULTIPLEXER", data);
        }
        else if(strcmp(buf, "INPUTVAR") == 0){
            fscanf(fp, "%s", buf);
            input_num = atoi(buf);
            data = malloc(30 * input_num);

            fscanf(fp, "%s", buf);
            handle_temps(buf);
            strcpy(data, buf);

            for(int i = 0; i < input_num-1; i++){
                fscanf(fp, "%s", buf);
                handle_temps(buf);
                strcat(data, " ");
                strcat(data, buf);
            }
            enqueue(queue, "INPUTVAR", data);
        }
        else if(strcmp(buf, "OUTPUTVAR") == 0){
            fscanf(fp, "%s", buf);
            output_num = atoi(buf);
            data = malloc(30 * output_num);

            fscanf(fp, "%s", buf);
            handle_temps(buf);
            strcpy(data, buf);
            for(int i = 0; i < output_num-1; i++){
                fscanf(fp, "%s", buf);
                handle_temps(buf);
                strcat(data, " ");
                strcat(data, buf);
            }
            enqueue(queue, "OUTPUTVAR", data);
        }
        else if(strcmp(buf, "XOR") == 0){
            data = malloc(100);

            fscanf(fp, "%s", buf);
            handle_temps(buf);
            strcpy(data, buf);

            for(int i = 0; i < 2; i++){
                fscanf(fp, "%s", buf);
                handle_temps(buf);
                strcat(data, " ");
                strcat(data, buf);
            }
            enqueue(queue, "XOR", data);
        }
        else if(strcmp(buf, "OR") == 0){
            data = malloc(100);

            fscanf(fp, "%s", buf);
            handle_temps(buf);
            strcpy(data, buf);
            for(int i = 0; i < 2; i++){
                fscanf(fp, "%s", buf);
                handle_temps(buf);
                strcat(data, " ");
                strcat(data, buf);
            }
            enqueue(queue, "OR", data);
        }
        else if(strcmp(buf, "AND") == 0){
            data = malloc(100);

            fscanf(fp, "%s", buf);
            handle_temps(buf);
            strcpy(data, buf);
            for(int i = 0; i < 2; i++){
                fscanf(fp, "%s", buf);
                handle_temps(buf);
                strcat(data, " ");
                strcat(data, buf);
            }
            enqueue(queue, "AND", data);
        }
        else if(strcmp(buf, "NOR") == 0){
            data = malloc(100);

            fscanf(fp, "%s", buf);
            handle_temps(buf);
            strcpy(data, buf);
            for(int i = 0; i < 2; i++){
                fscanf(fp, "%s", buf);
                handle_temps(buf);
                strcat(data, " ");
                strcat(data, buf);
            }
            enqueue(queue, "NOR", data);
        }
        else if(strcmp(buf, "NAND") == 0){
            data = malloc(100);

            fscanf(fp, "%s", buf);
            handle_temps(buf);
            strcpy(data, buf);

            for(int i = 0; i < 2; i++){
                fscanf(fp, "%s", buf);
                handle_temps(buf);
                strcat(data, " ");
                strcat(data, buf);
            }
            enqueue(queue, "NAND", data);
        }
        else if(strcmp(buf, "NOT") == 0){
            data = malloc(100);

            fscanf(fp, "%s", buf);
            handle_temps(buf);
            strcpy(data, buf);

            for(int i = 0; i < 1; i++){
                fscanf(fp, "%s", buf);
                handle_temps(buf);
                strcat(data, " ");
                strcat(data, buf);
            }
            enqueue(queue, "OR", data);
        }
    }
    while(queue->size > 0){
        Node *n = dequeue(queue);
        printf("Operation: %s\n", n->operation);
        printf("Data: %s\n", n->data);
    }
    printf("Number of temps: %d", num_temp);

}

void handle_temps(char *s){
    if((s[0] >= 'a' && s[0] <= 'z')){
        if(get_bit(s) == FALSE){
            set_bit(TRUE, s);
            num_temp++;
        }
    }
}


/* HASHMAP */

void set_bit(int visited, char *state_name){
    Visited *v_state;
    printf("\n SET BIT:%s", state_name);
    HASH_FIND_STR(v_states, state_name, v_state);
    if(v_state == NULL){
        v_state = (Visited *) malloc(sizeof(*v_state));
        v_state->name = state_name;
        HASH_ADD_KEYPTR(hh, v_states, v_state->name, strlen(v_state->name), v_state);
    }
    v_state->is_visited = visited;
}

int get_bit(char *state_name){
    Visited *v_state;

    HASH_FIND_STR(v_states, state_name, v_state);
    if(v_state == NULL){
        return FALSE;
    }
    if(v_state->is_visited == FALSE){
        return FALSE;
    }
    return TRUE;
}

void free_table(){
    Visited *curr, *tmp;

    HASH_ITER(hh, v_states, curr, tmp){
        HASH_DEL(v_states, curr);
        free(curr);
    }
}

void print_visited() {
    Visited *s;
    for(s=v_states; s != NULL; s=(Visited*)(s->hh.next)) {
        printf("State name: %s, is visited: %d\n", s->name, s->is_visited);
    }
}


