# Explications
Database in C


# Déroulé 
J'ai fait mes deux premières années à l'ESGI donc j'avais de solides bases, et je connaissais déjà assez bien l'algorithme du Btree, pour en avoir fait en Python (et un peu en C).
Je me suis entrainée de mon coté en relisant/reproduisant les exercices d'abord en Python pour revoir l'algo, puis en C via les exercices de l'année précédente.
Après m'être penchée sur l'algo, je me suis mise à developper des bouts de codes, puis le code complet de l'exercice.

# Technos utilisées
Prérequis : gcc + make          
Version de C : Bien qu'habituée au C90, j'ai recemment choisi de basculer vers du C99, notamment parce que j'ai la facheuse habitude des commentaires en //, n'existant pas en C90.
Distrib : Debian 12
Version Gcc: 12.2.0 ()
Flags utilisés :
    -02 : Optimisation level 2, permet de dev un programme sans necessités particulières en terme de perf. 
    -Wall : Active tous les advertissements. 
    -Wextra : Active le maximum d'advertissements.
    -Werror : Les adv sont traités comme des erreurs, ce qui permet de ne rien laisser passer.
    -ansi : utiliser ANSI
    -std=c99 : comme expliqué ci-dessus, surtout pour utiliser les commentaires en //

Compilation :   cd "nom du dossier"
                make
                ./executable

J'utilise un make que nous avons developpés avec notre professeur l'an dernier. Surnommé "MakeFile Magique", il récupère nos fichiers sans avoir à les entrer un par un. Je l'ai legerement modifié pour ajouter la compilation avec C99 + la lecture de mes sous dossiers.

Sanitaziers/Outils de debogage :
Mon makefile étant assez explicite concernant les erreurs grace à mes flags, je n'ai pas eu besoin d'en installer d'autres.
J'ai par contre utilisé valgrind pour les fuites mémoires, intégré dans un makefile sur ma linux. Je le pusherais sur git
J'ai choisi valgrind car j'ai debug un ami qui avait installé cet outil, et je l'ai trouvé assez simple de compréhension.


# Organisation
J'ai organisé mon arborescence en fonction de mes fonctions principales, pour eviter que mes .h et .c soient désordonnés.
Globalement, ma fonction/fichier main.c ne sert qu'a déclencher la première fonction.


# Fonctionnement
-On peut ajouter des bdd sous format CSV dans le dossier BDD (à condition de respecter le format).
-Mon programme lit le dossier, récupère les BDD et demande de choisir avec laquelle nous voulons intéragir.
Il ne lit que 15 fichiers parce que le programme serait un peu lourd autrement selon moi. Cependant, ce paramètre est changeable dans le code de mon main.C.
-La bdd est translate dans un Btree (load.c).
-On peut avoir un appercu graphique de notre bdd
-On peut effectuer les commands select et delete, avec un where (il faut mettre un espace dans la condition, exemple : select name from eleve where id = 1, delete * from eleve where id = 3)

# Type de Tree
Concernant l'utilisage d'un arbre, comme vu avec vous, j'utiliserais un Btree sans lequel je vais inserer mes données, extraites du CSV.
Concernant les algos, il y a l'algo de création de l'arbre, celui de recherche, celui de d'insertion et supression utilisant en partie celui de recherche

# Sources
BTree :
https://www.geeksforgeeks.org/implementation-of-b-tree-in-c/
https://www.programiz.com/dsa/b-tree
https://www.geeksforgeeks.org/introduction-of-b-tree-2/
https://www.programiz.com/dsa/insertion-into-a-b-tree
https://www.tutorialspoint.com/data_structures_algorithms/b_trees.htm
Et, en très grande partie:
-Cours de NSI
-Cours de Mr TRANCHO


Style Tableau :
https://stackoverflow.com/questions/3274824/color-console-in-ansi-c
https://solarianprogrammer.com/2019/04/08/c-programming-ansi-escape-codes-windows-macos-linux-terminals/

Valgrind :
https://cscie28.dce.harvard.edu/reference/programming/valgrind/valgrind.html
https://student.cs.uwaterloo.ca/~cs241/valgrind/
https://stackoverflow.com/questions/68475131/how-can-i-understand-a-memory-leak-from-valgrind-output