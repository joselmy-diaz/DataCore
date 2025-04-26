#include "./stuctura.h"


// Inicializar la tabla hash
Obj *initTH(int size) {
    ObjTebleH *table = (ObjTebleH *)malloc(sizeof(ObjTebleH));
    if (!table) return NULL;
    table->obj.type = TYPE_HASH_TABLE;
    table->obj.reference = 0;
    table->entries = (Entry **)calloc(size, sizeof(Entry *));
    table->length = size;
    for (int i = 0; i < size; i++) {table->entries[i] = NULL; }
    return &table->obj;
}

// Insertar un elemento en la tabla
void insertTH(ObjTebleH *table, Entry ent) {
    if (table->entries == NULL) return;
    unsigned int index = hash(ent.key) % table->length;
    Entry *new_entry = (Entry *)malloc(sizeof(Entry));
    if (!new_entry) return;

    new_entry->key = strdup(ent.key);
    assignData(&new_entry->data, ent.data);
    new_entry->next = table->entries[index];
    table->entries[index] = new_entry;
}

// Buscar un elemento en la tabla
Obj* searchTH(ObjTebleH *table, const char *key) {
    if (table->entries == NULL) return NULL;
    unsigned int index = hash(key) % table->length;
    Entry *entry = table->entries[index];
    while (entry != NULL) {
        if (strcmp(entry->key, key) == 0) return entry->data;
        entry = entry->next;
    }
    return NULL; // Valor no encontrado
}

void freeEnT(Entry* entry){
    // if (entry == NULL) return;
    if (entry->next != NULL) freeEnT(entry->next);
    free(entry->key);
    freeObjs(entry->data);
    free(entry);
}

// Liberar la memoria de la tabla
bool freeTH (ObjTebleH *table) {
    if (table == NULL) return true;
    for (int i = 0; i < table->length; i++) {
        if (table->entries[i] != NULL) {
            freeEnT(table->entries[i]);
        }
    }
    free(table->entries);
    table->entries = NULL;
    free(table);
    return true;
}