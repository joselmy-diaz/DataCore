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

    // Insertar las primeras 11 entradas
    insertL(obj, e1);
    insertL(obj, e2);
    insertL(obj, e3);
    insertL(obj, e4);

    // Agregar más datos dinámicamente hasta llegar a 100 entradas
    // for (int i = 5; i <= 10; i++) {
    //     char key[32]; // Increase buffer size to handle larger numbers safely
    //     snprintf(key, sizeof(key), "data%d", i); // Ensure format string is correct

    //     if (i % 3 == 0) {
    //         As asValue = (As){ .Num = i * 2 };
    //         Entry* entry = newEntry(key, newObj(TYPE_NUM, &asValue));
    //         insertL(obj, entry);
    //     } else if (i % 3 == 1) {
    //         As asValue = { .NumF = i * 1.3 };
    //         insertL(obj, newEntry(key, newObj(TYPE_NUMFL, &asValue)));
    //     } else {
    //         Entry* entry = newEntry(key, newObjString("Texto dinámico"));
    //         insertL(obj, entry);
    //     }
    // }
}

int main(int argc, char const *argv[]) {
    clock_t startA, start, end;
    double cpu_time_used;
    printf("tamano de Data es: %zu\n", sizeof(ObjString));
    printf("Alignment: %zu bytes\n", alignof(Obj));

    // Punto de control: Inicio de inicialización
    printf("\nInicializando objetos...\n");

    // Inicialización de objetos

    startA = clock();
    Obj *lis = initList(6, true);
    // Inserciones en la lista
    start = clock();
    chargeObj(lis);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Tiempo de inserción en la lista: %f segundos\n", cpu_time_used);
    printObjf(lis);

    // // Punto de control: Inicialización de tablas
    // printf("\nInicializando tabla hash y árbol...\n");
    // Obj *table = initTH(100);
    // Obj *tree = initTR();

    // // Inserciones en la tabla hash
    // printf("\nInsertando en la tabla hash...\n");
    // start = clock();
    // for (int i = 0; i < getziseL(lis); i++) {
    //     insertD(table, *(Entry*)searchL(lis, i));
    // }
    // end = clock();
    // cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    // printf("Tiempo de inserción en la tabla hash: %f segundos\n", cpu_time_used);

    // // Inserciones en el árbol AVL
    // printf("\nInsertando en el árbol AVL...\n");
    // start = clock();
    // for (int i = 0; i < getziseL(lis); i++) {
    //     insertD(tree, *(Entry*)searchL(lis, i));
    // }
    // end = clock();
    // cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    // printf("Tiempo de inserción en el árbol AVL: %f segundos\n", cpu_time_used);


    // Puntos de control: Lista
    printf("\nInsertando en la lista...\n");


    // Imprimir valores en la tabla hash
    // printf("\nValores en la tabla:\n");
    // for (int i = 0; i < getziseL(lis); i++) {
    //     printObj(searchD(table, ((Entry*)searchL(lis, i))->key));
    // }

    // Imprimir valores en el árbol AVL
    printf("\nValores en el árbol AVL:\n");
    // printObjf(tree);
    // for (int i = 0; i < sizeof(entries) / sizeof(entries[0]); i++) {
    //     printf("Valor de '%s': ", entries[i].key);
    //     printData(searchD(tree, entries[i].key));
    // }

    // Imprimir valores en la lista
    printf("\nValores en la lista:\n");
    // for (int i = 0; i < getziseL(lis); i++) {
    //     printf("Valor de '%s': ", entries[i].key);
    //     Obj* dataO = searchL(lis, i);
    //     printData(dataO);
    // }
    

    // Liberar memoria
    // freeObjs(table);
    // freeObjs(tree);
    bool res = freeObjs(lis);

    if (res)printf("Estructura no encontrada para liberas.");
    // Punto de control: Medición final
    printf("\nMedición del tiempo total...\n");
    end = clock();
    cpu_time_used = ((double)(end - startA)) / CLOCKS_PER_SEC;
    printf("Tiempo total de ejecución: %f segundos\n", cpu_time_used);
    return 0;
}
