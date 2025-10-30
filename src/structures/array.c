#include "estructura.h"
#include "DataCore.h"

// inicial  valores de la tamaño del array
bool initvaluesArray(ObjArray* objL, bool isTree) {
    size_t oldLength = 0;
    if (isTree) {
        oldLength = objL->length;
        objL->length += 10;
        objL->free += 10;
        Obj** temp = realloc(objL->array, objL->length * sizeof(Obj*));
        if (temp == NULL)  return false;  // Falló realloc → no toques objL
        objL->array = temp;
    }
    for (size_t i = oldLength; i < objL->length; i++) {
        objL->array[i] = NULL;
    }
    return true;
}

// Inicializa la lista
Obj* initArray(int8_t size) {
    ObjArray* objL = malloc(sizeof(ObjArray));
    objL->obj.type = TYPE_ARRAY;
    objL->obj.reference = 0;
    objL->length = size;
    objL->free = size;
    objL->array = (Obj**)malloc(size * sizeof(Obj*));
    initvaluesArray(objL, false);
    return (Obj*)objL;
}

// is TreeL or table Hash
bool isTreeOrTable(Obj* obj) {
    ObjArray* objL = (ObjArray*)obj;
    if (objL->array == NULL || (objL->length - objL->free) < 1) return false;
    Obj* data1 = objL->array[0];
    if (data1 != NULL) {
        if (GetType(data1) == TYPE_AVL_TREE) return true;
        else if (GetType(data1) == TYPE_HASH_TABLE) return true;
    }
    return false;
}


bool insertIArray (ObjArray* objL, int index, Obj* data) {
    if (objL == NULL || data == NULL) return false;
    if (index < 0 || index >= objL->length) return false;
    if (objL->array[index] != NULL) freeObjs(objL->array[index]);
    assignData(&objL->array[index], data);
    return true;
}

// Inserta un elemento en la lista
bool insertArray(Obj* obj, Obj* data) {
    ObjArray* objL = (ObjArray*)obj;
    if (objL == NULL || data == NULL) return false;    
    if (objL->free < 1) {
        initvaluesArray(objL, true);
    }
    size_t index = objL->length - objL->free;
    objL->free--;
    return insertIArray(objL, index, data);
}

// Obtiene el tamaño actual del array (elementos ocupados)
int getArraySize(Obj* obj) {
    ObjArray* objL = (ObjArray*)obj;
    if (objL == NULL) return 0;
    return (objL->length - objL->free);
}

// Busca un elemento en la lista
Obj* searchArray(Obj* obj, int index) {
    ObjArray* objL = (ObjArray*)obj;
    if (objL == NULL) return NULL;
    if (index < 0) {
        index = objL->length - objL->free + index;
    }
    if (index < getArraySize(obj)) {
        return objL->array[index];
    } else return NULL;
}


// Libera la memoria de la lista
bool freeArray(Obj* obj) {
    ObjArray* objL = (ObjArray*)obj;
    if (objL == NULL) return false;
    for (int i = 0; i < objL->length - objL->free; i++) {
        if(objL->array[i] != NULL) freeObjs(objL->array[i]);
    }
    free(objL->array);
    free(objL);
    return true;
}

// Elimina un elemento del array
Obj* removeArray(Obj* obj, int index) {
    ObjArray* objL = (ObjArray*)obj;
    if (objL == NULL || objL->array == NULL) return NULL;

    int size = getArraySize(obj);
    if (size < 1) return NULL;

    if (index < 0) {
        index = size + index; // índice desde el final
    }
    if (index < 0 || index >= size) return NULL;

    if (isTreeOrTable(obj) && index == 0) return NULL; // no eliminar estructura interna

    Obj* removed = searchArray(obj, index);

    // desplazar elementos hacia la izquierda
    for (int i = index; i < size - 1; i++) {
        objL->array[i] = objL->array[i + 1];
    }
    objL->array[size - 1] = NULL;
    objL->free++;

    return removed;
}

// Elimina y devuelve el último elemento del array (operación pop)
Obj* popArray(Obj* obj) {
    return removeArray(obj, -1);
}