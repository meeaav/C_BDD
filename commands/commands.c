#include "commands.h"
//On ignore les warnings pour les fonctions non déclarées parce que j'ai pas terminééééé
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wimplicit-function-declaration"
#pragma GCC diagnostic ignored "-Wunused-parameter"

//Fonction getTableInBtree pour récupérer la table recherchée 
Table* getTableInBtree(BTree* btree, const char* tableName) {
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
            const char* currentTableName = current->keys[i];
            //Si le nom correspond, on retourne la table
            if (strcasecmp(tableName, currentTableName) == 0) {
                printf("Table '%s' trouvée!\n", tableName);
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
            const char* currentTableName = current->keys[i];
            //Si le nom est plus grand, on va à droite
            if (strcasecmp(tableName, currentTableName) < 0) {
                break;
            }
        }
        //
        current = current->children[i];
    }
    return NULL;
}

//###############################################SELECT#####################################################
char* select(BTree* btree, char* commande) {
    //Déclaration des variables
    char* columns = NULL;
    char* tableName = NULL;
    char* whereColumn = NULL;
    char* whereOperator = NULL;
    char* whereValue = NULL;
    char* tmp = strtok(commande, " ");

    //Analyser la commande
    if (tmp != NULL) tmp = strtok(NULL, " ");
    if (tmp != NULL) {
        columns = strdup(tmp);
        while ((tmp = strtok(NULL, " ")) != NULL) {
            if (strcasecmp(tmp, "from") == 0) break;
            char* newColumns = realloc(columns, strlen(columns) + strlen(tmp) + 2);
            if (newColumns == NULL) {
                free(columns);
                return "Erreur : Allocation mémoire échouée";
            }
            columns = newColumns;
            strcat(columns, " ");
            strcat(columns, tmp);
        }
    }
    if (tmp != NULL) tableName = strtok(NULL, " ");
    if (columns == NULL || tableName == NULL) {
        free(columns);
        return "Erreur : Commande SELECT incorrecte";
    }

    //Vérifier la clause WHERE
    if (tableName != NULL) {
        tmp = strtok(NULL, " ");
        if (tmp != NULL && strcasecmp(tmp, "where") == 0) {
            whereColumn = strtok(NULL, " ");
            whereOperator = strtok(NULL, " ");
            whereValue = strtok(NULL, " ");
            if (whereColumn == NULL || whereOperator == NULL || whereValue == NULL) {
                free(columns);
                return "Erreur : Clause WHERE incorrecte";
            }
        }
    }

    //#######Recherche de commande#######
    //Trouver la table
    Table* tableSelect = getTableInBtree(btree, tableName);
    if (tableSelect == NULL) {
        free(columns);
        return "Erreur : Table inexistante";
    }

    //Afficher les columns sélectionnées
    for (int i = 0; i < tableSelect->columnCount; i++) {
        if (strstr(columns, tableSelect->columnNames[i]) != NULL || strcmp(columns, "*") == 0) {
            printf("%s ", tableSelect->columnNames[i]);
        }
    }
    printf("\n");

    //Afficher les données
    for (int i = 0; i < tableSelect->rowCount; i++) {
        int rowToPrint = 1;
        if (whereColumn != NULL) {
            //Appliquer la condition WHERE
            for (int j = 0; j < tableSelect->columnCount; j++) {
                if (strcmp(tableSelect->columnNames[j], whereColumn) == 0) {
                    //Vérifier la condition = > <
                    if (strcmp(whereOperator, "=") == 0) {
                        rowToPrint = (strcmp(tableSelect->rows[i].values[j], whereValue) == 0);
                    } else if (strcmp(whereOperator, ">") == 0) {
                        rowToPrint = (atoi(tableSelect->rows[i].values[j]) > atoi(whereValue)); //atoi convertit une chaine de caractères en entier
                    } else if (strcmp(whereOperator, "<") == 0) {
                        rowToPrint = (atoi(tableSelect->rows[i].values[j]) < atoi(whereValue));
                    }else{
                        printf("Erreur : Opérateur inconnu\n");
                        //Libération de la mémoire
                        free(columns);
                        return "Sélection terminée";
                    }
                    break;
                }
            }
        }
        //Afficher la ligne si elle a passé la condition
        if (rowToPrint) {
            for (int j = 0; j < tableSelect->columnCount; j++) {
                if (strstr(columns, tableSelect->columnNames[j]) != NULL || strcmp(columns, "*") == 0) {
                    printf("%s ", tableSelect->rows[i].values[j]);
                }
            }
            printf("\n");
        }
    }

    free(columns);
    return "Sélection terminée";
}
//###############################################DELETE#####################################################
char* delete(BTree* btree, char* commande) {
    //Déclaration des variables
    char* tableName = NULL;
    char* whereColumn = NULL;
    char* whereOperator = NULL;
    char* whereValue = NULL;
    char* tmp = strtok(commande, " ");

    //Analyser la commande
    if (tmp != NULL) tmp = strtok(NULL, " "); //Sauter "delete"
    if (tmp != NULL && strcmp(tmp, "*") == 0) tmp = strtok(NULL, " "); //Sauter "*" si présent
    if (tmp != NULL && strcasecmp(tmp, "from") == 0) {
        tableName = strtok(NULL, " ");
    }
    
    //Si pas de table, retourner une erreur
    if (tableName == NULL) {
        return strdup("Erreur : Commande DELETE incorrecte");
    }

    //Vérifier la clause WHERE
    tmp = strtok(NULL, " ");
    if (tmp != NULL && strcasecmp(tmp, "where") == 0) {
        whereColumn = strtok(NULL, " ");
        whereOperator = strtok(NULL, " ");
        whereValue = strtok(NULL, " ");
        if (whereColumn == NULL || whereOperator == NULL || whereValue == NULL) {
            return strdup("Erreur : Clause WHERE incorrecte");
        }
    }

    //Trouver la table
    Table* tableDelete = getTableInBtree(btree, tableName);
    if (tableDelete == NULL) {
        return strdup("Erreur : Table inexistante");
    }

    //##Recherche de commande##

    //Compteur du nombre de lignes supprimées
    int deleted_cpt = 0;
    int column_index = -1;

    //Trouver l'index de la colonne pour la condition WHERE
    if (whereColumn != NULL) {
        for (int i = 0; i < tableDelete->columnCount; i++) {
            if (strcasecmp(tableDelete->columnNames[i], whereColumn) == 0) {
                column_index = i;
                break;
            }
        }
        if (column_index == -1) {
            return strdup("Erreur : Colonne inexistante");
        }
    }

    //Parcourir les lignes et supprimer celles qui correspondent à la condition
    for (int i = 0; i < tableDelete->rowCount; i++) {
        int content_to_delete = (whereColumn == NULL); //Si pas de WHERE, on supprime tout

        if (whereColumn != NULL) {
            char* cell_value = tableDelete->rows[i].values[column_index];
            if (strcmp(whereOperator, "=") == 0) {
                content_to_delete = (strcmp(cell_value, whereValue) == 0);
            } else if (strcmp(whereOperator, ">") == 0) {
                content_to_delete = (atoi(cell_value) > atoi(whereValue));
            } else if (strcmp(whereOperator, "<") == 0) {
                content_to_delete = (atoi(cell_value) < atoi(whereValue));
            } else {
                return strdup("Erreur : Opérateur inconnu");
            }
        }

        if (content_to_delete) {
            //Supprimer la ligne en décalant toutes les lignes suivantes
            for (int j = i; j < tableDelete->rowCount - 1; j++) {
                tableDelete->rows[j] = tableDelete->rows[j + 1];
            }
            tableDelete->rowCount--;
            i--; //Reculer d'un indice car on a décalé les lignes
            deleted_cpt++;
        }
    }

    //Sauvegarder les modifications dans result
    char* result = malloc(100 * sizeof(char));
    if (result == NULL) {
        return strdup("Erreur : Allocation mémoire échouée");
    }

    //Afficher le nombre de lignes supprimées
    snprintf(result, 100, "%d ligne(s) supprimée(s) de %s", deleted_cpt, tableName); //snprintf permet de formater une chaine de caractères avec un nombre limité de caractères (zinon ce serait vraiment trop long...)
    return result; 
}

//###############################################INSERT#####################################################
//Fonction pour insérer une ligne dans une table
char* insert(BTree* btree, char* commande) {
    //Déclaration des variables
    char* tableName = NULL;
    char* values[MAX_COLUMNS];
    char* tmp = strtok(commande, " ");

    //Analyser la commande
    if (tmp != NULL) tmp = strtok(NULL, " "); //Sauter "insert"
    if (tmp != NULL && strcasecmp(tmp, "into") == 0) {
        tableName = strtok(NULL, " ");
    }
    if (tableName == NULL) {
        return strdup("Erreur : Commande INSERT incorrecte");
    }

    //Trouver la table
    Table* tableInsert = getTableInBtree(btree, tableName);
    if (tableInsert == NULL) {
        return strdup("Erreur : Table inexistante");
    }

    //Lire les valeurs à insérer
    int valueCount = 0;
    while ((tmp = strtok(NULL, " ")) != NULL) {
        if (strcasecmp(tmp, "values") == 0) break;
        values[valueCount] = tmp;
        valueCount++;
    }

    //Vérifier le nombre de valeurs
    if (valueCount != tableInsert->columnCount) {
        return strdup("Erreur : Nombre de valeurs incorrect");
    }

    //Insérer les valeurs dans la table
    insertIntoTable(tableInsert, values);

    return strdup("Insertion terminée");
}

//###############################################UPDATE#####################################################
//Fonction pour mettre à jour une ligne dans une table
char* update(BTree* btree, char* commande) {
    char* tableName = NULL;
    char* setColumn = NULL;
    char* setValue = NULL;
    char* whereColumn = NULL;
    char* whereOperator = NULL;
    char* whereValue = NULL;
    char* tmp = strtok(commande, " ");

    // Analyser la commande
    if (tmp != NULL) tableName = strtok(NULL, " "); // Nom de la table
    if (tmp != NULL) tmp = strtok(NULL, " "); // "set"
    if (tmp != NULL && strcasecmp(tmp, "set") == 0) {
        setColumn = strtok(NULL, " ");
        tmp = strtok(NULL, " "); // "="
        if (tmp != NULL && strcmp(tmp, "=") == 0) {
            setValue = strtok(NULL, " ");
        }
    }
    tmp = strtok(NULL, " "); // "where"
    if (tmp != NULL && strcasecmp(tmp, "where") == 0) {
        whereColumn = strtok(NULL, " ");
        whereOperator = strtok(NULL, " ");
        whereValue = strtok(NULL, " ");
    }

    if (tableName == NULL || setColumn == NULL || setValue == NULL || 
        whereColumn == NULL || whereOperator == NULL || whereValue == NULL) {
        return strdup("Erreur : Commande UPDATE incorrecte");
    }

    // Trouver la table
    Table* tableUpdate = getTableInBtree(btree, tableName);
    if (tableUpdate == NULL) {
        return strdup("Erreur : Table inexistante");
    }

    // Trouver l'index de la colonne à mettre à jour
    int column_index = -1;
    for (int i = 0; i < tableUpdate->columnCount; i++) {
        if (strcasecmp(tableUpdate->columnNames[i], setColumn) == 0) {
            column_index = i;
            break;
        }
    }
    if (column_index == -1) {
        return strdup("Erreur : Colonne à mettre à jour inexistante");
    }

    // Trouver l'index de la colonne pour la condition WHERE
    int where_column_index = -1;
    for (int i = 0; i < tableUpdate->columnCount; i++) {
        if (strcasecmp(tableUpdate->columnNames[i], whereColumn) == 0) {
            where_column_index = i;
            break;
        }
    }
    if (where_column_index == -1) {
        return strdup("Erreur : Colonne WHERE inexistante");
    }

    // Parcourir les lignes et mettre à jour celles qui correspondent à la condition
    int updated_count = 0;
    for (int i = 0; i < tableUpdate->rowCount; i++) {
        char* cell_value = tableUpdate->rows[i].values[where_column_index];
        if (strcmp(whereOperator, "=") == 0) {
            if (strcmp(cell_value, whereValue) == 0) {
                strcpy(tableUpdate->rows[i].values[column_index], setValue);
                updated_count++;
            }
        } else if (strcmp(whereOperator, ">") == 0) {
            if (atoi(cell_value) > atoi(whereValue)) {
                strcpy(tableUpdate->rows[i].values[column_index], setValue);
                updated_count++;
            }
        } else if (strcmp(whereOperator, "<") == 0) {
            if (atoi(cell_value) < atoi(whereValue)) {
                strcpy(tableUpdate->rows[i].values[column_index], setValue);
                updated_count++;
            }
        } else {
            return strdup("Erreur : Opérateur inconnu");
        }
    }

    // Préparer le message de résultat
    char* result = malloc(100 * sizeof(char));
    if (result == NULL) {
        return strdup("Erreur : Allocation mémoire échouée");
    }
    snprintf(result, 100, "%d ligne(s) mise(s) à jour dans la table %s", updated_count, tableName);
    return result;
}


//###############################################Prog Principal#####################################################
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
        } else if (strncmp(commande, "insert", 6) == 0) {
            strcpy(result, insert(btree, commande));
        } else if (strncmp(commande, "delete",6) == 0) {
            char* delete_result = delete(btree, commande);
            if (delete_result != NULL) {
                strcpy(result, delete_result);
                free(delete_result);
            } else {
                strcpy(result, "Erreur lors de la suppression");
            }
        } else if (strncmp(commande, "update", 6) == 0) {
            strcpy(result, update(btree, commande));
        } else if (strcmp(commande, "0") != 0) {
            printf("\033[1;31mCommande inconnue\n\033[0m");
        }

        if (result[0] != '\0') {
            printf("\033[1;32mResultat : %s\n\033[0m", result);
        }
        result[0] = '\0';
    } while (strcmp(commande, "0") != 0);
}