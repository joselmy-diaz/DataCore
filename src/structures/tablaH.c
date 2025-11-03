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
    Entry* new_entry = (Entry*)obj;
    ObjTableH *hashTable = (ObjTableH *)table;
    if (hashTable->entries == NULL) return false;
    unsigned int index = hash(new_entry->key) % hashTable->length;
    if (!new_entry) return false;

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
    for (int i = 0; i < hashTable->length; i++) {
        Entry *entry = hashTable->entries[i];
        while (entry != NULL) {
            insertArray(arr, entry);
            entry = entry->next;
        }
    }
    return arr;
}

void freeEnT(Entry* entry){
    if (entry == NULL) return;
    if (entry->next != NULL) freeEnT(entry->next);
    if (entry->key) free(entry->key);
    if (entry->data) freeObjs(entry->data);
    free(entry);
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