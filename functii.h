/*Vrinceanu Dan 315 CD*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char item;
    struct Node *next;
    struct Node *prev;
} Node;

typedef struct List {
    Node *sant;
    Node *curent;

    FILE *input;
    FILE *output;
} List;

typedef struct TNode {
    struct TNode *next;
    void (*f_pointer_c)(List *, char);
    void (*f_pointer)(List *);
    List *list;
    char item;
} TNode;

typedef struct TCoada {
    TNode *head;
    TNode *tail;
} TCoada;

typedef struct StackNode {
    struct Node *curent;
    struct StackNode *next;
} StackNode;

typedef struct Stack {
    StackNode *head;
} Stack;

TNode *initNode() {
    TNode *node = (TNode*)malloc(sizeof(TNode));
    node->next = NULL;

    return node;
}

Node *createNode() {
    Node *node = (Node*)malloc(sizeof(Node));
    node->item = '#';
    node->prev = node->next = NULL;

    return node;
}

// Creaza lista cu santinela si primul nod
// si salvez pointerii lor.

List *createBand() {
    List *list = (List *)malloc(sizeof(List));
    Node *santinel = (Node *)calloc(1, sizeof(Node));
    list->sant = santinel;

    Node *first_node = createNode();
    list->sant->next = first_node;
    first_node->prev = list->sant;
    first_node->next = NULL;
    list->curent = list->sant->next;

    return list;
}

// Functie pentru a creea o stiva

Stack *createStack() {
    Stack *undo_redo = (Stack *)malloc(sizeof(Stack));
    undo_redo->head = NULL;

    return undo_redo;
}

// Functie pentru a creea o coada

TCoada *createQ() {
    TCoada *q = (TCoada *)malloc(sizeof(TCoada));
    q->head = q->tail = NULL;

    return q;
}

// Daca precedentul nodului este nu este santinela;
// Degetul se muta in urma, altfel sta pe loc.

void move_left(List *list) {
    if (list->curent->prev != list->sant) {
        list->curent = list->curent->prev;
    }
}

// Daca in dreapta nodului curent nu este nod, se creeaza si dupa se muta degetul
// Daca este, doar se muta degetul

void move_right(List *list) {
    if (list->curent->next == NULL) {
        Node *newnode = createNode();
        list->curent->next = newnode;
        newnode->prev = list->curent;
        list->curent = list->curent->next;
    } else {
        list->curent = list->curent->next;
    }
}

// Pe pozitia degetului se pune un caracter

void write(List *list, char item) {
    list->curent->item = item;
}

// Daca degetul se afla pe primul nod, se afiseaza eroare;
// Daca nu, se insereaza un nod nou;

void insert_left(List *list, char item) {
    if (list->curent->prev == list->sant) {
        fprintf(list->output, "ERROR\n");
    } else {
        Node *newnode = createNode();
        list->curent->prev->next = newnode;
        newnode->prev = list->curent->prev;
        newnode->next = list->curent;
        list->curent->prev = newnode;
        list->curent = newnode;
        list->curent->item = item;
    }
}

// Daca la dreapta nu este nimic, se creeaza un nod nou;
// Daca este, se insereaza intre curent si acel nod, un nod nou;

void insert_right(List *list, char item) {
    Node *newnode = createNode();

    if (list->curent->next == NULL) {
        list->curent->next = newnode;
        newnode->item = item;
        newnode->prev = list->curent;
    } else {
        newnode->next = list->curent->next;
        list->curent->next->prev = newnode;
        list->curent->next = newnode;
        newnode->prev = list->curent;
    }
    list->curent = list->curent->next;
    list->curent->item = item;
}

// Iterez la stanga prin lista pentru a gasi pozitia unde trebuie mutat degetul
// Daca nu se gaseste acel caracter, se afiseaza eroare, degetul nu schimba poz.

void move_left_char(List *list, char item) {
    List *iter = list;
    Node *aux = list->curent; // salvez locul unde se afla degetul

    if (iter->curent->item != item) {

        while (iter->curent->prev != iter->sant && iter->curent->prev->item != item) {
            iter->curent = iter->curent->prev;
        }

        if (iter->curent->prev == iter->sant) {
            fprintf(list->output, "ERROR\n");
            iter->curent = aux;
        } else {
            iter->curent->prev->item = item;
            iter->curent = iter->curent->prev;
        }
    }
}

// Iterez la dreapta prin lista pentru a gasi pozitia unde trebuie mutat degetul
// Daca nu se gaseste acel caracter, se insereaza un nod nou la sfarsit

void move_right_char(List *list, char item) {
    List *iter = list;

    if (iter->curent->item != item) {

        while (iter->curent->next != NULL && iter->curent->next->item != item) {
            iter->curent = iter->curent->next;
        }

        if (iter->curent->next == NULL) {
            Node *newnode = createNode();
            iter->curent->next = newnode;
            iter->curent->next->item = '#';
            newnode->prev = iter->curent;

            iter->curent = newnode;
        } else {
            iter->curent->next->item = item;
            iter->curent = iter->curent->next;
        }
    }
}

// Se introduc in coada pointerii functiilor de la sfarsitul listei;
// Doar functiile care au parametru "List, char".

void insert_Q_c(TCoada *coada, void (*f_pointer_c)(List *, char), List *list, char item) {

    TNode *new_node = initNode();
    new_node->f_pointer_c = f_pointer_c;
    new_node->list = list;
    new_node->item = item;

    if (coada->tail == NULL) {
        coada->head = coada->tail = new_node;
    } else {
        coada->tail->next = new_node;
        coada->tail = new_node;
    }
}

// Se introduc in coada pointerii functiilor de la sfarsitul listei;
// Doar functiile care au doar parametru "List".

void insert_Q(TCoada *coada, void (*f_pointer)(List *), List *list) {

    TNode *new_node = initNode();
    new_node->f_pointer = f_pointer;
    new_node->list = list;
    new_node->item = '\0';

    if (coada->tail == NULL) {
        coada->head = coada->tail = new_node;
    } else {
        coada->tail->next = new_node;
        coada->tail = new_node;
    }
}

// Functie pentru a introduce pozitia precedenta a degetului;
// Dupa ce s a citit comanda EXECUTE.

void insert_stack(Stack *undo, List *list) {
    StackNode *newnode = (StackNode *)malloc(sizeof(StackNode));
    newnode->curent = list->curent;
    newnode->next = undo->head;
    undo->head = newnode;
}

// Mai intai se introduce in stiva pozitia degetului
// Dupa se scoate din coada fiecare functie

void execute(TCoada *coada, Stack *undo, List *list) {
    TNode *nod_curent = coada->head;

    if (coada->head != NULL) {

        // verific daca este o functie fara char

        if (nod_curent->item == '\0') {

            if (nod_curent->f_pointer == &move_right) {
                insert_stack(undo, list);
            }
            else if (nod_curent->f_pointer == &move_left) {
                if (nod_curent->list->curent->prev != nod_curent->list->sant) {
                    insert_stack(undo, list);
                }
            }
        } else {
            // Daca sunt alte operatii se goleste stiva
            while (undo->head != NULL) {
                StackNode *tmp = undo->head->next;
                free(undo->head);
                undo->head = tmp;
            }
        }

        // Se verifica daca este o functie cu parametru "char" sau nu

        if (nod_curent->item != '\0') {
            nod_curent->f_pointer_c(nod_curent->list, nod_curent->item);
        } else {
            nod_curent->f_pointer(nod_curent->list);
        }
        coada->head = coada->head->next;

        nod_curent->item = '\0';
        free(nod_curent);
    }

    // dau valoarea NULL pentru urmatoarea data cand se introduce
    // in coada si ea este goala

    if (coada->head == NULL) {
        coada->tail = coada->head;
    }
}

// Se afiseaza banda

void show(Node *curent, List *list) {
    Node *iter = curent;

    while (iter != NULL) {
        if (iter == list->curent) {
            fprintf(list->output, "|%c|", iter->item);
        } else {
            fprintf(list->output, "%c", iter->item);
        }
        iter = iter->next;
    }
    iter = curent;
    fprintf(list->output, "\n");
}

// Se afiseaza pozitia curenta in banda

void show_curent(List *list) {
    fprintf(list->output, "%c\n", list->curent->item);
}

// Apeland undo se introduce pozitia precedenta in redo si invers;

void undo_redo(Stack *stack1, List *list, Stack *stack2) {

    if (stack1->head != NULL) {
        StackNode *temp = stack1->head;
        temp->curent = stack1->head->curent;

        StackNode *newnode = (StackNode*)malloc(sizeof(StackNode));
        newnode->curent = list->curent;
        newnode->next = stack2->head;
        stack2->head = newnode;
        list->curent = temp->curent;

        StackNode *freeN = stack1->head->next;
        free(stack1->head);
        stack1->head = freeN;
    }
}

// Se goleste lista

void freeList(List *list) {
    Node *iter, *temp;
    iter = list->sant;

    while (iter != NULL) {
        temp = iter;
        iter = iter->next;
        free(temp);
    }
    free(list);
}

// Se golesc stivele

void freeStiva(Stack *stack) {
    while (stack->head != NULL) {
        StackNode *tmp = stack->head->next;
        free(stack->head);
        stack->head = tmp;
    }
    free(stack);
}

// Se goleste coada

void freeCoada(TCoada *coada) {
    while (coada->head != NULL) {
        TNode *tmp = coada->head->next;
        free(coada->head);
        coada->head = tmp;
    }
    free(coada);
}
