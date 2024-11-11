#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int nb_of_tables(char *bddChoisie) {
    FILE *fichier = NULL;
    char chemin[100] = "BDD/";
    strcat(chemin, bddChoisie);
    fichier = fopen(chemin, "r");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return 0;
    }
    char c;
    int count = 0;
    while ((c = fgetc(fichier)) != EOF) {
        if (c == '#') {
            count++;
        }
    }
    fclose(fichier);
    return count;
}

void loadBDD(char *bddChoisie) {
    printf("Chargement de la base de donnée %s\n", bddChoisie);
    int nb_tables = nb_of_tables(bddChoisie);

}