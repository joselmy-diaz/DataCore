# DataCore

## Descripción

**DataCore** es un módulo diseñado para gestionar estructuras de datos dinámicas en el proyecto Kintsugi. Proporciona una interfaz para trabajar con diferentes tipos de estructuras como tablas hash, árboles AVL, arreglos y cadenas de texto. Este módulo está optimizado para manejar datos de manera eficiente y flexible, permitiendo su inserción, búsqueda, y liberación de memoria.

## Utilidad

El propósito principal de DataCore es facilitar la manipulación de datos en estructuras complejas, proporcionando funciones reutilizables y bien definidas. Esto incluye:

- **Inserción de datos**: Permite agregar elementos a estructuras como tablas hash, árboles AVL y arreglos.
- **Búsqueda de datos**: Soporta búsquedas por clave o índice en las estructuras.
- **Gestión de memoria**: Incluye funciones para liberar memoria de manera segura y eficiente.
- **Creación de objetos**: Proporciona métodos para inicializar objetos y entradas clave-valor.

## Cómo utilizarlo

A continuación, se describe cómo utilizar las principales funcionalidades de **DataCore** con ejemplos prácticos basados en el archivo `DataCoreT.c`.

### 1. Inicialización de Objetos

Para inicializar un objeto, utiliza la función `newData` o `newObj` dependiendo del tipo de dato que quieras manejar. Por ejemplo:

```c
// Crear un objeto de tipo número entero
As asValue1 = { .Num = 31 };
Entry e1 = newEntry("data1", newObj(TYPE_NUM, &asValue1));

// Crear un objeto de tipo número flotante
As asValue2 = { .NumF = 12.2 };
Entry e2 = newEntry("data2", newObj(TYPE_NUMFL, &asValue2));

// Crear un objeto de tipo cadena de texto
Entry e6 = newEntry("data6", newObjString("Hola mundo1"));
```

### 2. Inicialización de Estructuras de Datos

**DataCore** permite trabajar con diferentes estructuras de datos como tablas hash, árboles AVL y listas. Para inicializarlas, utiliza las siguientes funciones:

```c
// Inicializar una tabla hash con capacidad para 100 elementos
Obj *table = initTH(100);

// Inicializar un árbol AVL
Obj *tree = initTR();

// Inicializar una lista con capacidad inicial de 14 elementos
Obj *lis = initList(14, true);
```

### 3. Inserción de Datos

Puedes insertar datos en las estructuras utilizando la función `insertD`. Por ejemplo:

```c
// Insertar entradas en una tabla hash
for (int i = 0; i < sizeof(entries) / sizeof(entries[0]); i++) {
    insertD(table, entries[i]);
}

// Insertar entradas en un árbol AVL
for (int i = 0; i < sizeof(entries) / sizeof(entries[0]); i++) {
    insertD(tree, entries[i]);
}

// Insertar entradas en una lista
for (int i = 0; i < sizeof(entries) / sizeof(entries[0]); i++) {
    insertD(lis, entries[i]);
}
```

### 4. Búsqueda de Datos

Para buscar datos en las estructuras, utiliza la función `searchD`. Por ejemplo:

```c
// Buscar un dato en la tabla hash
Obj* data = searchD(table, "data1");

// Buscar un dato en el árbol AVL
Obj* dataTree = searchD(tree, "data2");

// Buscar un dato en la lista
Obj* dataList = searchD(lis, "data3");
```

### 5. Impresión de Datos

Para imprimir los datos almacenados en las estructuras, utiliza la función `printData`. Por ejemplo:

```c
// Imprimir valores en la tabla hash
for (int i = 0; i < sizeof(entries) / sizeof(entries[0]); i++) {
    printf("Valor de '%s': ", entries[i].key);
    printData(searchD(table, entries[i].key));
}

// Imprimir valores en el árbol AVL
printObjf(dataTree);

// Imprimir valores en la lista
printObjf(lis);
```

### 6. Liberación de Memoria

Es importante liberar la memoria utilizada por las estructuras para evitar fugas de memoria. Utiliza la función `freeObjs` para liberar los recursos:

```c
freeObjs(table);
freeObjs(tree);
freeObjs(lis);
```

Con estas instrucciones, puedes aprovechar al máximo las funcionalidades de **DataCore** para gestionar estructuras de datos dinámicas de manera eficiente.
