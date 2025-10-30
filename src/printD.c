#include "obj.h"
#include "DataCore.h"
#include "estructura.h"
#include <stdio.h>



#define BLACK   "\x1B[30m"
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define WHITE   "\x1B[37m"
#define ORANGE  "\x1B[38;2;255;128;0m"
#define ROSE    "\x1B[38;2;255;151;203m"
#define LBLUE   "\x1B[38;2;53;149;240m"
#define LGREEN  "\x1B[38;2;17;245;120m"
#define GRAY    "\x1B[38;2;176;174;174m"
#define BOLD    "\x1b[1m"
#define RESET   "\x1b[0m"


void printEn(Entry* entry){
    printf("%s%s:", WHITE, entry->key);
    printObj(entry->data);
}

void recorrerNodo(NodeEntry* nodo) {
    if (nodo == NULL) return;

    // Primero el subárbol izquierdo
    recorrerNodo(nodo->left);

    printEn(&nodo->data);
    printf("%s ,", WHITE);

    recorrerNodo(nodo->right);
}

void TablaP(Entry* nodo) {
    if (nodo == NULL) return;
    printEn(nodo);
    printf("%s ,", WHITE);
}

void printObj (Obj* obj){
    if (obj == NULL) {
        printf("%s%s\n", RED, "NULL");
        return;
    }
    switch (GetType(obj)) {
        case TYPE_NULL:
            printf("%s%s\n", RED, "NULL");
            break;
        case TYPE_BOOL_F:
            printf("%s%s", MAGENTA, "false");
            break;
        case TYPE_BOOL_T:
            printf("%s%s", MAGENTA, "true");
            break;
        case TYPE_NUM: {
            Native* obtO = (Native*)obj;
            printf("%s%d", RED, obtO->as.Num);
            break;
        }
        case TYPE_NUMFL: {
            Native* obtO = (Native*)obj;
            printf("%s%f", RED, obtO->as.NumF);
            break;
        }
        case TYPE_ENTRY: {
            Entry* obtE = (Entry*)obj;
            printEn(obtE);
            break;
        }
        case TYPE_STRING: {
            printf("%s%s", YELLOW, getString(obj));
            break;
        }
        case TYPE_ARRAY:
            printf("%s[", WHITE);
            for (int i = 0; i < getArraySize(obj); i++) {
                printObj(searchArray(obj, i));
                if (i < getArraySize(obj) - 1) printf("%s, ", WHITE);
            }
            printf("%s]", WHITE);
            break;
        case TYPE_HASH_TABLE:
            printf("%s{", BLUE);
            hash_foreach(obj, TablaP);
            printf("%s}", BLUE);
            break;
        case TYPE_AVL_TREE:
            printf("%s{", WHITE);
            recorrerNodo(((ObjTree*)obj)->root);
            printf("%s}", WHITE);
            break;
        case TYPE_FUNCTION:
            printf("%s%s", GREEN, "<Function Object>");
            break;
        default:
            printf("%s%s", GRAY, "<Unknown Type>");
            break;
    }
}

void printObjf (Obj* obj) {
    if (obj == NULL) {
        printf("%s%s\n", RED, "NULL");
        return;
    }
    printObj(obj);
    printf("\n%s", WHITE);
}

