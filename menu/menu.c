#include <stdio.h>
#include "menu.h"
#include "../save/save.h"
#include "../load/load.h"
#include "../commands/commands.h"

//Recuperer le btree en paramètre
void display_menu( BTree* btree) {
    char choix;
    if (btree == NULL) {
        printf("Erreur : Aucune base de donnée chargée\n");
        return;
    }
    do {
        printf("\033[1;31m");
        printf("Choisissez une option :\n");
        printf("\033[0m");
        printf("1 : Afficher la base de données\n");
        printf("2 : Effectuer des commandes\n");
        printf("3 : Sauvegarder les changements effectués\n");
        printf("4 : Charger une autre base de donnée\n");
        printf("0 : Quitter\n");
        printf("Votre choix : ");
        
        scanf(" %c", &choix);
        
        switch(choix) {
            case '1':
                //Afficher la base de données
                printf("\033[1;32m");
                printf("Affichage de la base de données\n");
                printf("\033[0m");
                //affichzge du contenu de la base de données
                BTreeNode* root = btree->root;
                for (int i = 0; i < root->keyCount; i++) {
                    Table* table = root->tables[i];
                    
                    printf("\n\033[1;37m"); //Echapement pour couleur blanche, en gras
                    printf("╔══════════════════════════════════════════════════════════╗\n"); //J'ai trouvé ces caractères sur un git 
                    printf("║ %-56s ║\n", table->name);
                    printf("╠══════════════════════════════════════════════════════════╣\n");
                
                    printf("║ ");
                    printf("\033[1;31m"); //Echapement pour couleur rouge en gras
                    for (int j = 0; j < table->columnCount; j++) {
                        printf("%-15s", table->columnNames[j]);
                    }
                    printf("\033[1;37m"); //Echapement pour couleur blanche
                    printf("║\n");
                    printf("╠══════════════════════════════════════════════════════════╣\n");
                    for (int j = 0; j < table->rowCount; j++) {
                        printf("║ ");
                        for (int k = 0; k < table->columnCount; k++) {
                            printf("%-15s", table->rows[j].values[k]);
                        }
                        printf(" ║\n");
                    }
                    printf("╚══════════════════════════════════════════════════════════╝\n");
                    printf("\033[0m"); //Reset
                }
                break;
            case '2':
                //Effectuer des commandes
                printf("\033[1;32m");
                printf("Effectuer des commandes\n");
                printf("\033[0m");
                commands(btree);
                break;
            case '3':
                //Sauvegarder les changements effectués
                printf("\033[1;32m");
                printf("Sauvegarde des changements effectués\n");
                printf("\033[0m");
                break;
            case '4':
                //Charger une autre base de donnée
                printf("\033[1;32m");
                printf("Charger une autre base de donnée\n");
                printf("\n");
                printf("\033[0m");
                break;
            case '0':
                //Quitter
                printf("\033[1;32m");
                printf("Vous avez choisi de quitter.\n");
                printf("\033[0m");
                printf("\n");
                break;
            default:
                printf("\033[1;31m");
                printf("Erreur : Choix invalide\n");
                printf("\033[0m");
                break;
        }
        
    } while (choix != '0' && choix != '4');
}
