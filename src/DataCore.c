#include "./DataCore.h"
#include "structure.h"
#include "CoreTypes.h" 
#include <stdatomic.h>
#include <sched.h>

char getTypeCategory(char res) {
    if (res <= TYPE_NUMFL) {
        if (res <= TYPE_BOOL_T) res = IS_BOOL;
        else res = IS_NATIVE;
    } else {
        res = IS_OBJ;
    }
    return res;
}

#define MASCARA_LOCK 0x80 // 10000000 binario
#define MASCARA_DATOS 0x7F
void bloqueaObj (Obj* obj) {
    unsigned char valor_anterior;
    do {
        // Tenta establecer el bit de LOCK y devuelve el valor que tenía ANTES de la operación.
        valor_anterior = atomic_fetch_or(&obj->type, (unsigned char)MASCARA_LOCK);
        // Si el valor anterior ya tenía el bit de LOCK encendido, ¡FALLAMOS!
        if (!(valor_anterior & MASCARA_LOCK)) return; // El bit de bloqueo estaba LIBRE (0). ¡El lock fue adquirido con éxito!
        
        sched_yield(); 
    } while (1);
}

void desbloqueaObj (Obj* obj) {
    if (obj == NULL) return;
    atomic_fetch_and(&obj->type, (unsigned char)MASCARA_DATOS);
}


// busca el tipo de objeto y lo iguala a la estructura de datos
ObjOps* getOps(int8_t type) {
    ObjOps* ops = &ObjOpsList[type - TYPE_AVL_TREE];
    if (ops == NULL || ops->insert == NULL || ops->search == NULL || ops->free == NULL) return NULL;
    return ops;
}

bool insertD(Obj* obj, Entry* data) {
    if (obj == NULL || getTypeCategory(GetType(obj)) != IS_OBJ) return false;

    ObjOps* ops = getOps(GetType(obj));
    if (ops != NULL) {
        ops->insert(obj, data);
    }
    return true;
}

// Inserta de estrutura de datos punteres si es objeto igualalo y al as sumale 1 
bool assignData(Obj** obj, Obj* data) {
    if (obj == NULL || data == NULL) return false;
    char cat = getTypeCategory(GetType(data));
    if (cat == IS_OBJ) {
        ObjR* objT = (ObjR*)data;
        objT->reference++;
        *obj = objT;
    } else{
        Native* objT = (Native*)data;
        *obj = newObj(GetType(data), &objT->as);
    } 
    return true;
}

// Busca un elemento en la lista
Obj* searchD (Obj* obj, const char *key) {
    if (obj == NULL || getTypeCategory(GetType(obj)) != IS_OBJ) return NULL;
    ObjOps* ops = getOps(GetType(obj));
    if (ops != NULL) {
        return ops->search(obj, key);
    }
}

bool freeObjs(Obj* obj) {
    if (obj == NULL) return true;
    char cat = getTypeCategory(GetType(obj));
    if (cat == IS_BOOL) {
        free(obj);
        return true;
    } else if (cat == IS_NATIVE) {
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

    // Manejo de liberación basado en el tipo de objeto
    switch (GetType(obj)) {
        case TYPE_ENTRY:      freeEntry(obj);      break;
        case TYPE_STRING:     freeString(obj);     break;
        default: 
            ObjOps* ops = getOps(GetType(obj));
            if (ops != NULL) ops->free(obj); // Tipo desconocido
        break;
    }
    return true;
}