#include "menu.h"

//Fonction pour afficher le tree
void display_database(BTree* btree) {
    if (btree == NULL || btree->root == NULL) {
        printf("\033[1;31mErreur : Base de données vide ou non initialisée\n\033[0m");
        return;
    }

    BTreeNode* root = btree->root;
    for (int i = 0; i < root->keyCount; i++) {
        Table* table = root->tables[i];
        
        printf("\n\033[1;37m");
        printf("╔══════════════════════════════════════════════════════════╗\n");
        printf("║ %-56s ║\n", table->name);
        printf("╠══════════════════════════════════════════════════════════╣\n");
    
        printf("║ ");
        printf("\033[1;31m");
        for (int j = 0; j < table->columnCount; j++) {
            printf("%-15s", table->columnNames[j]);
        }
        printf("\033[1;37m");
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
        printf("\033[0m");
    }
}

//Recuperer le btree en paramètre
void display_menu( BTree* btree) {
    char choix;
    if (btree == NULL) {
        printf("\033[1;31m Erreur : Impossible de charger la base de donnée\n\033[0m");
        return;
    }
    do {
        printf("\033[1;31m Choisissez une option\n\033[0m");
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
                printf("\033[1;32m Afficher la base de données\n \033[0m");
                display_database(btree);
                break;
            case '2':
                //Effectuer des commandes
                printf("\033[1;32m Effectuer des commandes\n \033[0m");
                commands(btree);
                break;
            case '3':
                //Sauvegarder les changements effectués
                char filename[100];
                do {
                printf("\033[1;32m Entrez le nom du fichier de sauvegarde : \033[0m");
                scanf(" %[^\n]", filename);
                if (strlen(filename) == 0) {
                    printf("\033[1;31m Erreur : Nom de fichier invalide\n\033[0m");
                } else {
                    break;
                }
                } while (1);
                save(btree, filename);
                break;
            case '4':
                //Charger une autre base de donnée
                printf("\033[1;32m Vous avez choisi de charger une autre base de donnée. Retour au menu principal.\n \033[0m");
                break;
            case '0':
                //Quitter
                printf("\033[1;32m Vous avez choisi de quitter.\n \033[0m");
                break;
            default:
                printf("\033[1;31m Erreur : Choix invalide\n \033[0m");
                break;
        }
        
    } while (choix != '0' && choix != '4');
}
