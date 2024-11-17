#include "save.h"

//Sauvegarder les données d'un noeud
void save_node(BTreeNode* node, FILE* file) {
    if (node == NULL) {
        return;
    }
    for (int i = 0; i < node->keyCount; i++) {
        Table* table = node->tables[i];
        //Écrire le nom de la table
        fprintf(file, "#%s\n", table->name);
        //Écrire les noms des colonnes
        for (int j = 0; j < table->columnCount; j++) {
            fprintf(file, "%s", table->columnNames[j]);
            if (j < table->columnCount - 1) {
                fprintf(file, ",");
            }
        }
        fprintf(file, "\n");
        //Écrire les valeurs
        for (int j = 0; j < table->rowCount; j++) {
            for (int k = 0; k < table->columnCount; k++) {
                fprintf(file, "%s", table->rows[j].values[k]);
                if (k < table->columnCount - 1) {
                    fprintf(file, ",");
                }
            }
            fprintf(file, "\n");
        }
        fprintf(file, "\n"); //Ligne vide entre les tables
    }
    //Parcourir les enfants
    if (!node->isLeaf) {
        for (int i = 0; i <= node->keyCount; i++) {
            save_node(node->children[i], file);
        }
    }
}

//Sauvegarder les changements effectués
void save(BTree* btree, char* filename) {
    //Vérifier que l'arbre n'est pas vide
    if (btree == NULL || btree->root == NULL) {
        printf("\033[1;31mErreur : Arbre vide\n\033[0m");
        return;
    }

    //Créer le fichier
    char path[100] = "BDD/";
    strcat(path, filename);
    strcat(path, ".csv");
    //Tester si le fichier existe déjà
    FILE* file = fopen(path, "w");
    if (file == NULL) {
        printf("\033[1;31m");
        perror("Erreur : Impossible de créer le fichier");
        printf("\033[0m");
        return;
    }
    //Sauvegarder les données
    save_node(btree->root, file);

    //Fermer le fichier
    fclose(file);
    printf("\033[1;32mSauvegarde effectuée avec succès\n\033[0m");
}




