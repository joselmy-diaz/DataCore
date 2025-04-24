#include "./DataCore.h"
#include "stuctura.h"

//======================== Función para balidar el uso de un objeto =======================
bool isNative(Obj* obj) {
    if (obj->type == TYPE_NULL) return true;
    if (obj->type == TYPE_BOOL_F) return true;
    if (obj->type == TYPE_BOOL_T) return true;
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
    if (obj->type == OBJ_STRING) return true;
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
                return insertL(objT, &data.data);
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
    bool res = false;
    if (obj == NULL) return res;
    if (isNative(obj)) {
        free(obj);
        res = true;
    } else {
        if(isObj(obj)){
            ObjR* objT = (ObjR*)obj;
            if (objT->reference <= 1){
                switch (objT->type){
                    case OBJ_STRING:
                        printf(". Listo para liberas.");
                        free((ObjString*)obj);
                        break;
                    case OBJ_HASH_TABLE:
                        res = freeTH((ObjTebleH*)obj);
                        break;
                    case OBJ_AVL_TREE:
                        res = freeTR((ObjTree*)obj);
                    break;
                    case OBJ_ARRAY:
                        res = freeArray((ObjCon*)obj);
                    break;
                    default:
                        printf("Estructura no encontrada para liberas.");
                        break;
                }
            } else objT->reference--;
        } else printf("Estructura no encontrada para liberas.");
    }
    return res;
}