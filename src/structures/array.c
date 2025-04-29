#include "stuctura.h"
#include "DataCore.h"

// Inicializa la lista
Obj* initArray(int8_t size, bool isTree) {
    ObjCon* objL = malloc(sizeof(ObjCon));
    objL->obj.type = TYPE_ARRAY;
    objL->obj.reference = 0;
    objL->length = size;
    objL->free = size;
    objL->array = (Obj**)malloc(size * sizeof(Obj*));
    for(int i = 0; i < size; i++) {
        objL->array[i] = NULL;
    }
    if (isTree) {
        objL->array[0] = initTR();
        objL->free--;
    }
    return (Obj*)objL;
}

// is TreeL or table Hash
bool isTreeOrTable(ObjCon* objL) {
    if (objL->array == NULL || (objL->length - objL->free) < 1) return false;
    Obj* data1 = objL->array[0];
    if (data1 != NULL) {
        if (data1->type == TYPE_AVL_TREE) return true;
        else if (data1->type == TYPE_HASH_TABLE) return true;
    }
    return false;
}


// Inserta un elemento en la lista
bool insertLByKey(ObjCon* objL, Entry data) {
    if (1 <= objL->length - objL->free) {
        if (isTreeOrTable(objL)) {
            Obj* data1 = objL->array[0];
            return insertD(data1, data);
        } else {
            insertArray(objL, data.data);
        }
        //printf("No se puede insectar en este tipo de estructura 2");
    } else return false;
}

// Inserta un elemento en la lista
bool insertArray(ObjCon* objL, Obj* data) {
    if (data == NULL) return false;
    if (objL->free > 0) {
        size_t index = objL->length - objL->free;
        assignData(&objL->array[index], data);
        objL->free--;
        return true;
    } else {
        size_t oldLength = objL->length;
        objL->length += 100;
        objL->free = 99;
        Obj** temp = realloc(objL->array, objL->length * sizeof(Obj*));
        if (temp == NULL)  return false;  // Falló realloc → no toques objL
        objL->array = temp;
        for (size_t i = oldLength; i < objL->length; i++) {
            objL->array[i] = NULL;
        }
        assignData(&objL->array[oldLength], data);
        return true;
    }
}
// Busca un elemento en la lista
Obj* searchArray(ObjCon* objL, int index) {
    if (objL == NULL) return NULL;
    if (index <= objL->length - objL->free) {
        return objL->array[index + (isTreeOrTable(objL) ? 1 : 0)];
    } else return NULL;
}

// Busca un elemento en la lista por una key
Obj* searchLByKey (ObjCon* objL, const char* key) {
    Obj* RData = NULL;
    if (objL->array != NULL) {
        Obj* data1 = objL->array[0];
        if (isTreeOrTable(objL)) RData = searchD(data1, key);
        else printf("No se puede buscar en este tipo de estructura");
    }
    return RData;
}

int getziseL(ObjCon* objL) {
    return (objL->length - objL->free) - (isTreeOrTable(objL) ? 1 : 0);
}

// Libera la memoria de la lista
bool freeArray(ObjCon* objL) {
    if (objL == NULL) return false;
    for (int i = 0; i < objL->length - objL->free; i++) {
        if(objL->array[i] != NULL) freeObjs(objL->array[i]);
    }
    free(objL->array);
    free(objL);
    return true;
}