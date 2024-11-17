#ifndef SAVE_H
#define SAVE_H

//Libraries
#include "../load/load.h"
#include "../menu/menu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Fonctions
void save(BTree* btree, char* filename);
void save_node(BTreeNode* node, FILE* file);


#endif