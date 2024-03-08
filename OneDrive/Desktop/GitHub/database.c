#include <stdio.h>
#include <stdlib.h>

struct Studente {
    char nome[15];
    char cognome[50];
    char matricola[10];
    char email[20];
    int media;
    int eta;
};


typedef struct IndexNodeString {
    char * value;
} IndexNodeString;


typedef struct IndexNodeInt {
    int value;
} IndexNodeInt;


typedef struct {
    IndexNodeString* nome;
    IndexNodeString* cognome;
    IndexNodeString* matricola;
    IndexNodeString* email;
    IndexNodeInt* media;
    IndexNodeInt* eta;
} Database;

typedef struct Node {
    void* data;
    struct Node *left, *right;
} Node;

Node* create_node(void *data) {
    Node *node = (Node*)malloc(sizeof(Node));
    node->data = data;
    node->left = node->right = NULL;
    return node;
}

void free_tree(Node* root) {
    free(root);
}

void print_tree(Node *node, void (*fptr)(void *)) {
    if (node != NULL) {
        print_tree(node->left, fptr);
        fptr(node->data);
        print_tree(node->right, fptr);
    }
}

void print_int(void *n) {
    IndexNodeInt *num = (IndexNodeInt*)n;
    printf("%d ", num->value);
}

void print_string(void *str) {
    IndexNodeString *s = (IndexNodeString*)str;
    printf("%s ", s->value);
}

int main(int argc, const char * argv[]) {
    
    IndexNodeInt *intero = (IndexNodeInt*)malloc(sizeof(IndexNodeInt));
    intero->value = 1;
    Node *root = create_node(intero);

    IndexNodeString *dataString = (IndexNodeString*)malloc(sizeof(IndexNodeString));
    dataString->value = "Dario Neris Di Santo";
    root->left = create_node(dataString);

    IndexNodeInt *dataInt = (IndexNodeInt*)malloc(sizeof(IndexNodeInt));
    dataInt->value = 2;
    root->right = create_node(dataInt);
    
    print_tree(root, print_int);
    printf("\n");
    print_tree(root, print_string);
    printf("\n");  

    free_tree(root); 
    return 0;
}
