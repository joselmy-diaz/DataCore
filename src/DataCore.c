#include "DataCore.h"
#include "buffer.h"
#include <stdatomic.h>
#include <sched.h>
#include <stdint.h>

int8_t getTypeCategory(int8_t res) {
    if (res <= TYPE_NUMFL) {
        if (res <= TYPE_BOOL_T) return IS_BOOL;
        return IS_NATIVE;
    }
    return IS_OBJ;
}

ObjOps ObjOpsList[] = {
    {insertArray, NULL},       // TYPE_ARRAY = 8
    {insertAVL, searchTree},   // TYPE_AVL_TREE = 9
    {insertTH, searchTH},      // TYPE_HASH_TABLE = 10
};

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




bool insertD(Obj* obj, Entry* data) {
    if (obj == NULL || getTypeCategory(GetType(obj)) != IS_OBJ) return false;
    TypeObjs type = GetType(obj);
    ObjOps ops = ObjOpsList[type - TYPE_ARRAY];
    if (ops.insert == NULL) return false;
    return ops.insert(obj, data);
}

// Busca un elemento en la lista
Obj* searchD (Obj* obj, const char *key) {
    if (obj == NULL || getTypeCategory(GetType(obj)) != IS_OBJ) return NULL;
    TypeObjs type = GetType(obj);
    ObjOps ops = ObjOpsList[type - TYPE_ARRAY];
    if (ops.search == NULL) return NULL;
    return ops.search(obj, key);
}

// Inserta de estrutura de datos punteres si es objeto igualalo y al as sumale 1 
bool assignData(Obj** obj, Obj* data) {
    if (obj == NULL || data == NULL) return false;
    char cat = getTypeCategory(GetType(data));
    if (cat == IS_OBJ) {
        ObjR* objT = (ObjR*)data;
        objT->reference++;
        *obj = (Obj*)objT;
    } else{
        Native* objT = (Native*)data;
        *obj = newObj(GetType(data), &objT->as);
    } 
    return true;
}

// ============================================================================
// Sistema de Liberación de Memoria con Tabla de Destructores
// ============================================================================


FreeFn destructorTable[MAX_TYPES] = {
    #define X(name, func) [name] = func,
        OBJ_TYPES_X
    #undef X
};

/**
 * Libera un objeto y gestiona su reference counting
 * @param obj Objeto a liberar
 * @return true si se liberó correctamente, false en caso de error
 */
bool freeObjs(Obj* obj) {
    if (obj == NULL) return true;

    // Paso 1: Manejar tipos sin reference counting (IS_BOOL, IS_NATIVE)
    char cat = getTypeCategory(GetType(obj));
    if (cat == IS_BOOL) {
        free(obj);
        return true;
    } 
    
    if (cat == IS_NATIVE) {
        free((Native*)obj);
        return true;
    }

    ObjR* objT = (ObjR*)obj;
    // Si hay múltiples referencias, solo decrementar el contador
    if (objT->reference > 1) {
        objT->reference--;
        return true; 
    }
    
    if (objT->reference == 1) {
        objT->reference--;
    }
    int type = GetType(obj);
    
    // Verificación de seguridad: tipo válido y con destructor registrado
    if (type < 0 || type >= MAX_TYPES || destructorTable[type] == NULL) {
        fprintf(stderr, "Warning: Unknown object type %d\n", type);
        return false;
    }

    // Llamar al destructor específico del tipo
    return destructorTable[type](obj);
}