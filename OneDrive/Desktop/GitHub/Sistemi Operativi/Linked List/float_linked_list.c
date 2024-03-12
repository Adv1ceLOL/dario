#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "float_linked_list.h"

void FloatList_print(const ListHead* head){
  ListItem* aux=head->first;
  printf("[");
  while(aux){
    FloatListItem* element = (FloatListItem*) aux;
    printf("%f ", element->info);
    aux=aux->next;
  }
  printf("]\n");
}

float FloatList_sum(const ListHead* head){
    const ListItem* aux = head->first;
    float acc = 0.f;
    while(aux){
        const FloatListItem* element = (const FloatListItem*) aux;
        acc += element->info;
        aux=aux->next;
    }
    return acc;
}

