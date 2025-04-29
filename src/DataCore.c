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

bool (*insertObjR[])(Obj*, Entry) = {
    (bool (*)(Obj*, Entry))insertTH,
    (bool (*)(Obj*, Entry))insertAVL,
    (bool (*)(Obj*, Entry))insertLByKey,
};
// return insertLByKey(objT, data);

bool insertD(Obj* obj, Entry data) {
    if (obj == NULL || getTypeCategory(obj->type) != IS_OBJ) return false;
    return insertObjR[obj->type - 8](obj, data);
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

Obj* (*searchObjR[])(Obj*, const char *key) = {
    (Obj* (*)(Obj*, const char *))searchTH,
    (Obj* (*)(Obj*, const char *))searchTree,
    (Obj* (*)(Obj*, const char *))searchArray,
};

// Busca un elemento en la lista
Obj* searchD (Obj* obj, const char *key) {
    if (obj == NULL || getTypeCategory(obj->type) != IS_OBJ) return NULL;
    return searchObjR[obj->type - 8](obj, key);
}

Obj* searchDIndex (Obj* obj, int index) {
    Obj* RData = NULL;
    if (obj->type == TYPE_ARRAY) {
        ObjCon* objT = (ObjCon*)obj;
        RData = searchArray(objT, index);
    }
    return RData;
}

bool (*freeObjR[])(Obj*) = {
    (bool (*)(Obj*))freeEntry,
    (bool (*)(Obj*))freeString,
    (bool (*)(Obj*))freeTH,
    (bool (*)(Obj*))freeTR,
    (bool (*)(Obj*))freeArray
};


bool freeObjs(Obj* obj) {
    if (obj == NULL) return true;
    char cat = getTypeCategory(obj->type);
    if (cat == IS_BOOL) {
        free(obj);
        return true;
    }else if (cat == IS_NATIVE) {
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
    freeObjR[obj->type - 6](obj);
    return true;
}