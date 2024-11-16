#include "commands.h"
//On ignore les warnings pour les fonctions non déclarées parce que j'ai pas terminééééé
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wimplicit-function-declaration"
#pragma GCC diagnostic ignored "-Wunused-parameter"

//###############################################SELECT#####################################################
//Fonction get_table pour récupérer la table recherchée 
Table* get_table(BTree* btree, const char* table_name) {
    //Vérifier si l'arbre est vide
    if (btree == NULL || btree->root == NULL) {
        printf("L'arbre est vide.\n");
        return NULL;
    }
    //Récupérer la racine de l'arbre
    BTreeNode* current = btree->root;

    //Parcourir l'arbre pour trouver la table
    while (current != NULL) {
        //Parcourir les clés de l'arbre pour trouver la table correspondante
        for (int i = 0; i < current->keyCount; i++) {
            const char* current_table_name = current->keys[i];
            //Si le nom correspond, on retourne la table
            if (strcasecmp(table_name, current_table_name) == 0) {
                printf("Table '%s' trouvée!\n", table_name);
                return current->tables[i];
            }
        }

        //Si c'est une feuille, on sort de la boucle
        if (current->isLeaf) {
            break;
        }

        //On décidé de quel côté aller en fonction de la clé
        int i;
        for (i = 0; i < current->keyCount; i++) {
            const char* current_table_name = current->keys[i];
            //Si le nom est plus grand, on va à droite
            if (strcasecmp(table_name, current_table_name) < 0) {
                break;
            }
        }
        //
        current = current->children[i];
    }
    return NULL;
}

//Fonction pour la commande SELECT
char* select(BTree* btree, char* commande) {
    //Déclaration des variables
    char* colonnes = NULL;
    char* table_name = NULL;
    char* colonne_where = NULL;
    char* operateur_where = NULL;
    char* valeur_where = NULL;
    char* tmp = strtok(commande, " ");

    //Analyser la commande
    if (tmp != NULL) tmp = strtok(NULL, " ");
    if (tmp != NULL) {
        colonnes = strdup(tmp);
        while ((tmp = strtok(NULL, " ")) != NULL) {
            if (strcasecmp(tmp, "from") == 0) break;
            char* new_colonnes = realloc(colonnes, strlen(colonnes) + strlen(tmp) + 2);
            if (new_colonnes == NULL) {
                free(colonnes);
                return "Erreur : Allocation mémoire échouée";
            }
            colonnes = new_colonnes;
            strcat(colonnes, " ");
            strcat(colonnes, tmp);
        }
    }
    if (tmp != NULL) table_name = strtok(NULL, " ");
    if (colonnes == NULL || table_name == NULL) {
        free(colonnes);
        return "Erreur : Commande SELECT incorrecte";
    }

    //Vérifier la clause WHERE
    if (table_name != NULL) {
        tmp = strtok(NULL, " ");
        if (tmp != NULL && strcasecmp(tmp, "where") == 0) {
            colonne_where = strtok(NULL, " ");
            operateur_where = strtok(NULL, " ");
            valeur_where = strtok(NULL, " ");
            if (colonne_where == NULL || operateur_where == NULL || valeur_where == NULL) {
                free(colonnes);
                return "Erreur : Clause WHERE incorrecte";
            }
        }
    }

    //#######Recherche de commande#######
    //Trouver la table
    Table* table_select = get_table(btree, table_name);
    if (table_select == NULL) {
        free(colonnes);
        return "Erreur : Table inexistante";
    }

    //Afficher les colonnes sélectionnées
    for (int i = 0; i < table_select->columnCount; i++) {
        if (strstr(colonnes, table_select->columnNames[i]) != NULL || strcmp(colonnes, "*") == 0) {
            printf("%s ", table_select->columnNames[i]);
        }
    }
    printf("\n");

    //Afficher les données
    for (int i = 0; i < table_select->rowCount; i++) {
        int print_row = 1;
        if (colonne_where != NULL) {
            //Appliquer la condition WHERE
            for (int j = 0; j < table_select->columnCount; j++) {
                if (strcmp(table_select->columnNames[j], colonne_where) == 0) {
                    //Vérifier la condition = > <
                    if (strcmp(operateur_where, "=") == 0) {
                        print_row = (strcmp(table_select->rows[i].values[j], valeur_where) == 0);
                    } else if (strcmp(operateur_where, ">") == 0) {
                        print_row = (atoi(table_select->rows[i].values[j]) > atoi(valeur_where)); //atoi convertit une chaine de caractères en entier
                    } else if (strcmp(operateur_where, "<") == 0) {
                        print_row = (atoi(table_select->rows[i].values[j]) < atoi(valeur_where));
                    }else{
                        printf("Erreur : Opérateur inconnu\n");
                        //Libération de la mémoire
                        free(colonnes);
                        return "Sélection terminée";
                    }
                    break;
                }
            }
        }
        //Afficher la ligne si elle a passé la condition
        if (print_row) {
            for (int j = 0; j < table_select->columnCount; j++) {
                if (strstr(colonnes, table_select->columnNames[j]) != NULL || strcmp(colonnes, "*") == 0) {
                    printf("%s ", table_select->rows[i].values[j]);
                }
            }
            printf("\n");
        }
    }

    free(colonnes);
    return "Sélection terminée";
}

//Fonction principale pour gérer les commandes
void commands(BTree* btree) {
    char commande[100];
    char result[200];
    do {
        result[0] = '\0';
        if (btree == NULL) {
            printf("\033[1;31mErreur : Aucune base de données chargée\n\033[0m");
            return;
        }
        
        printf("\033[1;31mEntrez une commande (0 pour retourner en arrière): \033[0m");
        scanf(" %[^\n]", commande);

        printf("\033[1;32mCommande : %s\n\033[0m", commande);

        if (strncmp(commande, "select", 6) == 0) {
            strcpy(result, select(btree, commande));
        } else if (strcmp(commande, "insert") == 0) {
            printf("insert\n");
        } else if (strcmp(commande, "delete") == 0) {
            printf("delete\n");
        } else if (strcmp(commande, "update") == 0) {
            printf("update\n");
        } else if (strcmp(commande, "0") != 0) {
            printf("\033[1;31mCommande inconnue\n\033[0m");
        }

        if (result[0] != '\0') {
            printf("\033[1;32mResultat : %s\n\033[0m", result);
        }
        result[0] = '\0';
    } while (strcmp(commande, "0") != 0);
}