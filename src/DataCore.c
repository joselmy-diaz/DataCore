#include "./DataCore.h"
#include "stuctura.h"

char getTypeCategory(char res) {
    if (res <= 5) {
        if (res <= 2) res = IS_BOOL;
        else res = IS_NATIVE;
    } else {
        res = IS_OBJ;
    }
    return res;
}

bool insertD(Obj* obj, Entry data) {
    if (obj == NULL || getTypeCategory(obj->type) != IS_OBJ) return false;

    switch (obj->type) {
        case TYPE_HASH_TABLE:  insertTH((ObjCon*)obj, data); break;
        case TYPE_AVL_TREE:    return insertAVL((ObjCon*)obj, data); break;
        case TYPE_ARRAY:       return insertLByKey((ObjCon*)obj, data);
        default:               return false;
    }
    return false;
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

    switch (obj->type) {
        case TYPE_HASH_TABLE:
            return searchTH(obj, key);
        case TYPE_AVL_TREE:
            return searchTree(obj, key);
        case TYPE_ARRAY:
            return searchLByKey(obj, key);
        default:
            return NULL;
    }
}

Obj* searchDIndex (Obj* obj, int index) {
    Obj* RData = NULL;
    if (obj->type == TYPE_ARRAY) {
        ObjCon* objT = (ObjCon*)obj;
        RData = searchArray(objT, index);
    }
    return RData;
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
        case TYPE_HASH_TABLE: freeTH(obj);         break;
        case TYPE_AVL_TREE:   freeTR(obj);         break;
        case TYPE_ARRAY:      freeArray(obj);      break;
        default: return false; // Tipo desconocido
    }
    return true;
}