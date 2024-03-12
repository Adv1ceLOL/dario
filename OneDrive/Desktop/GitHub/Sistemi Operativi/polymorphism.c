#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    void *data;
    struct Node *next;
} Node;

Node* createNode(void *data) {
    Node *newNode = (Node*)malloc(sizeof(Node));
    if(newNode == NULL) {
        printf("Errore di allocazione della memoria.\n");
        exit(0);
    }
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

void appendNode(Node **head, void *data) {
    Node *newNode = createNode(data);
    if(*head == NULL) {
        *head = newNode;
        return;
    }
    Node *current = *head;
    while(current->next != NULL) {
        current = current->next;
    }
    current->next = newNode;
}

// Funzione di stampa per interi
void printInt(void *data) {
    printf("%d ", *(int*)data);
}

// Funzione di stampa per float
void printFloat(void *data) {
    printf("%.2f ", *(float*)data);
}

// Funzione polimorfica per stampare la lista
void printList(Node *head, void (*printFunc)(void*)) {
    Node *current = head;
    while(current != NULL) {
        printFunc(current->data);
        current = current->next;
    }
    printf("\n");
}

int main() {
    Node *intList = NULL;
    Node *floatList = NULL;

    int num1 = 10, num2 = 20;
    appendNode(&intList, &num1);
    appendNode(&intList, &num2);
    printList(intList, printInt);

    float num3 = 1.1, num4 = 2.2;
    appendNode(&floatList, &num3);
    appendNode(&floatList, &num4);
    printList(floatList, printFloat);

    return 0;
}