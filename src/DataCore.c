#include "./DataCore.h"
#include "stuctura.h"
#include "CoreTypes.h" 

char getTypeCategory(char res) {
    if (res <= TYPE_NUMFL) {
        if (res <= TYPE_BOOL_T) res = IS_BOOL;
        else res = IS_NATIVE;
    } else {
        res = IS_OBJ;
    }
    return res;
}

// busca el tipo de objeto y lo iguala a la estructura de datos
ObjOps* getOps(int8_t type) {
    ObjOps* ops = &ObjOpsList[type - TYPE_AVL_TREE];
    if (ops == NULL || ops->insert == NULL || ops->search == NULL || ops->free == NULL) return NULL;
    return ops;
}

bool insertD(Obj* obj, Entry* data) {
    if (obj == NULL || getTypeCategory(obj->type) != IS_OBJ) return false;

    ObjOps* ops = getOps(obj->type);
    if (ops != NULL) {
        ops->insert(obj, data);
    }
    return true;
}

// Inserta de estrutura de datos punteres si es objeto igualalo y al as sumale 1 
bool assignData(Obj** obj, Obj* data) {
    if (obj == NULL || data == NULL) return false;
    char cat = getTypeCategory(data->type);
    if (cat == IS_OBJ) {
        ObjR* objT = (ObjR*)data;
        objT->reference++;
        *obj = objT;
    } else{
        Native* objT = (Native*)data;
        *obj = newObj(data->type, &objT->as);
    } 
    return true;
}

// Busca un elemento en la lista
Obj* searchD (Obj* obj, const char *key) {
    if (obj == NULL || getTypeCategory(obj->type) != IS_OBJ) return NULL;
    ObjOps* ops = getOps(obj->type);
    if (ops != NULL) {
        return ops->search(obj, key);
    }
}

bool freeObjs(Obj* obj) {
    if (obj == NULL) return true;
    char cat = getTypeCategory(obj->type);
    if (cat == IS_BOOL) {
        free(obj);
        return true;
    } else if (cat == IS_NATIVE) {
        free((Native*)obj);
        return true;
    }

    // Asegúrate de que es un objeto válido
    ObjR* objT = (ObjR*)obj;

    // Si tiene referencias activas, solo reduce el contador
    if (objT->reference > 0) {
        objT->reference--;
        return true;
    }

    // Manejo de liberación basado en el tipo de objeto
    switch (obj->type) {
        case TYPE_ENTRY:      freeEntry(obj);      break;
        case TYPE_STRING:     freeString(obj);     break;
        default: 
            ObjOps* ops = getOps(obj->type);
            if (ops != NULL) ops->free(obj); // Tipo desconocido
        break;
    }
    return true;
}