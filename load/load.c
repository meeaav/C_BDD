#include "load.h"

// ########################## Fonctions ##########################
//Fonction pour créer une table
Table* createTable(const char* name, int columnCount, char** columnNames) {
    //Dans l'ordre, on va allouer de la mémoire pour la table, les colonnes puis les lignes
    Table* table = (Table*)malloc(sizeof(Table)); //Alloc mémoire
    strncpy(table->name, name, MAX_NAME_LENGTH - 1); //Copie l e nom de la table
    table->name[MAX_NAME_LENGTH - 1] = '\0'; //Ajoute un caractère de fin de ligne
    table->columnCount = columnCount; //Nombre de colonnes
    table->columnNames = (char**)malloc(columnCount * sizeof(char*)); //Alloc mémoire pour les noms des colonnes
    for (int i = 0; i < columnCount; i++) { //Boucle pour copier les noms des colonnes
        table->columnNames[i] = (char*)malloc(MAX_NAME_LENGTH * sizeof(char)); //Alloc mémoire
        strncpy(table->columnNames[i], columnNames[i], MAX_NAME_LENGTH - 1); //Copie du nom
        table->columnNames[i][MAX_NAME_LENGTH - 1] = '\0'; //Ajout d'un caractère de fin de ligne
    }
    table->rows = (Row*)malloc(MAX_ROWS * sizeof(Row)); //Alloc mémoire pour nos lignes
    table->rowCount = 0;
    return table; //Retourne la table
}

//Fonction pour insérer une ligne dans une table
void insertIntoTable(Table* table, char** values) {
    if (table->rowCount >= MAX_ROWS) { //Si la table est pleine, on arrête et on le signale à l'utilisateur
        printf("Table is full\n");
        return;
    }

    //Sinon, on insère la ligne (comme pour les colonnes)
    Row* row = &table->rows[table->rowCount]; //On récupère la ligne
    row->values = (char**)malloc(table->columnCount * sizeof(char*)); //Alloc mémoire pour les valeurs
    row->valueCount = table->columnCount; //Nombre de valeurs
    for (int i = 0; i < table->columnCount; i++) { //Boucle pour copier les valeurs
        row->values[i] = (char*)malloc(MAX_NAME_LENGTH * sizeof(char)); //Alloc mémoire
        strncpy(row->values[i], values[i], MAX_NAME_LENGTH - 1); //Copie de la valeur
        row->values[i][MAX_NAME_LENGTH - 1] = '\0'; //Ajout d'un caractère de fin de ligne
    }
    table->rowCount++; //Incrémentation du nombre de lignes pour passer à la suivante
}

//Fonction pour créer un noeud
BTreeNode* createNode(int isLeaf) {
    //On alloue de la mémoire pour le noeud, les clés, les tables et les enfants
    BTreeNode* node = (BTreeNode*)malloc(sizeof(BTreeNode)); //Alloc mémoire
    node->keys = (char**)malloc(MAX_TABLES * sizeof(char*)); //Alloc mémoire pour les clés
    for (int i = 0; i < MAX_TABLES; i++) { //Boucle pour copier les clés, qui sont nos noms de tables
        node->keys[i] = (char*)malloc(MAX_KEY_LENGTH * sizeof(char)); //Alloc mémoire
    }
    node->tables = (Table**)malloc(MAX_TABLES * sizeof(Table*)); //Alloc mémoire pour les tables
    node->children = (BTreeNode**)malloc((MAX_TABLES + 1) * sizeof(BTreeNode*)); //Alloc mémoire pour les enfants
    node->keyCount = 0;
    node->isLeaf = isLeaf; //On indique si c'est une feuille ou non
    return node;
}

//Fonction pour créer un btree
BTree* createBTree() {
    //On alloue de la mémoire pour le btree et on crée un noeud
    BTree* tree = (BTree*)malloc(sizeof(BTree));
    tree->root = createNode(1);
    return tree;
}

//Fonction pour insérer dans le btree
void insertIntoBTree(BTree* tree, const char* key, Table* table) {
    //On récupère la racine du btree et on vérifie si elle est pleine
    BTreeNode* root = tree->root;
    if (root->keyCount == MAX_TABLES - 1) {
        //Si elle est pleine, on crée un nouveau noeud et on le met en racine
        BTreeNode* newRoot = createNode(0);
        newRoot->children[0] = root;
        tree->root = newRoot;
        strncpy(newRoot->keys[0], key, MAX_KEY_LENGTH - 1);
        newRoot->keys[0][MAX_KEY_LENGTH - 1] = '\0';
        newRoot->tables[0] = table;
        newRoot->keyCount++;
    } else {
        //Sinon, on inssère la table dans la racine actuelle en décalant les clés
        int i;
        for (i = root->keyCount - 1; i >= 0 && strcmp(key, root->keys[i]) < 0; i--) {
            strncpy(root->keys[i + 1], root->keys[i], MAX_KEY_LENGTH - 1); //On décale les clés
            root->keys[i + 1][MAX_KEY_LENGTH - 1] = '\0'; //Ajout d'un caractère de fin de ligne
            root->tables[i + 1] = root->tables[i]; //On décale les tables
        }

        //Ensuite on insère la clé et la table
        strncpy(root->keys[i + 1], key, MAX_KEY_LENGTH - 1); //On insère la clé
        root->keys[i + 1][MAX_KEY_LENGTH - 1] = '\0'; //On ajoute encore un caractère de fin de ligne pour signifier la fin de la clé
        root->tables[i + 1] = table; //On insère la table
        root->keyCount++; //On incrémente le nombre de clés pour passer à la suivante
    }
}

//Fonction principale pour charger la base de donnée
BTree* loadBDD(char *bddChoisie) {
    //printf("Chargement de la base de donnée %s\n", bddChoisie);

    //Ouverture du fichier (après reflexion j'aurais pu faire une fonction pour ouvrir le fichier)
    FILE *fichier = NULL;
    char chemin[100] = "BDD/";
    strcat(chemin, bddChoisie);
    fichier = fopen(chemin, "r");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return NULL;
    }


    //########## Lecture du fichier et chargement de la base de donnée##########

    //Initialisation des variables
    char line[MAX_LINE_LENGTH];
    char *token;
    char *columnNames[MAX_COLUMNS];
    int columnCount = 0;
    Table *currentTable = NULL;
    BTree *chosedDBTree = createBTree();
    
    //Boucle pour lire le fichier ligne par ligne
    while (fgets(line, sizeof(line), fichier)) {
        line[strcspn(line, "\n")] = 0;

        if (line[0] == '#') { 
            //Si la ligne commence par un #, c'est le nom de la table
            if (currentTable != NULL) {
                insertIntoBTree(chosedDBTree, currentTable->name, currentTable); //On insère la table dans le btree
            }
            char tableName[MAX_NAME_LENGTH]; //On récupère le nom de la table
            strncpy(tableName, line + 1, MAX_NAME_LENGTH - 1); // On copie le nom de la table
            tableName[MAX_NAME_LENGTH - 1] = '\0'; //On ajoute toujours un caractère de fin de ligne
            currentTable = createTable(tableName, 0, NULL); //On crée la table
            columnCount = 0; //On remet le compteur de colonnes à 0
            //-->On a rempli le nom de la table

        } else if (currentTable != NULL) { //Sinon, on lit les colonnes et les valeurs
            if (columnCount == 0) {
                //Si le compteur de colonnes est à 0, on lit les colonnes
                token = strtok(line, ",");
                while (token != NULL && columnCount < MAX_COLUMNS) {
                    //On lit les colonnes et on les copie
                    columnNames[columnCount] = (char*)malloc(MAX_NAME_LENGTH * sizeof(char)); //Alloc mémoire
                    strncpy(columnNames[columnCount], token, MAX_NAME_LENGTH - 1); //Copiage
                    columnNames[columnCount][MAX_NAME_LENGTH - 1] = '\0'; //Le caractère de fin de ligne
                    columnCount++; //Incrémentation pour passer à la suivante
                    token = strtok(NULL, ","); 
                }

                //On crée la table avec les colonnes
                Table* newTable = createTable(currentTable->name, columnCount, columnNames); //On crée la table
                free(currentTable); //On libère la mémoire de l'ancienne table
                currentTable = newTable; //On met la nouvelle table à la place de l'ancienne
                for (int i = 0; i < columnCount; i++) {
                    //On libère la mémoire des colonnes
                    free(columnNames[i]);
                }
                //-->On a rempli les colonnes  

            } else {
                //Sinon, on lit les valeurs
                char *values[MAX_COLUMNS];
                int valueCount = 0;
                token = strtok(line, ","); 
                while (token != NULL && valueCount < columnCount) {
                    //On lit les valeurs et on les copie
                    values[valueCount] = token; //Copiage de la valeur
                    valueCount++; //Incrémentation
                    token = strtok(NULL, ",");
                }

                //On insère les valeurs dans la table
                insertIntoTable(currentTable, values); 

                //-->On a rempli les valeurs
            }
        }
    }

    //On insère la dernière table dans le btree
    if (currentTable != NULL) {
        insertIntoBTree(chosedDBTree, currentTable->name, currentTable);
    }

    //Fermeture du fichier
    fclose(fichier);
    //printf("Chargement terminé.\n");


    /*TEST AFFICHAGE
    BTreeNode* root = chosedDBTree->root;
    for (int i = 0; i < root->keyCount; i++) {
        Table* table = root->tables[i];
        printf("Table: %s\n", table->name);
        printf("Colonnes: ");
        for (int j = 0; j < table->columnCount; j++) {
            printf("%s ", table->columnNames[j]);
        }
        printf("\n");
        printf("Nombre de lignes: %d\n\n", table->rowCount);
    }*/

    return chosedDBTree;
}