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
    //printf("Riga 45: %s\n", root->value);
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


void insertName(IndexNodeString* root, struct Persona * persona){
    //printf("Riga 65: %s\n",persona->name);  //controllo
    if(root == NULL)return;
    if(persona->name <= root->value){
        if(root->left == NULL){
            IndexNodeString* node = treeGenString(persona->name);
            //printf("Riga 69: %s\n", node->value);
            root->left = node;
            return;
        }
        insertName(root->left, persona);
        return;
    }   
    if(root->right == NULL){
        IndexNodeString* node = treeGenString(persona->name);
        //printf("Riga 78: %s\n", node->value);
        root->right = node;
        return;
    }
    insertName(root->right, persona);
}

void insertSurname(IndexNodeString* root, struct Persona * persona){
    if(root == NULL)return;
    if(persona->surname <= root->value){
        if(root->left == NULL){
            IndexNodeString* node = treeGenString(persona->surname);
            root->left = node;
            return;
        }
        insertSurname(root->left, persona);
        return;
    }   
    if(root->right == NULL){
        IndexNodeString* node = treeGenString(persona->surname);
        root->right = node;
        return;
    }
    insertSurname(root->right, persona);
}

void insertAddress(IndexNodeString* root, struct Persona * persona){
    if(root == NULL)return;
    if(persona->address <= root->value){
        if(root->left == NULL){
            IndexNodeString* node = treeGenString(persona->address);
            root->left = node;
            return;
        }
        insertAddress(root->left, persona);
        return;
    }   
    if(root->right == NULL){
        IndexNodeString* node = treeGenString(persona->address);
        root->right = node;
        return;
    }
    insertAddress(root->right, persona);
}

void insertAge(IndexNodeInt* root, struct Persona * persona){
    if(root == NULL)return;
    if(root->value <= persona->age){
        if(root->left == NULL){
            IndexNodeInt* node = treeGenInt(persona->age);
            root->left = node;
            return;
        }
        insertAge(root->left, persona);
        return;
    }
    if(root->right == NULL){
        IndexNodeInt* node = treeGenInt(persona->age);
        root->right = node;
        return;
    }
    insertAge(root->right, persona);
}

void insert(Database * database, struct Persona * persona){
    if(persona == NULL) return;
    //* Creo un insert per ogni oggetto in Persona

    //! NAME INSERTION
    insertName(database->name, persona);
    //printf("Riga 143: %s\n", database->name);
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
    if(strcmp(name,"") == 0)return NULL; 
    findByNameAux(database->name, name);
}

//! FIND SURNAME
struct Persona* findBySurnameAux(IndexNodeString* node, char* surname){
    if(strcmp(node->value,surname) == 0) return (struct Persona*) node;
    if(node->left != NULL)findBySurnameAux(node->left, surname);
    if(node->right != NULL)findBySurnameAux(node->right, surname);
}

struct Persona* findBySurname(Database * database, char * surname){
    if(strcmp(surname,"") == 0)return NULL; 
    findBySurnameAux(database->surname, surname);
}

//! FIND ADRESS
struct Persona* findByAdressAux(IndexNodeString* node, char* address){
    if(strcmp(node->value,address) == 0) return (struct Persona*) node;
    if(node->left != NULL)findByAdressAux(node->left, address);
    if(node->right != NULL)findByAdressAux(node->right, address);
}

struct Persona* findByAddress(Database * database, char * address){
    if(strcmp(address,"") == 0)return NULL; 
    findByAdressAux(database->address, address);
}

//! FIND AGE
struct Persona* findByAgeAux(IndexNodeInt* node, int age){
    if(node->value == age) return (struct Persona*) node;
    if(node->left != NULL)findByAgeAux(node->left, age);
    if(node->right != NULL)findByAgeAux(node->right, age);
}

struct Persona* findByAge(Database * database, int age){
    if(age == 0)return NULL;
    findByAgeAux(database->age, age);
}

void printTreeString(IndexNodeString* node, int space) {
    if (node == NULL)
        return;

    space += 10;

    printTreeString(node->right, space);

    printf("\n");
    for (int i = 10; i < space; i++)
        printf(" ");
    printf("%s\n", node->value);

    printTreeString(node->left, space);
}

void printTreeInt(IndexNodeInt* node, int space) {
    if (node == NULL)
        return;

    space += 10;

    printTreeInt(node->right, space);

    printf("\n");
    for (int i = 10; i < space; i++)
        printf(" ");
    printf("%d\n", node->value);

    printTreeInt(node->left, space);
}

void printPersona(struct Persona* person) {
        printf("Name: %s\n", person->name);
        printf("Surname: %s\n", person->surname);
        printf("Address: %s\n", person->address);
        printf("Age: %d\n", person->age);
}

void freeDatabase(Database* root){
    free(root);
}

int main(int argc, const char * argv[]) {
    Database* database = malloc(sizeof(Database));
    // Creazione dei roots:
    database->name = treeGenString("Nomi");
    database->surname = treeGenString("Cognomi");
    database->address = treeGenString("Indirizzi");
    database->age = treeGenInt(0);

    struct Persona * deadpool = malloc(sizeof(struct Persona));
    char* name = "Ryan";
    char* surname = "Reynolds";
    char* address = "Los Angeles";
    int age = 47;

    strcpy(deadpool->name, name);
    strcpy(deadpool->surname, surname);
    strcpy(deadpool->address, address);
    deadpool->age = age;

    struct Persona * homelander = malloc(sizeof(struct Persona));
    name = "Antony";
    surname = "Starr";
    address = "New York";
    age = 48;

    strcpy(homelander->name, name);
    strcpy(homelander->surname, surname);
    strcpy(homelander->address, address);
    homelander->age = age;

    struct Persona * LastWeekTonight = malloc(sizeof(struct Persona));
    name = "John";
    surname = "Oliver";
    address = "Liverpool";
    age = 46;

    strcpy(LastWeekTonight->name, name);
    strcpy(LastWeekTonight->surname, surname);
    strcpy(LastWeekTonight->address, address);
    LastWeekTonight->age = age;

    insert(database, deadpool);
    insert(database, homelander);
    insert(database, LastWeekTonight);

    
    //struct Persona* find = findByName(database, "Ryan");
    //printPersona(find);

    printTreeString(database->name,0);
    printTreeString(database->surname,0);
    printTreeString(database->address,0);
    printTreeInt(database->age,0);

    freeDatabase(database);
}


