#include "./DataCore.h"
#include "stuctura.h"

//======================== Función para balidar el uso de un objeto =======================
bool isNative(Obj* obj) {
    if (obj->type == TYPE_NULL) return true;
    if (obj->type == TYPE_NUM) return true;
    if (obj->type == TYPE_NUMFL) return true;
    return false;
}

// verifica si es una estructura de tipo Datas
bool isObjD(Obj* obj) {
    if (obj->type == OBJ_HASH_TABLE) return true;
    if (obj->type == OBJ_ARRAY) return true;
    if (obj->type == OBJ_AVL_TREE) return true;
    return false;
}
// verifica si es una estructura de tipo Obj
bool isObj(Obj* obj) {
    if (isObjD(obj)) return true;
    if (obj->type == TYPE_BOOL_F) return true;
    if (obj->type == TYPE_BOOL_T) return true;
    if (obj->type == OBJ_STRING) return true;
    if (obj->type == TYPE_ENTRY) return true;
    return false;
}

// Inserta un elemento en la lista 
bool insertD(Obj* obj, Entry data) {
    if (obj == NULL && !isObjD(obj)) return false;

    switch (obj->type) {
        case OBJ_HASH_TABLE: {
            ObjTebleH* objT = (ObjTebleH*)obj;
            insertTH(objT, data);
            return true;
        }
        case OBJ_AVL_TREE: {
            ObjTree* objT = (ObjTree*)obj;
            objT->root = insertAVL(objT->root, data);
            return true;
        }
        case OBJ_ARRAY: {
            ObjCon* objT = (ObjCon*)obj;
            if (data.key == NULL) 
                return insertL(objT, &data);
            else 
                return insertLByKey(objT, data);
        }
        default:
            return false;
    }
}

// Inserta de estrutura de datos punteres si es objeto igualalo y al as sumale 1 
bool assignData(Obj** obj, Obj* data) {
    if (obj == NULL || data == NULL) return false;
    if (isObj(data)) {
        ObjR* objT = (ObjR*)data;
        objT->reference++;
        *obj = objT;
    } else {
        Nativo* objT = (Nativo*)data;
        *obj = newObj(data->type, &objT->as);
    }
    return true;
}

// Busca un elemento en la lista
Obj* searchD (Obj* obj, const char *key) {
    if (obj == NULL) return NULL;
    if (key == NULL) return NULL;
    Obj* RData = NULL;
    if (obj->type == OBJ_HASH_TABLE) {
        RData = searchTH(obj, key);
    }else if (obj->type == OBJ_AVL_TREE) {
        RData = searchTree(obj, key);
    } else if (obj->type == OBJ_ARRAY) {
        RData = searchLByKey(obj, key);
    } else {
        printf("No se puede buscar en este tipo de estructura 1");
    }
    return RData;
}

Obj* searchDIndex (Obj* obj, int index) {
    Obj* RData = NULL;
    if (obj->type == OBJ_ARRAY) {
        ObjCon* objT = (ObjCon*)obj;
        RData = searchL(objT, index);
    }
    return RData;
}


bool freeObjs(Obj* obj) {
    if (obj == NULL) return true;

    // Si es un objeto nativo, simplemente libera
    if (isNative(obj)) {
        free((Nativo*)obj);
        return true;
    }

    // Asegúrate de que es un objeto válido
    if (!isObj(obj)) return false;

    ObjR* objT = (ObjR*)obj;

    // Si tiene referencias activas, solo reduce el contador
    if (objT->reference > 0) {
        objT->reference--;
        return true;
    }

    // Ahora libera según el tipo del objeto
    switch (objT->type) {
        case TYPE_ENTRY: {
            Entry* ent = (Entry*)obj;
            if (ent->key) free(ent->key);
            if (ent->next) freeObjs((Obj*)ent->next);  // usar recursión si next es otro objeto
            free(ent);
            return true;
        }
        case OBJ_STRING: {
            ObjString* objS = (ObjString*)obj;
            if (objS->chars) free(objS->chars);
            free(objS);
            return true;
        }
        case OBJ_HASH_TABLE:
            return freeTH((ObjTebleH*)obj);
        case OBJ_AVL_TREE:
            return freeTR((ObjTree*)obj);
        case OBJ_ARRAY:
            return freeArray((ObjCon*)obj);
        default:
            return false;
    }
}
