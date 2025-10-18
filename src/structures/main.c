#include "./stuctura.h"

//================ Función para inicializar un nuevo objeto ==================
// de tipo Obj con un valor específico
Obj* newObj(ObjType type, As* as) {
    Obj* res = NULL;
    char cat = getTypeCategory(type);
    if (cat == IS_BOOL) {
        res = (Obj*)malloc(sizeof(Obj));
        if (!res) return NULL;
        res->type = type;
    } else if (cat == IS_NATIVE) {
        if (!as) return NULL;
        Native *objN = (Native *)malloc(sizeof(Native));
        if (!objN) return NULL;
        objN->type = type;
        objN->as = *as;
        res = (Obj*)objN;
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


bool freeEntry(Entry* entry) {
    if (entry == NULL) return false;
    if (entry->key) free(entry->key);
    if (entry->data) freeObjs(entry->data);  // liberar el objeto de datos
    if (entry->next) freeObjs(entry->next);  // liberar el siguiente objeto
    free(entry);
    return true;
}


bool setObjString (Obj* objString, const char* str) {
    if (objString == NULL || objString->type != TYPE_STRING) return false;
    ObjString* objStr = (ObjString*)objString;
    objStr->obj.type = TYPE_STRING;
    objStr->length = strlen(str);
    if (objStr->length >=  objStr->capacity) { // redimensionar
        objStr->capacity = objStr->length + 10; // +10 para el terminador nulo
        objStr->chars = (char*)realloc(objStr->chars, objStr->capacity);
        if (objStr->chars == NULL) return false;
    }
    strcpy(objStr->chars, str);
    return true;
}

Obj* newObjString (char * str) {
    ObjString *valueObj = (ObjString*)malloc(sizeof(ObjString));
    valueObj->obj.type = TYPE_STRING;
    valueObj->obj.reference = 0;
    valueObj->length = strlen(str);
    valueObj->capacity = valueObj->length + 10; // +1 para el terminador nulo
    valueObj->chars = (char*)malloc(valueObj->capacity);
    if (valueObj->chars == NULL) return false;
    strcpy(valueObj->chars, str);
    return (Obj*)valueObj;
}

char* getString(Obj* obj) {
    if (obj == NULL || obj->type != TYPE_STRING) return NULL;
    ObjString* obtS = (ObjString*)obj;
    return obtS->chars;
}

// Función para saver el tamaño de un Srintg
int getStringSize(Obj* obj) {
    ObjString* obtS = (ObjString*)obj;
    return obtS->length;
}

bool freeString(ObjString* objS) {
    if (objS->chars) free(objS->chars);
    free(objS);
    return true;
}