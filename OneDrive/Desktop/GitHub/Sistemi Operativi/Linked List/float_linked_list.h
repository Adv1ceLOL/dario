#pragma once
#include <stdio.h>
#include "linked_list.h"

typedef struct IntListItem{
  ListItem list;
  float info;
} FloatListItem;

void FloatList_print(const ListHead* head);
