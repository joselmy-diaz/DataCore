#include "./DataCore.h"
#include "stuctura.h"

//================ Función para inicializar un nuevo objeto ==================
// de tipo Obj con un valor específico
Obj* newData(ObjType type, As as) {
    Obj *obj = (Obj *)malloc(sizeof(Obj));
    if (obj == NULL) {
        return NULL;
    }
    obj->type = type;
    obj->as = as;
    return obj;
}

// Función para inicializar el valor de un objeto de clave valor
Entry initEntry(const char* key, Obj* data) {
    Entry entry;
    entry.key = key;
    entry.data = data;
    return entry;
}

Obj* newObjString (char * str) {
    ObjString *valueObj = (ObjString*)malloc(sizeof(ObjString));
    valueObj->obj.as.Num = 0;
    valueObj->obj.type = OBJ_STRING;
    valueObj->length = strlen(str);
    valueObj->chars = (char*)malloc(strlen(str) + 1);
    strcpy(valueObj->chars, str);
    return valueObj;
}


//======================== Función para balidar el uso de un objeto =======================
bool isNative(Obj* obj) {
    if (obj->type == TYPE_NULL) return true;
    if (obj->type == TYPE_BOOL) return true;
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
        data->as.Num++;
        *obj = data;
    } else {
        *obj = newData(data->type, data->as);
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
            if (obj->as.Num <= 1){
                switch (obj->type){
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
            } else obj->as.Num--;
        } else printf("Estructura no encontrada para liberas.");
    }
    return res;
}