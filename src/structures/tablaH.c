#include "./structure.h"


// Inicializar la tabla hash
Obj *initTH(int size) {
    ObjTableH *table = (ObjTableH *)malloc(sizeof(ObjTableH));
    if (!table) return NULL;
    table->obj.type = TYPE_HASH_TABLE;
    table->obj.reference = 0;
    table->entries = (Entry **)calloc(size, sizeof(Entry *));
    table->length = size;
    for (int i = 0; i < size; i++) {table->entries[i] = NULL; }
    return (Obj*)table;
}

// Insertar un elemento en la tabla
bool insertTH(Obj *table, Obj* obj) {
    Entry* source_entry = (Entry*)obj;
    ObjTableH *hashTable = (ObjTableH *)table;
    if (hashTable->entries == NULL || !source_entry) return false;
    
    // Primero verificar si la clave ya existe
    unsigned int index = hash(source_entry->key) % hashTable->length;
    Entry* existing = hashTable->entries[index];
    
    while (existing) {
        if (strcmp(existing->key, source_entry->key) == 0) {
            // Clave encontrada - actualizar el valor en lugar de insertar duplicado
            if (source_entry->data) {
                char cat = getTypeCategory(GetType(source_entry->data));
                if (cat == IS_NATIVE) {
                    // Para tipos nativos, siempre crear una nueva copia y reemplazar
                    Native* src = (Native*)source_entry->data;
                    Native* copy = (Native*)malloc(sizeof(Native));
                    copy->type = src->type;
                    copy->as = src->as;
                    existing->data = (Obj*)copy;
                } else {
                    // Para objetos y booleanos, simplemente reemplazar
                    existing->data = source_entry->data;
                }
            }
            return true;
        }
        existing = existing->next;
    }
    
    // No existe - crear nueva entrada
    Entry* new_entry = newEntry(source_entry->key, source_entry->data);
    
    if (source_entry->data) {
        char cat = getTypeCategory(GetType(source_entry->data));
        if (cat == IS_OBJ) {
            // Para objetos referenciales, compartir el puntero e incrementar referencia
            ObjR* objData = (ObjR*)source_entry->data;
            objData->reference++;
        } else if (cat == IS_NATIVE) {
            // Para tipos nativos, crear una copia
            Native* src = (Native*)source_entry->data;
            Native* copy = (Native*)malloc(sizeof(Native));
            copy->type = src->type;
            copy->as = src->as;
            new_entry->data = (Obj*)copy;
        } else if (cat == IS_BOOL) {
            // Para booleanos, crear una copia
            Obj* copy = (Obj*)malloc(sizeof(Obj));
            copy->type = source_entry->data->type;
            new_entry->data = copy;
        }
    }
    
    new_entry->next = hashTable->entries[index];
    hashTable->entries[index] = new_entry;
    return true;
}

// Buscar un elemento en la tabla
Obj* searchTH(Obj *table, const char *key) {
    ObjTableH *hashTable = (ObjTableH *)table;
    if (hashTable->entries == NULL) return NULL;
    unsigned int index = hash(key) % hashTable->length;
    Entry *entry = hashTable->entries[index];
    while (entry != NULL) {
        if (strcmp(entry->key, key) == 0) return entry->data;
        entry = entry->next;
    }
    return NULL; // Valor no encontrado
}

/* --- Recorrer --- */
Obj* recorrerTH(Obj *table) {
    ObjTableH *hashTable = (ObjTableH *)table;
    Obj *arr = initArray(5);
    if (hashTable->entries == NULL) return NULL;
    ObjArray* objL = (ObjArray*)arr;
    for (int i = 0; i < hashTable->length; i++) {
        Entry *entry = hashTable->entries[i];
        while (entry != NULL) {
            // Insertar directamente sin incrementar referencias (solo para visualización)
            if (objL->free < 1) {
                initvaluesArray(objL, true);
            }
            size_t index = objL->length - objL->free;
            objL->free--;
            objL->array[index] = (Obj*)entry;
            entry = entry->next;
        }
    }
    return arr;
}

bool freeEnT(Entry* entry){
    if (entry == NULL) return true;
    // Primero liberar el siguiente nodo en la cadena
    if (entry->next != NULL) {
        freeEnT(entry->next);
        entry->next = NULL;
    }
    // Liberar la key (copia local)
    if (entry->key) {
        free(entry->key);
        entry->key = NULL;
    }
    // Liberar/decrementar los datos compartidos
    if (entry->data) {
        freeObjs(entry->data);
        entry->data = NULL;
    }
    free(entry);
    return true;
}

// Liberar la memoria de la tabla
bool freeTH (Obj *table) {
    ObjTableH *hashTable = (ObjTableH *)table;
    if (hashTable == NULL) return true;
    for (int i = 0; i < hashTable->length; i++) {
        if (hashTable->entries[i] != NULL) {
            freeEnT(hashTable->entries[i]);
        }
    }
    free(hashTable->entries);
    hashTable->entries = NULL;
    free(hashTable);
    return true;
}