#ifndef COMMANDS_H
#define COMMANDS_H

//Libraries
#define _POSIX_C_SOURCE 200809L //Pour strdup
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h> //Pour tolower
#include "../save/save.h"
#include "../menu/menu.h"
#include "../load/load.h"

//Fonction pour les commandes
void commands(BTree* btree);
char* delete(BTree* btree, char* commande);
char* select(BTree* btree, char* commande);
Table* getTableInBtree(BTree* btree, const char* tableName);
char* insert(BTree* btree, char* commande);

#endif