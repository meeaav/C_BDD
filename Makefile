#Variables
CC = gcc
CFLAGS = -O2 -Wall -Wextra -Werror -ansi -std=c99 -g
CLIBS = -lm
EXE = executable
OBJ_DIR = obj/
SRC_DIRS = load menu save commands

#Trouver tous les fichiers .c et .h dans les sous-répertoires
FILEC := $(wildcard main.c $(addsuffix /*.c, $(SRC_DIRS)))
FILEH := $(wildcard $(addsuffix /*.h, $(SRC_DIRS)))

#Générer la liste des fichiers objets correspondants
FILEO := $(patsubst %.c, $(OBJ_DIR)%.o, $(FILEC))

#Règle par défaut : compilation de l'exécutable
$(EXE): $(FILEO)
	$(CC) $(CFLAGS) -o $@ $^ $(CLIBS)

#Compilation des fichiers .c en .o
$(OBJ_DIR)%.o: %.c $(FILEH)
	@mkdir -p $(OBJ_DIR)$(dir $<)  # Créer le répertoire obj/ si nécessaire
	$(CC) $(CFLAGS) -o $@ -c $<

#Nettoyage des fichiers objets et de l'exécutable
clean:
	rm -rf $(OBJ_DIR)*.o
	rm -rf $(EXE)

#Nouvelle cible pour Valgrind
valgrind: $(EXE)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(EXE)

#Cible pour exécuter Valgrind et sauvegarder la sortie dans un fichier
valgrind-log: $(EXE)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(EXE) 2> valgrind_output.txt

#Cible pour nettoyer tous les fichiers générés, y compris le rapport Valgrind
clean-all: clean
	rm -f valgrind_output.txt