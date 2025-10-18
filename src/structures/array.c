#include "stuctura.h"
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
        if (data1->type == TYPE_AVL_TREE) return true;
        else if (data1->type == TYPE_HASH_TABLE) return true;
    }
    return false;
}


// Inserta un elemento en la lista
bool insertLByKey(Obj* obj, Entry* data) {
    ObjArray* objL = (ObjArray*)obj;
    if (1 <= objL->length - objL->free) {
        if (isTreeOrTable(obj)) {
            Obj* data1 = objL->array[0];
            // return insertD(data1, data);
        } else {
            insertArray(objL, data->data);
        }
        //printf("No se puede insectar en este tipo de estructura 2");
    } else return false;
    return true;
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


// Elimina y devuelve el último elemento del array (operación pop)
Obj* popArray(Obj* obj) {
    ObjArray* objL = (ObjArray*)obj;
    if (objL == NULL || objL->array == NULL) return NULL;
    
    // Calcular el índice del último elemento
    int lastIndex = objL->length - objL->free - 1;
    
    // Verificar que haya elementos para hacer pop
    if (lastIndex < 0) return NULL;
    
    // Si es un array con un árbol o tabla hash en la posición 0, ajustar el índice
    if (isTreeOrTable(obj) && lastIndex == 0) return NULL; // No permitir eliminar la estructura interna
    
    // Obtener el último elemento
    Obj* lastElement = objL->array[lastIndex];
    
    // Eliminar la referencia al elemento (sin liberarlo)
    objL->array[lastIndex] = NULL;
    objL->free++;
    
    return lastElement;
}