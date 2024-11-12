//Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Constantes que nous pouvons changer. 
Je les ai volontairement restreintes pour des raisons de simplicité*/
#define MAX_LINE_LENGTH 200
#define MAX_COLUMNS 10
#define MAX_TABLES 10
#define MAX_ROWS 100
#define MAX_KEY_LENGTH 50
#define MAX_NAME_LENGTH 100

// ########################## Structures ##########################
//Structure pour les lignes
typedef struct {
    char** values;
    int valueCount;
} Row;

//Structure pour les tables
typedef struct {
    char name[MAX_NAME_LENGTH];
    char** columnNames;
    int columnCount;
    Row* rows;
    int rowCount;
} Table;

//Structure pour les noeuds de mon btree
typedef struct BTreeNode {
    char** keys;
    Table** tables;
    struct BTreeNode** children;
    int keyCount;
    int isLeaf;
} BTreeNode;

//Structure pour mon btree
typedef struct {
    BTreeNode* root;
} BTree;

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

    //###################################PAUSE###################################
    /* TODO
     allc mémoire pour les tables et les enfants
     + verif si leaf ou pas
    */


}

/*TODO
fonction pour creer mon btrree avec alloc mémoire pour le root et creation du noeud
*/


/*TODO
fonction pour insérer une table dans le btree
 recupérer le root	vrifier si full dans ce cas split et update root sinon insert dans le noeud
*/

/*TODO
fonction pour split un noeud ou alors l'implémenter dans insertTable direc 
*/

//Fonction principale pour charger la base de donnée
void loadBDD(char *bddChoisie) {
    printf("Chargement de la base de donnée %s\n", bddChoisie);

    //Ouverture du fichier (après reflexion j'aurais pu faire une fonction pour ouvrir le fichier)
    FILE *fichier = NULL;
    char chemin[100] = "BDD/";
    strcat(chemin, bddChoisie);
    fichier = fopen(chemin, "r");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }


    /* TODO
     Gros chantier, coeur du Tree

     Initialisation var 

     Lire le csv ligne paer ligne et diriger en fonction
     ne pas oublier de check les #
        1er ligne = nom des tables
        2eme ligne = nom des colonnes
    
    Test : affichage

    ++++++ : reflechir à une opt affichage ? comme exercice année dernière de combat.
    */

}