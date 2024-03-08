#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// This represent a record in the only schema of this database
struct Persona {
    char name[20];
    char surname[50];
    char address[100];
    int age;
};

// This is a node of an index that hold a string
typedef struct IndexNodeString {
    char * value;
    struct IndexNodeString * left;
    struct IndexNodeString * right;
} IndexNodeString;

// This is a node of an index that hold an int
typedef struct IndexNodeInt {
    int value;
    struct IndexNodeInt * left;
    struct IndexNodeInt * right;
} IndexNodeInt;

// A database hold a set of records and a set of indexes
typedef struct {
    IndexNodeString * name;
    IndexNodeString * surname;
    IndexNodeString * address;
    IndexNodeInt * age;
} Database;

// TODO implement the following methods
// The method return a Persona or NULL 

//! CREAZIONE DI NODO E INSERIMENTO VALORE PER STRINGHE
IndexNodeString* treeGenString(char* root_value) {
    IndexNodeString *root = malloc(sizeof(IndexNodeString));
    if (root == NULL) {
        return NULL;
    } 
    root->value = root_value;
    root->left = NULL;
    root->right = NULL;
    return root;
}

//! CREAZIONE DI NODO E INSERIMENTO VALORE PER INTERI
IndexNodeInt* treeGenInt(int root_value) {
    IndexNodeInt *root = malloc(sizeof(IndexNodeInt));
    if (root == NULL) {
        return NULL;
    } 
    root->value = root_value;
    root->left = NULL;
    root->right = NULL;
    return root;
}


void insertName(IndexNodeString* node, struct Persona * persona){
    if(persona->name <= node->value){
        if(node->left == NULL){
            IndexNodeString* node = treeGenString(persona->name);
            node->left = node;
            return;
        }
        insertName(node->left, persona);
        return;
    }   
    if(node->right == NULL){
        IndexNodeString* node = treeGenString(persona->name);
        node->right = node;
        return;
    }
    insertName(node->right, persona);
}

void insertSurname(IndexNodeString* node, struct Persona * persona){
    if(persona->surname <= node->value){
        if(node->left == NULL){
            IndexNodeString* node = treeGenString(persona->surname);
            node->left = node;
            return;
        }
        insertSurname(node->left, persona);
        return;
    }   
    if(node->right == NULL){
        IndexNodeString* node = treeGenString(persona->surname);
        node->right = node;
        return;
    }
    insertSurname(node->right, persona);
}

void insertAddress(IndexNodeString* node, struct Persona * persona){
    if(persona->address <= node->value){
        if(node->left == NULL){
            IndexNodeString* node = treeGenString(persona->address);
            node->left = node;
            return;
        }
        insertAddress(node->left, persona);
        return;
    }   
    if(node->right == NULL){
        IndexNodeString* node = treeGenString(persona->address);
        node->right = node;
        return;
    }
    insertAddress(node->right, persona);
}

void insertAge(IndexNodeInt* node, struct Persona * persona){
    if(persona->age <= node->value){
        if(node->left == NULL){
            IndexNodeInt* node = treeGenInt(persona->age);
            node->left = node;
            return;
        }
        insertAge(node->left, persona);
        return;
    }
    if(node->right == NULL){
        IndexNodeInt* node = treeGenInt(persona->age);
        node->right = node;
        return;
    }
    insertAge(node->right, persona);
}

void insert(Database * database, struct Persona * persona){
    if(database == NULL) return;
    //* Creo un insert per ogni oggetto in Persona

    //! NAME INSERTION
    insertName(database->name, persona);
    //! SURNAME INSERTION
    insertSurname(database->surname, persona);
    //! ADDRESS INSERTION
    insertAddress(database->address, persona);
    //! AGE INSERTION
    insertAge(database->age, persona);
}

//! FIND NAME
//* Creo una funzione ausiliare per spostarmi ricorsivamente nell'albero
struct Persona* findByNameAux(IndexNodeString* node, char* name){
    if(strcmp(node->value,name) == 0) return (struct Persona*) node;
    if(node->left != NULL)findByNameAux(node->left, name);
    if(node->right != NULL)findByNameAux(node->right, name);
    
}

struct Persona* findByName(Database * database, char * name){
    if(database == NULL)return NULL; 
    findByNameAux(database->name, name);
}

//! FIND SURNAME
struct Persona* findBySurnameAux(IndexNodeString* node, char* surname){
    if(strcmp(node->value,surname) == 0) return (struct Persona*) node;
    if(node->left != NULL)findBySurnameAux(node->left, surname);
    if(node->right != NULL)findBySurnameAux(node->right, surname);
}

struct Persona* findBySurname(Database * database, char * surname){
    if(database == NULL)return NULL; 
    findBySurnameAux(database->surname, surname);
}

//! FIND ADRESS
struct Persona* findByAdressAux(IndexNodeString* node, char* address){
    if(strcmp(node->value,address) == 0) return (struct Persona*) node;
    if(node->left != NULL)findByAdressAux(node->left, address);
    if(node->right != NULL)findByAdressAux(node->right, address);
}

struct Persona* findByAddress(Database * database, char * address){
    if(database == NULL)return NULL; 
    findByAdressAux(database->address, address);
}

//! FIND AGE
struct Persona* findByAgeAux(IndexNodeInt* node, int age){
    if(node->value == age) return (struct Persona*) node;
    if(node->left != NULL)findByAgeAux(node->left, age);
    if(node->right != NULL)findByAgeAux(node->right, age);
}

struct Persona* findByAge(Database * database, int age){
    if(database == NULL)return NULL;
    findByAgeAux(database->age, age);
}

/*
void free_all(Database* database){
    free(database);
}
*/
