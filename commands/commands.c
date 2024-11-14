#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../save/save.h"
#include "../load/load.h"
#include "../menu/menu.h"
#include "commands.h"


//Recuperer le btree en paramètre
void commands(BTree* btree) {
    do {
        if (btree == NULL) {
        printf("\033[1;31m");
        printf("Erreur : Aucune base de donnée chargée\n");
        printf("\033[0m");
        return 0;
        }
        
        //Demander à l'utilisateur de rentrer une commande
        printf("\033[1;31m");
        printf("Entrez une commande (0 pour retourner en arrière): ");
        printf("\033[0m");

        char commande[100];
        scanf(" %[^\n]", commande);



        //On affiche la commande
        printf("\033[1;32m");
        printf("Commande : %s\n", commande);
        printf("\033[0m");
    } while (1);
}