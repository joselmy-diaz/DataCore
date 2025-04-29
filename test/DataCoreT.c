#include "DataCore.h"
#include "stuctura.h"
#include <time.h>
#include <stdio.h>
#include <stdalign.h>
#include <stdlib.h>

//======================== prurba de estructuras =======================

void chargeObj(Obj* obj) {
    if (obj == NULL) return;

    // Agregar las primeras 11 entradas existentes

    Entry* e1 = newEntry("data1", newObj(TYPE_BOOL_F, NULL));
    Entry* e2 = newEntry("data2", newObjString("Hola mundo1"));
    Entry* e3 = newEntry("data3", newObj(TYPE_BOOL_T, NULL));
    Entry* e4 = newEntry("data4", newObjString("Hola"));

    // // Insertar las primeras 11 entradas
    insertArray(obj, e1);
    freeObjs(e1);
    insertArray(obj, e2);
    freeObjs(e2);
    insertArray(obj, e3);
    freeObjs(e3);
    insertArray(obj, e4);
    freeObjs(e4);

    // Agregar más datos dinámicamente
    for (int i = 5; i <= 1000; i++) {
        char key[32]; // Increase buffer size to handle larger numbers safely
        snprintf(key, sizeof(key), "data%d", i); // Ensure format string is correct

        if (i % 3 == 0) {
            As asValue = { .Num = i * 2 };
            Entry* entry = newEntry(key, newObj(TYPE_NUM, &asValue));
            insertArray(obj, entry);
            freeObjs(entry);
        } else if (i % 3 == 1) {
            As asValue = { .Num = i * 1.3 };
            Entry* entry = newEntry(key, newObj(TYPE_NUM, &asValue));
            insertArray(obj, entry);
            freeObjs(entry);
        } else {
            Entry* entry = newEntry(key, newObjString("Texto dinámico"));
            insertArray(obj, entry);
            freeObjs(entry);
        }
    }
}

int main(int argc, char const *argv[]) {
    clock_t startA, start, end;
    double cpu_time_used;
    printf("tamano de Data es: %zu\n", sizeof(ObjString));
    printf("Alignment: %zu bytes\n", alignof(Obj));

    // Punto de control: Inicio de inicialización
    printf("\nInicializando objetos...\n");

    // Inicialización de objetos

    start = clock();
    startA = start;
    Obj *lis = initArray(100, false);
    // Inserciones en la lista
    chargeObj(lis);
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
    for (int i = 0; i < getziseL(lis); i++) {
        Entry* entry = (Entry*)searchArray(lis, i);
        if (entry != NULL) {
            insertD(table, *entry);
        }
    }
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Tiempo de inserción en la tabla hash: %f segundos\n", cpu_time_used);
    start = clock();

    // Inserciones en el árbol AVL
    printf("\nInsertando en el árbol AVL...\n");
    start = clock();
    for (int i = 0; i < getziseL(lis); i++) {
        Entry* entry = (Entry*)searchArray(lis, i);
        if (entry != NULL) {
            insertD(tree, *entry);
        }
    }
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Tiempo de inserción en el árbol AVL: %f segundos\n", cpu_time_used);
    start = clock();

    // Imprimir valores en la tabla hash
    printf("\nValores en la tabla:\n");
    for (int i = 0; i < getziseL(lis); i++) {
        char key[32]; // Increase buffer size to handle larger numbers safely
        snprintf(key, sizeof(key), "data%d", i+1);
        printf(", %s:", key);
        printObj(searchD(table, key));
        printf(" ");
    }
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
    freeObjs(table);
    freeObjs(tree);
    if (!res)printf("Estructura no encontrada para liberar.");
    printf("\nMedición del tiempo total...\n");
    end = clock();
    cpu_time_used = ((double)(end - startA)) / CLOCKS_PER_SEC;
    printf("Tiempo total de ejecución: %f segundos\n", cpu_time_used);
    return 0;
}
