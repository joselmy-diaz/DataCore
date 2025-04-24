#include "DataCore.h"
#include "stuctura.h"
#include <time.h>
#include <stdio.h>
#include <stdalign.h>
#include <stdlib.h>

// Suposición de que existen las siguientes estructuras y funciones.
// Este código asume que estas estructuras y funciones ya están definidas:
// Data, Obj, Entry, ObjString, initTH(), initTR(), insertD(), searchD(), printData(),
// freeD(), initList(), searchDIndex(), insertL().

int main(int argc, char const *argv[]) {
    clock_t startA, start, end;
    double cpu_time_used;
    printf("tamano de Data es: %zu\n", sizeof(ObjString));
    printf("Alignment: %zu bytes\n", alignof(Obj));

    // Punto de control: Inicio de inicialización
    printf("\nInicializando objetos...\n");

    // Inicialización de objetos
    As asValue1 = { .Num = 31 };
    Entry e1 = newEntry("data1", newObj(TYPE_NUM, &asValue1));

    As asValue2 = { .NumF = 12.2 };
    Entry e2 = newEntry("data2", newObj(TYPE_NUMFL, &asValue2));

    As asValue3 = { .NumF = 13.3 };
    Entry e3 = newEntry("data3", newObj(TYPE_NUMFL, &asValue3));

    As asValue4 = { .NumF = 14.4 };
    Entry e4 = newEntry("data4", newObj(TYPE_NUMFL, &asValue4));

    Entry e5 = newEntry("data5", newObj(TYPE_BOOL_F, NULL));

    Entry e6 = newEntry("data6", newObjString("Hola mundo1"));

    As asValue7 = { .Num = 45 };
    Entry e7 = newEntry("data7", newObj(TYPE_NUM, &asValue7));

    As asValue8 = { .NumF = 22.5 };
    Entry e8 = newEntry("data8", newObj(TYPE_NUMFL, &asValue8));


    Entry e9 = newEntry("data9", newObj(TYPE_BOOL_T, NULL));

    As asValue10 = { .Num = 78 };
    Entry e10 = newEntry("data10", newObj(TYPE_NUM, &asValue10));

    Entry e11 = newEntry("data11", newObjString("Hola"));

    // // Punto de control: Inicialización de tablas
    printf("\nInicializando tabla hash y árbol...\n");
    Obj *table = initTH(100);
    Obj *tree = initTR();

    Entry entries[] = {e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11};
    startA = clock();


    // Inserciones en la tabla hash
    printf("\nInsertando en la tabla hash...\n");
    start = clock();
    for (int i = 0; i < sizeof(entries) / sizeof(entries[0]); i++) {
        insertD(table, entries[i]);
    }
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Tiempo de inserción en la tabla hash: %f segundos\n", cpu_time_used);

    // Inserciones en el árbol AVL
    printf("\nInsertando en el árbol AVL...\n");
    start = clock();
    for (int i = 0; i < sizeof(entries) / sizeof(entries[0]); i++) {
        insertD(tree, entries[i]);
    }
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Tiempo de inserción en el árbol AVL: %f segundos\n", cpu_time_used);


    // // Puntos de control: Lista
    // printf("\nInicializando lista...\n");
    Obj *lis = initList(14, true);

    // Inserciones en la lista
    printf("\nInsertando en la lista...\n");
    start = clock();
    for (int i = 0; i < sizeof(entries) / sizeof(entries[0]); i++) {
        insertD(lis, entries[i]);
    }
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Tiempo de inserción en la lista: %f segundos\n", cpu_time_used);

    // Imprimir valores en la tabla hash
    printf("\nValores en la tabla:\n");
    for (int i = 0; i < sizeof(entries) / sizeof(entries[0]); i++) {
        printf("Valor de '%s': ", entries[i].key);
        printData(searchD(table, entries[i].key));
    }

    // Imprimir valores en el árbol AVL
    printf("\nValores en el árbol AVL:\n");
    for (int i = 0; i < sizeof(entries) / sizeof(entries[0]); i++) {
        printf("Valor de '%s': ", entries[i].key);
        printData(searchD(tree, entries[i].key));
    }

    // Imprimir valores en la lista
    printf("\nValores en la lista:\n");
    for (int i = 0; i < sizeof(entries) / sizeof(entries[0]); i++) {
        printf("Valor de '%s': ", entries[i].key);
        Obj* dataO = searchD(lis, entries[i].key);
        printData(dataO);
    }
    

    // Liberar memoria
    freeObjs(table);
    freeObjs(tree);
    freeObjs(lis);
    // Punto de control: Medición final
    printf("\nMedición del tiempo total...\n");
    end = clock();
    cpu_time_used = ((double)(end - startA)) / CLOCKS_PER_SEC;
    printf("Tiempo total de ejecución: %f segundos\n", cpu_time_used);
    return 0;
}
