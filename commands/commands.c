#include "commands.h"

//Fonction pour select
char* select(BTree* btree, char* commande) {
    char* colonnes = NULL;
    char* table = NULL;
    char* tmp = strtok(commande, " ");

    //On saute le SELECT
    if (tmp != NULL) {
        tmp = strtok(NULL, " ");
    }

    //On récupère les colonnes demandées jusqu'à FROM
    if (tmp != NULL) {
        colonnes = tmp;
        while ((tmp = strtok(NULL, " ")) != NULL) {
            //Si on arrive à FROM, on arrête
            if (strcasecmp(tmp, "from") == 0) {
                break;
            }
            //On ajoute la colonne à la liste
            colonnes = realloc(colonnes, strlen(colonnes) + strlen(tmp) + 2);
            strcat(colonnes, " ");
            strcat(colonnes, tmp);
        }
    }

    //On récupère le nom de la table après FROM
    if (tmp != NULL) {
        table = strtok(NULL, " ");
    }

    //Affichage des résultats pour vérification
    printf("Colonnes : %s\n", colonnes ? colonnes : "Aucune");
    printf("Table : %s\n", table ? table : "Aucune");

    //Si les colonnes/la table sont nulles, on renvoie une erreur
    if (colonnes == NULL || table == NULL) {
        return "Erreur : Commande SELECT incorrecte";
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

