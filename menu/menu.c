#include <stdio.h>
#include "menu.h"
#include "../save/save.h"
#include "../load/load.h"

void afficher_menu(void) {
    char choix;
    
    do {
        printf("Choisissez une option :\n");
        printf("1 : Effectuer des commandes\n");
        printf("2 : Sauvegarder une base de donnée\n");
        printf("3 : Charger une autre base de donnée\n");
        printf("Q : Quitter\n");
        printf("Votre choix : ");
        
        scanf(" %c", &choix);
        
        switch(choix) {
            case '1':
                printf("Effectuer des commandes\n");
                break;
            case '2':
                printf("Sauvegarder une base de donnée\n");
                // save();
                break;
            case '3':
                printf("Charger une autre base de donnée\n");
                // load();
                break;
            case 'Q':
            case 'q':
                printf("Quitter\n");
                break;
            default:
                printf("Choix invalide\n");
        }
        
    } while (choix != 'Q' && choix != 'q');
}