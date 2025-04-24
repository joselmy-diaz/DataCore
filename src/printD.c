#include "obj.h"
#include "DataCore.h"
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

void printData (Obj* obj) {
    if (obj == NULL) {
        printf("%s%s", RED, "NULL");
        return;
    }
    ObjString* obtS;
    Nativo* obtO;
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
        case TYPE_NUM:
            obtO = (ObjR*)obj;
            printf("%s%d", RED, obtO->as.Num);
            break;
        case TYPE_NUMFL:
            obtO = (ObjR*)obj;
            printf("%s%f", RED, obtO->as.NumF);
            break;
        case OBJ_STRING:
            obtS = (ObjString*)obj;
            printf("%s%s", YELLOW, obtS->chars);
            break;
        case OBJ_ARRAY:
        case OBJ_HASH_TABLE:
            printf("%s%s", BLUE, "Tabla Hash");
            break;
        case OBJ_AVL_TREE:
            printf("%s%s", BLUE, "Arbol AVL");
            break;
        case OBJ_FUNCTION:
            printf("%s%s", GREEN, "Objeto");
            break;
    }
    printf("\n%s", WHITE);
}

