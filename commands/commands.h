#ifndef COMMANDS_H
#define COMMANDS_H

//Libraries
#define _POSIX_C_SOURCE 200809L //Pour strdup
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "../save/save.h"
#include "../menu/menu.h"
#include "../load/load.h"

//Fonction pour les commandes
void commands(BTree* btree);

#endif