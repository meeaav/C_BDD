#include "commands.h"

//On ignore les warnings pour les fonctions non déclarées parce que j'ai pas terminééééé
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wimplicit-function-declaration"
#pragma GCC diagnostic ignored "-Wunused-parameter"

//########################## SELECT ##########################
char* select(BTree* btree, char* commande) {
    //Déclaration des variables
    char* colonnes = NULL;
    char* table = NULL;
    char* condition = NULL;
    char* colonne_where = NULL;
    char* valeur_where = NULL;
    char* tmp = strtok(commande, " ");

    //On saute le SELECT
    if (tmp != NULL) {
        tmp = strtok(NULL, " ");
    }
    //On récupère les colonnes demandées jusqu'à FROM
    if (tmp != NULL) {
        colonnes = strdup(tmp);
        while ((tmp = strtok(NULL, " ")) != NULL) {
            //Si on arrive à FROM, on arrête
            if (strcasecmp(tmp, "from") == 0) {
                break;
            }
            //On alloue de la mémoire pour les colonnes 
            char* new_colonnes = realloc(colonnes, strlen(colonnes) + strlen(tmp) + 2);
            if (new_colonnes == NULL) {
                free(colonnes);
                return "Erreur : Allocation mémoire échouée";
            }
            //On ajoute un espace entre chaque colonne
            colonnes = new_colonnes;
            strcat(colonnes, " ");
            strcat(colonnes, tmp);
        }
    }
    //On récupère le nom de la table après FROM
    if (tmp != NULL) {
        table = strtok(NULL, " ");
    }
    //Si les colonnes/la table sont nulles, on renvoie une erreur
    if (colonnes == NULL || table == NULL) {
        free(colonnes);
        return "Erreur : Commande SELECT incorrecte";
    }

    //On regarde s'il y a un WHERE
    if (table != NULL) {
        tmp = strtok(NULL, " ");
        if (tmp != NULL && strcasecmp(tmp, "where") == 0) {
            // On récupère la condition
            colonne_where = strtok(NULL, " ");
            condition = strtok(NULL, " ");
            valeur_where = strtok(NULL, " ");

            // Si il y a une valeur manquante, on renvoie une erreur
            if (colonne_where == NULL || condition == NULL || valeur_where == NULL) {
                free(colonnes);
                return "Erreur : Clause WHERE incorrecte";
            }
        }
    }

    //Affichage des résultats pour vérification
    printf("Colonnes : %s\n", colonnes ? colonnes : "Aucune");
    printf("Table : %s\n", table ? table : "Aucune");
    if (colonne_where != NULL) {
        printf("WHERE %s %s %s\n", colonne_where, condition, valeur_where);
    }

    //Let's go le traitement de la requête

    //Libération de la mémoire
    free(colonnes);

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

