#include "./stuctura.h"

//================ Función para inicializar un nuevo objeto ==================
// de tipo Obj con un valor específico
Obj* newObj(ObjType type, void* as) {
    Obj* res;
    switch (type) {
        case TYPE_NULL:
        case TYPE_BOOL_F:
        case TYPE_BOOL_T:
            res = (Obj*)malloc(sizeof(Obj));
            res->type = type;
            break;
        case TYPE_NUM:
        case TYPE_NUMFL:
            if (!as) return NULL;
            Nativo *objN = (Nativo *)malloc(sizeof(Nativo));
            objN->type = type;
            objN->as = *(As*)as;
            res = (Obj*)objN;
            break;
        default:
        break;
    }
    return res;
}

// Función para inicializar el valor de un objeto de clave valor
Entry newEntry(const char* key, Obj* data) {
    Entry entry;
    entry.key = key;
    entry.data = data;
    return entry;
}

Obj* newObjString (char * str) {
    ObjString *valueObj = (ObjString*)malloc(sizeof(ObjString));
    valueObj->obj.type = OBJ_STRING;
    valueObj->obj.reference = 0;
    valueObj->length = strlen(str);
    valueObj->chars = (char*)malloc(strlen(str) + 1);
    strcpy(valueObj->chars, str);
    return valueObj;
}

char* getString(Obj* obj) {
    ObjString* obtS = (ObjString*)obj;
    return obtS->chars;
}