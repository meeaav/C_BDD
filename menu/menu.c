#include <stdio.h>
#include "menu.h"
#include "../save/save.h"
#include "../load/load.h"

//Recuperer le btree en paramètre
void display_menu( BTree* btree) {
    char choix;
    if (btree == NULL) {
        printf("Erreur : Aucune base de donnée chargée\n");
        return;
    }
    do {
        printf("Choisissez une option :\n");
        printf("1 : Afficher la base de données\n");
        printf("2 : Effectuer des commandes\n");
        printf("3 : Sauvegarder les changements effectués\n");
        printf("4 : Charger une autre base de donnée\n");
        printf("Q : Quitter\n");
        printf("Votre choix : ");
        
        scanf(" %c", &choix);
        
        switch(choix) {
            case '1':
                //Afficher la base de données
                printf("Affichage de la base de données\n");
                break;
            case '2':
                //Effectuer des commandes
                printf("Effectuer des commandes\n");
                break;
            case '3':
                //Sauvegarder les changements effectués
                printf("Sauvegarde des changements effectués\n");
                break;
            case '4':
                //Charger une autre base de donnée
                printf("Charger une autre base de donnée\n");
                break;
            case 'Q':
            case 'q':
                //Quitter
                printf("Quitter\n");
                break;
            default:
                printf("Erreur : Choix invalide\n");
                break;
        }
        
    } while (choix != 'Q' && choix != 'q');
}