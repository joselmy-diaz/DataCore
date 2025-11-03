#include "obj.h"
#include "DataCore.h"
#include "structure.h"
#include <stdio.h>

/**
 * Color definitions for console output formatting
 * These ANSI escape codes are used to color the output text
 */
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

// Constantes para mensajes comunes
#define NULL_STR "NULL"
#define FUNCTION_STR "<Function Object>"
#define UNKNOWN_TYPE_STR "<Unknown Type>"
#define SEPARATOR ", "


/**
 * Imprime una entrada (key-value pair)
 * @param entry Puntero a la entrada a imprimir
 */
void printEn(Entry* entry) {
    if (entry == NULL) {
        printf("%s%s\n", RED, NULL_STR);
        return;
    }
    printf("%s%s:", WHITE, entry->key);
    printObj(entry->data);
}

/**
 * Imprime los elementos de un array
 * @param array Puntero al array a imprimir
 */
void printArray(ObjArray* array) {
    if (array == NULL) return;
    
    size_t size = getArraySize(array);
    for (size_t i = 0; i < size; i++) {
        printObj(array->array[i]);
        if (i < size - 1) {
            printf("%s%s", WHITE, SEPARATOR);
        }
    }
}

/**
 * Función recursiva auxiliar para recorrer un árbol binario en orden
 * @param nodo Puntero al nodo actual
 * @param first Indicador de primer elemento
 */
static void recorrerNodo_rec(NodeEntry* nodo, int *first) {
    if (nodo == NULL) return;
    
    recorrerNodo_rec(nodo->left, first);
    if (!*first) {
        printf("%s%s", WHITE, SEPARATOR);
    }
    printEn(&nodo->data);
    *first = 0;
    recorrerNodo_rec(nodo->right, first);
}

/**
 * Inicia el recorrido de un árbol binario
 * @param nodo Puntero al nodo raíz
 */
void recorrerNodo(NodeEntry* nodo) {
    int first = 1;
    recorrerNodo_rec(nodo, &first);
}

/**
 * Imprime una entrada de la tabla hash
 * @param nodo Puntero a la entrada
 */
void TablaP(Entry* nodo) {
    if (nodo == NULL) return;
    printEn(nodo);
    printf("%s%s", WHITE, SEPARATOR);
}

/**
 * Imprime un objeto según su tipo
 * @param obj Puntero al objeto a imprimir
 */
void printObj(Obj* obj) {
    if (obj == NULL) {
        printf("%s%s\n", RED, NULL_STR);
        return;
    }

    switch (GetType(obj)) {
        case TYPE_NULL:
            printf("%s%s\n", RED, NULL_STR);
            break;
            
        case TYPE_BOOL_F:
            printf("%s%s", MAGENTA, "false");
            break;
            
        case TYPE_BOOL_T:
            printf("%s%s", MAGENTA, "true");
            break;
            
        case TYPE_NUM: {
            Native* native = (Native*)obj;
            printf("%s%d", RED, native->as.Num);
            break;
        }
            
        case TYPE_NUMFL: {
            Native* native = (Native*)obj;
            printf("%s%.6f", RED, native->as.NumF);  // Precisión fija para float
            break;
        }
            
        case TYPE_ENTRY: {
            Entry* entry = (Entry*)obj;
            printEn(entry);
            break;
        }
            
        case TYPE_STRING: {
            const char* str = getString(obj);
            if (str) {
                printf("%s%s", YELLOW, str);
            } else {
                printf("%s%s", RED, NULL_STR);
            }
            break;
        }
            
        case TYPE_ARRAY:
            printf("%s[", WHITE);
            printArray((ObjArray*)obj);
            printf("%s]", WHITE);
            break;
            
        case TYPE_HASH_TABLE:
            printf("%s{", BLUE);
            Obj* arr = recorrerTH(obj);
            if (arr) {
                printArray((ObjArray*)arr);
                freeArray(arr);
            }
            printf("%s}", BLUE);
            break;
            
        case TYPE_AVL_TREE:
            printf("%s{", WHITE);
            ObjTree* tree = (ObjTree*)obj;
            if (tree && tree->root) {
                recorrerNodo(tree->root);
            }
            printf("%s}", WHITE);
            break;
            
        case TYPE_FUNCTION:
            printf("%s%s", GREEN, FUNCTION_STR);
            break;
            
        default:
            printf("%s%s", GRAY, UNKNOWN_TYPE_STR);
            break;
    }
}

/**
 * Imprime un objeto y agrega un salto de línea al final
 * @param obj Puntero al objeto a imprimir
 */
void printObjf(Obj* obj) {
    if (obj == NULL) {
        printf("%s%s\n", RED, NULL_STR);
        return;
    }
    printObj(obj);
    printf("\n%s", RESET);  // Usamos RESET en lugar de WHITE para restaurar el color por defecto
}

