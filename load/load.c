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

