#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#define TRUE 1
#define FALSE 0
#define BUFSIZE 100

typedef struct Position{
    int line;
    int offset;
    struct Position *next;
}Position;

typedef struct TreeNode{
    char *word;
    struct Position *pos;
    struct TreeNode *parent;
    struct TreeNode *left;
    struct TreeNode *right;
}TreeNode;

typedef struct Node{
    TreeNode *state;
    struct Node *next;
}Node;

typedef struct Queue{
    Node *head;
    Node *tail;
    int size;
    int limit;
} Queue;

typedef struct NodeOne{
    char *operation;
    int line;
    int offset;
    char *word;
    struct NodeOne *next;
}NodeOne;

typedef struct QueueOne{
    NodeOne *head;
    NodeOne *tail;
    int size;
    int limit;
} QueueOne;

void insertPos(Position **head_ref, Position pos);
TreeNode *newNode(char* s, Position pos, TreeNode *root);
TreeNode *insertNode(TreeNode *tree, char* s, Position pos);
TreeNode *removeWord(TreeNode *tree, char* word);
TreeNode *removeNodePosition(TreeNode *tree, Position pos);
TreeNode *removeLine(TreeNode *tree, int line);
void output(TreeNode *root);
void editNodePositions(TreeNode *tree, int line, int offset);
void text_output(TreeNode *head, int line);

/* Queue Functions */
Queue *constructQueue(int limit);
void destructQueue(Queue *queue);
int enqueue(Queue *queue, TreeNode *tree);
Node *dequeue(Queue *queue);


/* QueueOne Functions */
QueueOne *constructQueue1(int limit);
void destructQueue1(QueueOne *queue);
int enqueue1(QueueOne *queue, char *operation, int line, int offset);
int enqueue2(QueueOne *queue, char *operation, char *word, int line, int offset);
NodeOne *dequeue1(QueueOne *queue);

/* Util Functions */
int isNum(char c);
int get_char();
int unget_char(c);


int main(){
    /*TreeNode *head = insertNode(NULL, "Hello", (Position){0, 0, NULL});
    head = insertNode(head, "Hello", (Position){0, 1, NULL});
    head = insertNode(head, "Aello", (Position){1, 0, NULL});
    head = insertNode(head, "World", (Position){1, 1, NULL});
    output(head);
    printf("\n");
    //head = removeWord(head, "Aello");
    output(head);
    printf("\n");
    //head = removeNodePosition(head, (Position){0, 0, NULL});
    output(head);
    printf("\n");

    //head = removeWord(head, "World");
    //head = removeLine(head, 1);
    //output(head);
    //printf("\n");
    head = insertNode(head, "Dick", (Position){0, 4, NULL});
    head = insertNode(head, "Hello", (Position){0, 1, NULL});

    output(head);
    printf("\n");*/

    QueueOne *queue = constructQueue1(10000);
    int line = 0;
    int offset = 0;
    char buf[2500];

    char *data;
    //int line;
    //int offset;
    char *fix_atoi;

    while( fscanf(stdin, "%s", buf) != EOF ){
        if(strcmp(buf, "I") == 0){
            data = malloc(sizeof(50));
            fscanf(stdin, "%s", data);
            fix_atoi = malloc(15);
            fscanf(stdin, "%s", fix_atoi);
            line = atoi(fix_atoi);
            fscanf(stdin, "%d", &offset);
            enqueue2(queue, "I", data, line, offset);
        }
        else if(strcmp(buf, "R") == 0){
            data = malloc(sizeof(50));
            fscanf(stdin, "%s", data);
            if(isNum(data[0])){
                line = atoi(data);
                fscanf(stdin, "%d", &offset);
                //printf("FSFA %d\n", line);
                enqueue1(queue, "R", line, offset);
            }
            else{
                enqueue2(queue, "R", data, -1, -1);
            }
        }
        else if(strcmp(buf, "RL") == 0){
            fscanf(stdin, "%d", &line);
            enqueue1(queue, "RL", line, -1);
        }
        else if(strcmp(buf, "END") == 0){
            line = 0;
            offset = 0;
            break;
        }
    }
    TreeNode *head;
    char *read_word;
    if(fscanf(stdin, "%s", buf) != EOF){
        head = insertNode(NULL, buf, (Position){line, offset, NULL});
        offset++;
    }
    int x;
    while( fscanf(stdin, "%s", buf) != EOF ){
        read_word = (char*)malloc(strlen(buf) * sizeof(char));
        strcpy(read_word, buf);
        head = insertNode(head, read_word, (Position){line, offset, NULL});
        offset++;
        do{
            x = getchar();
            if(x == ' ' || x == '\t'){
                //offset++;
            }
            else if(x == '\n'){
                line++;
                offset = 0;
            }
            if(!isspace(x)){
                ungetc(x, stdin);

            }
           if(x == EOF){
                goto cheatcode;

           }

        }while(isspace(x));
    }

    cheatcode: ;
    //Process cmds
    NodeOne *cmd;
    while(queue->size > 0){

        int x, y;
        cmd = dequeue1(queue);
        if(strcmp(cmd->operation, "I") == 0){
            x = cmd->line, y = cmd->offset;
            //printf("X Y: %d, %d\n", x, y);
            head = insertNode(head, cmd->word, (Position){x, y, NULL});
        }
        else if(strcmp(cmd->operation, "RL") == 0){
            x = cmd->line;
            head = removeLine(head, x);
        }
        else if(strcmp(cmd->operation, "R") == 0){
            if(cmd->offset == -1 && cmd->line == -1){
                head = removeWord(head, cmd->word);
            }
            else{
                x = cmd->line, y = cmd->offset;
                head = removeNodePosition(head, (Position){x, y});
            }
        }
    }
    text_output(head, line);
    //output(head);

    return 0;
}

TreeNode *insertNode(TreeNode *tree, char* s, Position pos){
    static int num_recur = 0;
    static TreeNode *root;
    if(num_recur == 0){
        root = tree;
        num_recur++;
    }
    if (tree == NULL){
        return newNode(s, pos, root);
    }

    if (strcmp(s, tree->word) < 0){
        tree->left = insertNode(tree->left, s, pos);
    }
    else if (strcmp(s, tree->word) > 0){
        tree->right = insertNode(tree->right, s, pos);
    }
    else if(strcmp(tree->word, s) == 0){
        Position *temp = malloc(sizeof(Position));
        temp->line = -1;
        temp->offset = -1;
        temp->next = tree->pos;
        tree->pos = temp;
        editNodePositions(tree, pos.line, pos.offset);
        tree->pos->line = pos.line;
        tree->pos->offset = pos.offset;
    }
    /* return the (unchanged) node pointer */
    num_recur = 0;
    return tree;
}

TreeNode * minValueNode(TreeNode* node)
{
    TreeNode* current = node;

    /* loop down to find the leftmost leaf */
    while (current->left != NULL)
        current = current->left;

    return current;
}

TreeNode *removeWord(TreeNode *tree, char* word){
    if(tree == NULL) return tree;

    if(strcmp(tree->word, word) > 0 ){
        tree->left = removeWord(tree->left, word);
    }
    else if(strcmp(tree->word, word) < 0){
        tree->right = removeWord(tree->right, word);
    }

    else{
        if(tree->left == NULL){
            TreeNode *temp = tree->right;
            free(tree);
            return temp;
        }
        else if(tree->right == NULL){
            TreeNode *temp = tree->left;
            free(tree);
            return temp;
        }

        /* Smallest Node in the right subtree */
        TreeNode *temp = minValueNode(tree->right);

        tree->word = temp->word;
        tree->pos = temp->pos;
        tree->right = removeWord(tree->right, temp->word);
    }
    return tree;
}

Position *predecessorNode(Position *head, Position pos){
    if((head == NULL) || (head->next == NULL)){
        return(NULL);
    }
    if(head->next->line == pos.line && head->next->offset == pos.offset){
        return head;
    }
    else{
        return(predecessorNode((head->next), pos));
    }
}

Position *searchPositions(Position *head, Position pos){
    if (head == NULL) return NULL;
    Position *temp = head;

    while(temp != NULL){
        if(temp->line == pos.line && temp->offset == pos.offset){
            //printf("temp %d %d\n", temp->line, temp->offset);
            //printf("pos %d %d\n", pos.line, pos.offset);
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

void deletePosition(Position **head, Position pos){
    Position *temp;
    Position *pred;
    temp = searchPositions(*head, pos);
    if(temp != NULL){
        pred = predecessorNode(*head, pos);
        if (pred == NULL){
            *head = temp->next;
        }
        else{
            pred->next = temp->next;
        }
       free(temp);
    }
}

TreeNode *removeNodePosition(TreeNode *tree, Position pos){
    if (tree != NULL){
        if(searchPositions(tree->pos, pos) != NULL){
            deletePosition(&tree->pos, pos);
            if(tree->pos == NULL){
                tree = removeWord(tree, tree->word);
            }
            return tree;
        }
        if(tree->left != NULL){
            tree->left = removeNodePosition(tree->left, pos);
        }
        if(tree->right != NULL){
            tree->right = removeNodePosition(tree->right, pos);
        }
    }
    return tree;
}

TreeNode *searchNodePos(TreeNode *tree, Position pos){
    Queue *bfs_queue = constructQueue(1000);

    int i, j;
    TreeNode *temp = tree;
    if(temp != NULL){
        enqueue(bfs_queue, temp);
    }else{
        return NULL;
    }
    while(bfs_queue->size > 0){
        temp = dequeue(bfs_queue)->state;
        if(searchPositions(temp->pos, pos) != NULL){
            destructQueue(bfs_queue);
            return temp;
        }
        if(temp->right != NULL){
            enqueue(bfs_queue, temp->right);
        }
        if(temp->left != NULL){
            enqueue(bfs_queue, temp->left);
        }
    }
    destructQueue(bfs_queue);
    return NULL;
}

void editNodePositions(TreeNode *tree, int line, int offset){
    Queue *bfs_queue = constructQueue(1000);
    int i, j;
    Position *pos, *temp_pos;
    TreeNode *temp = tree;
    if(temp != NULL){
        enqueue(bfs_queue, temp);
    }
    while(bfs_queue->size > 0){
        temp = dequeue(bfs_queue)->state;
        pos = temp->pos;
        while(pos != NULL){
            if(pos->line == line && pos->offset >= offset){
                //printf("Curr Node: %d %d\n", pos->line, pos->offset);
                //printf("To Insert Node: %d %d\n", line, offset);
                pos->offset++;
            }
            pos = pos->next;
        }

        if(temp->right != NULL){
            enqueue(bfs_queue, temp->right);
        }
        if(temp->left != NULL){
            enqueue(bfs_queue, temp->left);
        }
    }
    destructQueue(bfs_queue);
}

TreeNode *removeLine(TreeNode *tree, int line){
    for(int i = 0; searchNodePos(tree, (Position){line, i, NULL}) != NULL; i++){
        tree = removeNodePosition(tree, (Position){line, i, NULL});
    }
    return tree;
}

TreeNode *newNode(char* s, Position pos, TreeNode *root){
    TreeNode *temp = (TreeNode*)malloc(sizeof(TreeNode));
    temp->word = malloc(strlen(s) * sizeof(char));
    strcpy(temp->word, s);
    free(s);
    temp->parent = temp->right = temp->left = NULL;
    temp->pos = malloc(sizeof(Position));
    temp->pos->line = -1;
    temp->pos->offset = -1;
    if(root != NULL){
        editNodePositions(root, pos.line, pos.offset);
    }
    temp->pos->line = pos.line;
    temp->pos->offset = pos.offset;
    temp->pos->next = NULL;

    return temp;
}

void output(TreeNode *root){
    if (root != NULL){
        output(root->left);
        printf("%s \n", root->word);
        Position *temp = root->pos;
        while(temp != NULL){
            printf("Line: %d, Offset: %d\n", temp->line, temp->offset);
            temp = temp->next;
        }
        output(root->right);
    }
}

void text_output(TreeNode *root, int line){
    int i, j;
    if (root != NULL){
        TreeNode *temp;
        for(i = 0; i < line; i ++){
            for(j = 0; j < 500; j++){
                if ((temp = searchNodePos(root, (Position){i, j, NULL})) != NULL){
                    printf("%s ", temp->word);
                }
            }
            printf("\n");
        }
    }
}

void insertPos(Position **head_ref, Position pos){
    Position *temp = malloc(sizeof(Position));
    temp->line = pos.line;
    temp->offset = pos.offset;

    temp->next = (*head_ref);

    (*head_ref) = temp;
}








int enqueue(Queue *queue, TreeNode *tree){
    if(queue->size >= queue->limit){
        return FALSE;
    }

    if((queue == NULL) || (tree == NULL)){
        //printf("enqueue: bad parameter\n");
        return FALSE;
    }

    Node *temp = (Node*) malloc(sizeof(Node));
    temp->state = tree;

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


/* QueueOne Functions */

int enqueue2(QueueOne *queue, char *operation, char *word, int line_i, int offset_i){
    if(queue->size >= queue->limit){
        return FALSE;
    }

    if((queue == NULL) || (operation == NULL)){
        printf("enqueue2: bad parameter");
        return FALSE;
    }

    NodeOne *temp = (NodeOne*) malloc(sizeof(NodeOne));
    temp->operation = operation;
    temp->line = line_i;
    //printf("Enqueue: %d\n", temp->line);
    temp->offset = offset_i;
    char *word_h = malloc(strlen(word));
    strcpy(word_h, word);
    temp->word = word_h;
    free(word);


    if(queue->size == 0){
        queue->size++;
        temp->next = NULL;
        queue->head = queue->tail = temp;
        //printf("enqueue item: %s", queue->tail->state->name);
        return TRUE;
    }

    queue->tail->next = temp;
    //printf("DIGIGT :%d\n", queue->tail->next->line);
    queue->tail = temp;
    queue->size++;
    return TRUE;

}


int enqueue1(QueueOne *queue, char *operation, int line_i, int offset_i){
    if(queue->size >= queue->limit){
        return FALSE;
    }

    if((queue == NULL) || (operation == NULL)){
        //printf("enqueue1: bad parameter");
        return FALSE;
    }

    NodeOne *temp = (NodeOne*) malloc(sizeof(NodeOne));
    temp->operation = operation;
    temp->line = line_i;
    temp->offset = offset_i;
    temp->word = NULL;

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

NodeOne *dequeue1(QueueOne *queue){
    if(queue->size == 0){
        return NULL;
    }

    NodeOne *temp = queue->head;
    //printf("PLEASE X: %d\n", temp->line);
    //printf("Dequeue state name: %s", temp->state->name);
    queue->head = queue->head->next;

    if(queue->head == NULL){
        queue->tail = NULL;
    }
    queue->size--;
    return temp;
}

QueueOne *constructQueue1(int limit) {
    QueueOne *queue = (QueueOne*) malloc(sizeof(QueueOne));
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

void destructQueue1(QueueOne *queue){
    NodeOne *tmp;
    while(queue->size > 0){
        tmp = dequeue1(queue);
        free(tmp);
    }
    free(queue);
}

/* Util Functions */
int isNum(char c){
    return ((c >= 48 && c <= 57)) ? TRUE : FALSE;
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
