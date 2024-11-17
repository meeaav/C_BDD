#include "commands.h"

//Fonction getTableInBtree pour récupérer la table recherchée 
Table* getTableInBtree(BTree* btree, const char* tableName) {
    //Vérifier si l'arbre est vide
    if (btree == NULL || btree->root == NULL) {
        printf("\033[1;31mErreur : Arbre vide\n\033[0m");
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


//Fonction  pour libérer la mémoire d'une table
void freeTable(Table* table) {
    if (table == NULL) return;

    //Libérer les noms des colonnes
    for (int i = 0; i < table->columnCount; i++) {
        free(table->columnNames[i]);
    }
    free(table->columnNames);

    //Libérer les lignes
    for (int i = 0; i < table->rowCount; i++) {
        for (int j = 0; j < table->columnCount; j++) {
            free(table->rows[i].values[j]);
        }
        free(table->rows[i].values);
    }
    free(table->rows);

    //Libérer la structure de la table
    free(table);
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
    int resultFound = 0;  // Variable pour vérifier si des résultats ont été trouvés

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

    //#######Recherche de commande#######
    //Trouver la table
    Table* tableSelect = getTableInBtree(btree, tableName);
    if (tableSelect == NULL) {
        free(columns);
        return "Erreur : Table inexistante";
    }

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
                        rowToPrint = (atoi(tableSelect->rows[i].values[j]) > atoi(whereValue));
                    } else if (strcmp(whereOperator, "<") == 0) {
                        rowToPrint = (atoi(tableSelect->rows[i].values[j]) < atoi(whereValue));
                    } else {
                        printf("\033[1;31mErreur : Opérateur inconnu\n\033[0m");
                        free(columns);
                        return "Sélection terminée avec erreur";
                    }
                    break;
                }
            }
        }
        //Afficher la ligne si elle a passé la condition
        if (rowToPrint) {
            resultFound = 1;  // Marquer qu'un résultat a été trouvé
            for (int j = 0; j < tableSelect->columnCount; j++) {
                if (strstr(columns, tableSelect->columnNames[j]) != NULL || strcmp(columns, "*") == 0) {
                    printf("%s ", tableSelect->rows[i].values[j]);
                }
            }
            printf("\n");
        }
    }

    free(columns);

    if (!resultFound) {
        return "Aucun résultat trouvé";
    }

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
        return strdup("Erreur : Commande DELETE incorrecte"); //strdup = malloc + strcpy
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
    int deletedCpt = 0;
    int columnIndex = -1;

    //Trouver l'index de la colonne pour la condition WHERE
    if (whereColumn != NULL) {
        for (int i = 0; i < tableDelete->columnCount; i++) {
            if (strcasecmp(tableDelete->columnNames[i], whereColumn) == 0) {
                columnIndex = i;
                break;
            }
        }
        if (columnIndex == -1) {
            return strdup("Erreur : Colonne inexistante");
        }
    }

    //Parcourir les lignes et supprimer celles qui correspondent à la condition
    for (int i = 0; i < tableDelete->rowCount; i++) {
        int contentToDelete = (whereColumn == NULL); //Si pas de WHERE, on supprime tout

        if (whereColumn != NULL) {
            char* cellValue = tableDelete->rows[i].values[columnIndex];
            if (strcmp(whereOperator, "=") == 0) {
                contentToDelete = (strcmp(cellValue, whereValue) == 0);
            } else if (strcmp(whereOperator, ">") == 0) {
                contentToDelete = (atoi(cellValue) > atoi(whereValue)); //atoi convertit une chaine de caractères en entier
            } else if (strcmp(whereOperator, "<") == 0) {
                contentToDelete = (atoi(cellValue) < atoi(whereValue));
            } else {
                return strdup("Erreur : Opérateur inconnu");
            }
        }

        if (contentToDelete) {
            //Supprimer la ligne en décalant toutes les lignes suivantes
            for (int j = i; j < tableDelete->rowCount - 1; j++) {
                tableDelete->rows[j] = tableDelete->rows[j + 1];
            }
            tableDelete->rowCount--;
            i--; //Reculer d'un indice car on a décalé les lignes
            deletedCpt++;
        }
    }

    //Sauvegarder les modifications dans result
    char* result = malloc(100 * sizeof(char));
    if (result == NULL) {
        return strdup("Erreur : Allocation mémoire échouée");
    }

    //Afficher le nombre de lignes supprimées
    snprintf(result, 100, "%d ligne(s) supprimée(s) de %s", deletedCpt, tableName); //snprintf permet de formater une chaine de caractères avec un nombre limité de caractères (zinon ce serait vraiment trop long...)
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
    int valuesFound = 0;
    while ((tmp = strtok(NULL, " ")) != NULL) {
        if (strcasecmp(tmp, "values") == 0) {
            valuesFound = 1;
            continue;
        }
        if (valuesFound) {
            values[valueCount] = tmp;
            valueCount++;
        }
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
    //Déclaration des variables
    char* tableName = NULL;
    char* setColumn = NULL;
    char* setValue = NULL;
    char* whereColumn = NULL;
    char* whereOperator = NULL;
    char* whereValue = NULL;
    char* tmp = strtok(commande, " ");

    //Analyser la commande 
    if (tmp != NULL) tableName = strtok(NULL, " "); //Nom de la table
    if (tmp != NULL) tmp = strtok(NULL, " "); //"set"
    if (tmp != NULL && strcasecmp(tmp, "set") == 0) {
        setColumn = strtok(NULL, " ");
        tmp = strtok(NULL, " "); //"="
        if (tmp != NULL && strcmp(tmp, "=") == 0) {
            setValue = strtok(NULL, " ");
        }
    }
    tmp = strtok(NULL, " "); //"where"
    if (tmp != NULL && strcasecmp(tmp, "where") == 0) {
        whereColumn = strtok(NULL, " ");
        whereOperator = strtok(NULL, " ");
        whereValue = strtok(NULL, " ");
    }


    //Vérifier la validité de la commande, si tous les paramètres sont présents
    if (tableName == NULL || setColumn == NULL || setValue == NULL || 
        whereColumn == NULL || whereOperator == NULL || whereValue == NULL) {
        return strdup("Erreur : Commande UPDATE incorrecte");
    }

    //Trouver la table
    Table* tableUpdate = getTableInBtree(btree, tableName);
    if (tableUpdate == NULL) {
        return strdup("Erreur : Table inexistante");
    }

    //Trouver l'index de la colonne à mettre à jour
    int columnIndex = -1;
    for (int i = 0; i < tableUpdate->columnCount; i++) {
        if (strcasecmp(tableUpdate->columnNames[i], setColumn) == 0) {
            columnIndex = i;
            break;
        }
    }
    if (columnIndex == -1) {
        return strdup("Erreur : Colonne à mettre à jour inexistante");
    }

    //Trouver l'index de la colonne pour la condition WHERE
    int whereColumnIndex = -1;
    for (int i = 0; i < tableUpdate->columnCount; i++) {
        if (strcasecmp(tableUpdate->columnNames[i], whereColumn) == 0) {
            whereColumnIndex = i;
            break;
        }
    }
    if (whereColumnIndex == -1) {
        return strdup("Erreur : Colonne WHERE inexistante");
    }

    //Parcourir les lignes et mettre à jour celles qui correspondent à la condition
    int updatedCount = 0;
    for (int i = 0; i < tableUpdate->rowCount; i++) {
        char* cellValue = tableUpdate->rows[i].values[whereColumnIndex];
        if (strcmp(whereOperator, "=") == 0) {
            if (strcmp(cellValue, whereValue) == 0) {
                strcpy(tableUpdate->rows[i].values[columnIndex], setValue);
                updatedCount++;
            }
        } else if (strcmp(whereOperator, ">") == 0) {
            if (atoi(cellValue) > atoi(whereValue)) {
                strcpy(tableUpdate->rows[i].values[columnIndex], setValue);
                updatedCount++;
            }
        } else if (strcmp(whereOperator, "<") == 0) {
            if (atoi(cellValue) < atoi(whereValue)) {
                strcpy(tableUpdate->rows[i].values[columnIndex], setValue);
                updatedCount++;
            }
        } else {
            return strdup("Erreur : Opérateur inconnu");
        }
    }

    //Préparer le message de résultat
    char* result = malloc(100 * sizeof(char));
    if (result == NULL) {
        return strdup("Erreur : Allocation mémoire échouée");
    }
    snprintf(result, 100, "%d ligne(s) mise(s) à jour dans la table %s", updatedCount, tableName);
    return result;
}

//###############################################DROP TABLE#####################################################
//Fonction pour supprimer une table
char* dropTable(BTree* btree, char* commande) {
    //Déclaration des variables
    char* tableName = NULL;
    char* tmp = strtok(commande, " ");

    //Analyser la commande
    if (tmp != NULL) tmp = strtok(NULL, " "); //Sauter "drop"
    if (tmp != NULL && strcasecmp(tmp, "table") == 0) {
        tableName = strtok(NULL, " ");
    }
    if (tableName == NULL) {
        return strdup("Erreur : Commande DROP TABLE incorrecte");
    }

    //Trouver la table
    Table* tableDrop = getTableInBtree(btree, tableName);
    if (tableDrop == NULL) {
        return strdup("Erreur : Table inexistante");
    }

    //Fonction pour supprimer la table du nœud
    int removeTableFromNode(BTreeNode* node) {
        for (int i = 0; i < node->keyCount; i++) {
            if (strcasecmp(node->keys[i], tableName) == 0) {
                //Libérer la mémoire de la table
                freeTable(node->tables[i]);

                //Décaler les clés et tables restantes
                for (int j = i; j < node->keyCount - 1; j++) {
                    node->keys[j] = node->keys[j + 1];
                    node->tables[j] = node->tables[j + 1];
                }
                node->keyCount--;

                return 1; //Table trouvée et supprimée
            }
        }

        //Si ce n'est pas une feuille, chercher dans les enfants
        if (!node->isLeaf) {
            for (int i = 0; i <= node->keyCount; i++) {
                if (removeTableFromNode(node->children[i])) {
                    return 1;
                }
            }
        }

        return 0; //Table non trouvée
    }

    //Supprimer la table de l'arbre
    if (removeTableFromNode(btree->root)) {
        //Si la racine est vide après la suppression, ajuster l'arbre
        if (btree->root->keyCount == 0 && !btree->root->isLeaf) {
            BTreeNode* oldRoot = btree->root;
            btree->root = btree->root->children[0];
            free(oldRoot);
        }
        return strdup("Table supprimée avec succès");
    } else {
        return strdup("Erreur : Impossible de supprimer la table de l'arbre B");
    }
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
            char* deletResult = delete(btree, commande);
            if (deletResult != NULL) {
                strcpy(result, deletResult);
                free(deletResult);
            } else {
                strcpy(result, "Erreur lors de la suppression");
            }
        } else if (strncmp(commande, "update", 6) == 0) {
            strcpy(result, update(btree, commande));
        }else if (strncmp(commande, "drop table", 10) == 0) {
        char* dropResult = dropTable(btree, commande);
        if (dropResult != NULL) {
                strcpy(result, dropResult);
                free(dropResult);
        } else {
                strcpy(result, "Erreur lors de la suppression de la table");
            }
        } else if (strcmp(commande, "0") != 0) {
            printf("\033[1;31mCommande inconnue\n\033[0m");
        } 
        if (result[0] != '\0') {
            printf("\033[1;32mResultat : %s\n\033[0m", result);
        }
        result[0] = '\0';
    } while (strcmp(commande, "0") != 0);
}