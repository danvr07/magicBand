/*Vrinceanu Dan 315 CD*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functii.h"

#define MAX 100

int main() {
    int nr = 0, i = 0;
    char str[MAX];

    Stack *undo = createStack();
    Stack *redo = createStack();

    List *list = createBand();
    Node *curent = list->curent;

    TCoada *coada = createQ();

    list->input = fopen("tema1.in", "r");
    list->output = fopen("tema1.out", "w");

    fscanf(list->input, "%d", &nr);

    // Verific daca string urile sunt identice cu cele din fisier;

    for (i = 0; i < nr; i++) {
        char caracter = 0;
        fscanf(list->input, "%s", str);

        if (strcmp(str, "SHOW") == 0) {
            show(curent, list);
        }

        else if (strcmp(str, "MOVE_LEFT") == 0) {
            insert_Q(coada, &move_left, list);
        }

        else if (strcmp(str, "MOVE_RIGHT") == 0) {
            insert_Q(coada, &move_right, list);
        }

        else if (strcmp(str, "INSERT_LEFT") == 0) {
            fscanf(list->input, "%*c %c", &caracter);
            insert_Q_c(coada, &insert_left, list, caracter);
        }

        else if (strcmp(str, "INSERT_RIGHT") == 0) {
            fscanf(list->input, "%*c %c", &caracter);
            insert_Q_c(coada, &insert_right, list, caracter);
        }

        else if (strcmp(str, "MOVE_LEFT_CHAR") == 0) {
            fscanf(list->input, "%*c %c", &caracter);
            insert_Q_c(coada, &move_left_char, list, caracter);
        }

        else if (strcmp(str, "MOVE_RIGHT_CHAR") == 0) {
            fscanf(list->input, "%*c %c", &caracter);
            insert_Q_c(coada, &move_right_char, list, caracter);
        }

        else if (strcmp(str, "WRITE") == 0) {
            fscanf(list->input, "%*c %c", &caracter);
            insert_Q_c(coada, &write, list, caracter);
        }

        else if (strcmp(str, "EXECUTE") == 0) {
            execute(coada, undo, list);
        }

        else if (strcmp(str, "SHOW_CURRENT") == 0) {
            show_curent(list);
        }

        else if (strcmp(str, "UNDO") == 0) {
            undo_redo(undo, list, redo);
        }

        else if (strcmp(str, "REDO") == 0) {
            undo_redo(redo, list, undo);
        }
    }

    fclose(list->input);
    fclose(list->output);


    freeList(list);
    freeCoada(coada);
    freeStiva(undo);
    freeStiva(redo);

    return 0;
}
