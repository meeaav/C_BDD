#include "save.h"

//Sauvegarder les changements effectués
void save(BTree* btree, char* filename) {
    if (btree == NULL) {
        printf("\033[1;31m");
        printf("Erreur : Arbre vide\n");
        printf("\033[0m");
        return;
    }
    //Enregistrer les changements dans un nouveau fichier situé dans BDD/
    char path[100] = "BDD/";
    strcat(path, filename);
    strcat(path, ".csv");
    FILE* file = fopen(path, "w");
    if (file == NULL) {
        printf("\033[1;31m");
        perror("Erreur : Impossible de créer le fichier. Mettez les droits d'écriture, ouverture et exec pour votre user sur le dossier s'il vous plait.\n");
        printf("\033[0m");
        return;
    }
    //Ecrire les données dans le fichier
    


}