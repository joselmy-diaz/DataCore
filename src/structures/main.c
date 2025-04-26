#include "./stuctura.h"

//================ Función para inicializar un nuevo objeto ==================
// de tipo Obj con un valor específico
Obj* newObj(ObjType type, As* as) {
    Obj* res = NULL;
    switch (type) {
        case TYPE_NULL:
        case TYPE_BOOL_F:
        case TYPE_BOOL_T:
            res = (Obj*)malloc(sizeof(Obj));
            if (!res) return NULL;
            res->type = type;
            break;

        case TYPE_NUM:
        case TYPE_NUMFL:
            if (!as) return NULL;
            Nativo *objN = (Nativo *)malloc(sizeof(Nativo));
            if (!objN) return NULL;
            objN->type = type;
            objN->as = *as; // Liberamos as después de copiarlo
            res = (Obj*)objN;
            break;

        default:
            // Tipo desconocido, no se hace nada
            break;
    }
    return res;
}


// Función para inicializar el valor de un objeto de clave valor
Entry* newEntry(const char* key, Obj* data) {
    Entry* entry = (Entry*)malloc(sizeof(Entry));
    entry->obj.type = TYPE_ENTRY;
    entry->obj.reference = 0;
    entry->next = NULL;
    entry->key = strdup(key);
    entry->data = data;
    return entry;
}

Obj* newObjString (char * str) {
    ObjString *valueObj = (ObjString*)malloc(sizeof(ObjString));
    valueObj->obj.type = OBJ_STRING;
    valueObj->obj.reference = 0;
    valueObj->length = strlen(str);
    valueObj->capacity = valueObj->length + 1; // +1 para el terminador nulo
    valueObj->chars = (char*)malloc(valueObj->capacity);
    if (valueObj->chars == NULL) return NULL;
    strcpy(valueObj->chars, str);
    return (Obj*)valueObj;
}

// void fereeString(Obj* obj) {
//     ObjString* obtS = (ObjString*)obj;
//     if (obtS->chars) free(obtS->chars);
//     free(obtS);
// }

// void freeEntry(Entry* entry) {
//     if (entry == NULL) return;
//     if (entry->key) free(entry->key);
//     if (entry->data) freeObjs(entry->data);  // liberar el objeto de datos
//     if (entry->next) freeObjs(entry->next);  // liberar el siguiente objeto
//     free(entry);
// }

char* getString(Obj* obj) {
    ObjString* obtS = (ObjString*)obj;
    return obtS->chars;
}