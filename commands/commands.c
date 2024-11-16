#include "commands.h"

char* select(BTree* btree, char* commande) {
    char* table = strtok(commande, " ");
    char* colonne = strtok(NULL, " ");
    char* condition = strtok(NULL, " ");
    char* valeur = strtok(NULL, " ");
    printf("Table : %s\n", table);
    printf("Colonne : %s\n", colonne);
    printf("Condition : %s\n", condition);
    printf("Valeur : %s\n", valeur);
    if (btree == NULL) {
        printf("\033[1;31m");
        printf("Erreur : Aucune base de donnée chargée\n");
        printf("\033[0m");
        return "Erreur : Aucune base de donnée chargée";
    }
    return "select";
}

//Recuperer le btree en paramètre
void commands(BTree* btree) {
    char commande[100];
    char result[200];
    do {
        result[0] = '\0';
        if (btree == NULL) {
        printf("\033[1;31m");
        printf("Erreur : Aucune base de donnée chargée\n");
        printf("\033[0m");
        return;
        }
        
        //Demander à l'utilisateur de rentrer une commande
        printf("\033[1;31m");
        printf("Entrez une commande (0 pour retourner en arrière): ");
        printf("\033[0m");
        scanf(" %[^\n]", commande);


        //On affiche la commande
        printf("\033[1;32m");
        printf("Commande : %s\n", commande);
        printf("\033[0m");

        //Switch pour les différentes commandes (insert, select, delete, update)
        if (strcmp(commande, "insert") == 0) {
            printf("insert\n");
        } else if (strncmp(commande, "select",6) == 0) {
            printf("select\n");
            strcpy(result, select(btree, commande));
        } else if (strcmp(commande, "delete") == 0) {
            printf("delete\n");
        } else if (strcmp(commande, "update") == 0) {
            printf("update\n");
      } else {
            printf("\033[1;31m");
            printf("Commande inconnue\n");
            printf("\033[0m");
      }
    if (result[0] != '\0') {
        printf("\033[1;32m");
        printf("Resultat : %s\n", result);
        printf("\033[0m");
    }
    result[0] = '\0';
    } while (strcmp(commande, "0") != 0);
}

