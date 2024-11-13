# Explications
Database in C


# Déroulé 
J'ai fait mes deux premières années à l'ESGI donc j'avais de solides bases, et je connaissais déjà assez bien l'algorithme du Btree, pour en avoir fait en Python (et un peu en C).
Je me suis entrainée de mon coté en relisant/reproduisant les exercices d'abord en Python pour revoir l'algo, puis en C via les exercices de l'année précédente.
Après m'être penchée sur l'algo, je me suis mise à developper des bouts de codes, puis le code complet de l'exercice.

# Technos utilisées
Version de C : Bien qu'habituée au C90, j'ai recemment choisi de basculer vers du C99, notamment parce que j'ai la facheuse habitude des commentaires en //, n'existant pas en C90.
Distrib : Debian 12
Compilation :   cd "nom du dossier"
                make
                ./executable

J'utilise un make que nous avons developpés avec notre professeur l'an dernier. Surnommé "MakeFile Magique", il récupère nos fichiers sans avoir à les entrer un par un. Je l'ai legerement modifié pour ajouter la compilation avec C99 + la lecture de mes sous dossiers.

# Organisation
J'ai organisé mon arborescence en fonction de mes fonctions principales, pour eviter que mes .h et .c soient désordonnés.
Globalement, ma fonction/fichier main.c ne sert qu'a déclencher la première fonction.


# Fonctionnement
-On peut ajouter des bdd sous format CSV dans le dossier BDD (à condition de respecter le format).
-Mon programme lit le dossier, récupère les BDD et demande de choisir avec laquelle nous voulons intéragir.
Il ne lit que 15 fichiers parce que le programme serait un peu lourd autrement selon moi. Cependant, ce paramètre est changeable dans le code de mon main.C.
-La bdd est translate dans un Btree (load.c).
-On peut avoir un appercu graphique de notre bdd

# Tree
Concernant l'utilisage d'un arbre, comme vu avec vous, j'utiliserais un Btree sans lequel je vais inserer mes données, extraites du CSV.


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
