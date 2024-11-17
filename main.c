/* Librairies */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>  /*Pour opendir(), readdir(), closedir()*/
#include "menu/menu.h" /*Pour fonction d'affichage du menu*/
#include "load/load.h" /*Pour fonction de chargement de la BDD*/
#include "commands/commands.h" /*Pour fonction de commande SQL*/


/*Fonction pour scanner les BDD dans le répertoire BDD (.csv)*/
int scan_bdd(char *listeBDD[], int max_fichiers) {
    DIR *dir;
    struct dirent *entree;
    int cpt = 0;

    /*Test d'ouverture du répertoire en question*/
    dir = opendir("BDD");
    if (dir == NULL) {
        printf("\033[1;31mErreur : Impossible d'ouvrir le répertoire BDD\n\033[0m");
        return 0;
    }

    /*Récupérer nos csv*/
    while ((entree = readdir(dir)) != NULL && cpt < max_fichiers) {
        if (strstr(entree->d_name, ".csv") != NULL) { /*Si c'est un CSV*/
            listeBDD[cpt] = malloc(strlen(entree->d_name) + 1); /*Lui allouer un espace mémoire*/
            if (listeBDD[cpt] == NULL) {
                perror("Erreur d'allocation mémoire");
                closedir(dir);
                return cpt; /*Retourner le nombre de fichiers trouvés*/
            }
            strcpy(listeBDD[cpt], entree->d_name); /*Copier le nom du fichier et l'associer*/
            cpt++;
        }
    }

    closedir(dir); /*Fermer le répertoire*/

    /*Vérifier si on a atteint la limite de fichiers (Je limite à 15)*/
    if (cpt >= max_fichiers) {
        printf("\033[1;31mAttention : Le nombre de fichiers a été limité à %d\n\033[0m", max_fichiers);
    }

    return cpt; /*Je retourne le nombre de fichiers trouvés*/
}

/*Fonction pour choisir une base de donnée*/
char* choiceBDD(char *listeBDD[], int listBDD) {
    int choix = 0;

    /*Si la liste est vide, prévenir l'utilisateur*/
    if (listBDD == 0) {
        printf("Aucune base de donnée n'a été trouvée.\n");
        return NULL; //Retourne NULL si aucune base n'est trouvée
    } else {
        do {
            printf("\033[1;31m Quelle base de donnée voulez-vous charger ?\n\033[0m");
            for (int i = 0; i < listBDD; i++) {  
                printf("%d : %s\n", i + 1, listeBDD[i]);
            }
            printf("0 : Quitter\n");
            printf("Votre choix : ");
            scanf("%d", &choix);
        } while (choix < 0 || choix > listBDD);

        if (choix == 0) {
            printf("\033[1;32m Vous avez choisi de quitter.\n\033[0m");
            return NULL; //Retourne NULL si l'utilisateur quitte
        }
        printf("\033[1;32m Vous avez choisi la base de donnée %s\n\033[0m", listeBDD[choix - 1]);
        return listeBDD[choix - 1]; //Retourne le nom de la base choisie
    }
}

int main(void) {
    do {
    char *listeBDD[15]; /*Tableau pour stocker les noms des bases de données*/
    
    int listBDD = scan_bdd(listeBDD, 15); /*Récupérer les bases de données avec une limite à 15 */
    char *bddChoisie = choiceBDD(listeBDD, listBDD); /*Choisir une base de donnée et stocker le nom choisi */

    if (bddChoisie == NULL) {
        break; //Quitter si aucune base n'est choisie
    }

    //Charger la base de donnée choisie dans un BTREE
    BTree *btree = loadBDD(bddChoisie);
    if (btree == NULL) {
        printf("\033[1;31mErreur : Impossible de charger la base de donnée\n\033[0m");
        return 1;
    }

    //Affichage du menu
    display_menu(btree);
    /*Libération des ressources allouées*/
    for (int i = 0; i < listBDD; i++) {
        free(listeBDD[i]);
    }
    } while (1);
    return 0;
}