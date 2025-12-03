#include "DataCore.h"
#include "structure.h"
#include <time.h>
#include <stdio.h>
#include <stdalign.h>
#include <stdlib.h>

//======================== prurba de estructuras =======================

void chargeObj(Obj* obj) {
    if (obj == NULL) return;

    // Crear e insertar entradas - NO liberar después porque el array toma propiedad
    Obj* e1 = newEntry("data1", newObj(TYPE_BOOL_F, NULL));
    Obj* e2 = newEntry("data2", newObjString("Hola mundo1"));
    Obj* e3 = newEntry("data3", newObj(TYPE_BOOL_T, NULL));
    Obj* e4 = newEntry("data4", newObjString("Hola"));

    insertArray(obj, e1);
    insertArray(obj, e2);
    insertArray(obj, e3);
    insertArray(obj, e4);

    // Agregar más datos dinámicamente
    for (int i = 5; i <= 10; i++) {
        char key[32];
        snprintf(key, sizeof(key), "data%d", i);

        Obj* entry = NULL;
        if (i % 3 == 0) {
            As asValue = { .Num = i * 2 };
            entry = newEntry(key, newObj(TYPE_NUM, &asValue));
        } else if (i % 3 == 1) {
            As asValue = { .NumF = i * 1.3 };
            entry = newEntry(key, newObj(TYPE_NUMFL, &asValue));
        } else {
            entry = newEntry(key, newObjString("Texto dinámico "));
        }
        insertArray(obj, entry);
    }
}

int main(int argc, char const *argv[]) {
    clock_t startA, start, end;
    double cpu_time_used;
    printf("tamano de Data es: %zu\n", sizeof(Native*));
    printf("Alignment: %zu bytes\n", sizeof(Native));

    // Punto de control: Inicio de inicialización
    printf("\nInicializando objetos...\n");

    // Inicialización de objetos

    start = clock();
    startA = start;
    Obj *lis = initArray(100);
    // Inserciones en la lista
    chargeObj(lis);
    printObjf(lis);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Tiempo de inserción en la lista: %f segundos\n", cpu_time_used);

    // Punto de control: Inicialización de tablas
    printf("\nInicializando tabla hash y árbol...\n");
    start = clock();
    Obj *table = initTH(100);
    Obj *tree = initTR();

    // Inserciones en la tabla hash
    printf("\nInsertando en la tabla hash...\n");
    start = clock();
    for (int i = 0; i < getArraySize(lis); i++) {
        Entry* entry = (Entry*)searchArray(lis, i);
        if (entry != NULL) {
            insertD(table, entry);
        }
    }
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Tiempo de inserción en la tabla hash: %f segundos\n", cpu_time_used);
    start = clock();

    // Inserciones en el árbol AVL
    printf("\nInsertando en el árbol AVL...\n");
    start = clock();
    for (int i = 0; i < getArraySize(lis); i++) {
        Entry* entry = (Entry*)searchArray(lis, i);
        if (entry != NULL) {
            insertD(tree, entry);
        }
    }
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Tiempo de inserción en el árbol AVL: %f segundos\n", cpu_time_used);
    start = clock();

    // Imprimir valores en la tabla hash
    printf("\nValores en la tabla:\n");
    printObjf(table);
    printf("\n");
    // Imprimir valores en el árbol AVL
    printf("\nValores en el árbol AVL:\n");
    printObjf(tree);
    
    // Imprimir valores en la lista
    printf("\nValores en la lista:\n");
    printObjf(lis);


    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Belosidad de Busqueda en todas las estructuras: %f segundos\n", cpu_time_used);
    start = clock();


    // Liberar memoria
    printf("\nLiberando memoria...\n");
    bool res = freeObjs(lis);
    if (!res) printf("Error al liberar lis.\n");
    
    freeObjs(table);
    freeObjs(tree);
    
    printf("\nMedición del tiempo total...\n");
    end = clock();
    cpu_time_used = ((double)(end - startA)) / CLOCKS_PER_SEC;
    printf("Tiempo total de ejecución: %f segundos\n", cpu_time_used);
    return 0;
}
