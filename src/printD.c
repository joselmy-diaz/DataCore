#include "obj.h"
#include "DataCore.h"
#include "stuctura.h"
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


char* printEn(Entry* entry){
    printf("%s:", entry->key);
    printObj(entry->data);
    return NULL;
}

void recorrerNodo(NodeEntry* nodo) {
    if (nodo == NULL) return;

    // Primero el subárbol izquierdo
    recorrerNodo(nodo->left);

    if (nodo->data != NULL) {
        printEn(nodo);
        printf("%s ,", WHITE);
    }
    recorrerNodo(nodo->right);
}

void printObj (Obj* obj){
    switch (obj->type) {
        case TYPE_NULL:
            printf("%s%s", RED, "NULL");
            break;
        case TYPE_BOOL_F:
            printf("%s%s", MAGENTA, "false");
            break;
        case TYPE_BOOL_T:
            printf("%s%s", MAGENTA, "true");
            break;
        case TYPE_NUM: {
            Nativo* obtO = (Nativo*)obj;
            printf("%s%d", RED, obtO->as.Num);
            break;
        }
        case TYPE_NUMFL: {
            Nativo* obtO = (Nativo*)obj;
            printf("%s%f", RED, obtO->as.NumF);
            break;
        }
        case OBJ_STRING: {
            printf("%s%s", YELLOW, getString(obj));
            break;
        }
        case OBJ_ARRAY:
            printf("%s[", WHITE);
            for (int i = 0; i < getziseL(obj); i++) {
                printObj(searchL(obj, i));
                if (i < getziseL(obj) - 1) printf("%s, ", WHITE);
            }
            printf("%s]", WHITE);
            break;
        case OBJ_HASH_TABLE:
            printf("%s%s", BLUE, "Hash Table");
            break;
        case OBJ_AVL_TREE:
            printf("%s{", WHITE);
            recorrerNodo( ((ObjTree*)obj)->root);
            printf("%s}", WHITE);
            break;
        case OBJ_FUNCTION:
            printf("%s%s", GREEN, "<Function Object>");
            break;
        default:
            printf("%s%s", GRAY, "<Unknown Type>");
            break;
    }
}

void printObjf (Obj* obj) {
    if (obj == NULL) {
        printf("%s%s", RED, "NULL");
        return;
    }
    printObj(obj);
    printf("\n%s", WHITE);
}

