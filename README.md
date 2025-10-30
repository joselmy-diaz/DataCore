# DataCore

## Descripción

**DataCore** es un módulo diseñado para gestionar estructuras de datos dinámicas en el proyecto Kintsugi. Proporciona una interfaz para trabajar con diferentes tipos de estructuras como tablas hash, árboles AVL, arreglos y cadenas de texto. Este módulo está optimizado para manejar datos de manera eficiente y flexible, permitiendo su inserción, búsqueda, y liberación de memoria.

## Utilidad

El propósito principal de DataCore es facilitar la manipulación de datos en estructuras complejas, proporcionando funciones reutilizables y bien definidas. Esto incluye:

- **Inserción de datos**: Permite agregar elementos a estructuras como tablas hash, árboles AVL y arreglos.
- **Búsqueda de datos**: Soporta búsquedas por clave o índice en las estructuras.
- **Gestión de memoria**: Incluye funciones para liberar memoria de manera segura y eficiente.
- **Creación de objetos**: Proporciona métodos para inicializar objetos y entradas clave-valor.

## Dependencias

**DataCore** requiere las siguientes dependencias para compilar:

- **CMake** (versión 3.10 o superior)
- **Compilador C** compatible (GCC, Clang, etc.)
- **Bibliotecas estándar de C** (stdio, stdlib, string, pthread para corutinas)

## Instalación

```bash
# Clona este repositorio
git clone https://github.com/joselmy-diaz/DataCore.git
cd DataCore

# Compila el proyecto con CMake
mkdir build
cd build
cmake ..
make

# (Opcional) Instala la librería en el sistema
cd ..
./install.sh

# También puedes especificar directorios de instalación personalizados:
./install.sh --lib-dir /ruta/personalizada/lib --include-dir /ruta/personalizada/include
```

## Compilación y Enlace

### Compilación estática

Para enlazar tu aplicación con la librería estática de DataCore:

```bash
# Compilar tu aplicación con DataCore
gcc -o mi_programa mi_programa.c -I/ruta/a/DataCore/include -L/ruta/a/DataCore/build -lDataCore

# O si instalaste la librería en el sistema:
gcc -o mi_programa mi_programa.c -lDataCore
```

### Compilación dinámica

Si generas una librería dinámica (.so), puedes enlazarla así:

```bash
# Compilar con librería dinámica
gcc -o mi_programa mi_programa.c -I/ruta/a/DataCore/include -L/ruta/a/DataCore/build -lDataCore -Wl,-rpath,/ruta/a/DataCore/build
```

### Uso con CMake

Puedes integrar DataCore en tu proyecto CMake:

```cmake
# En tu CMakeLists.txt
include_directories(/ruta/a/DataCore/include)
link_directories(/ruta/a/DataCore/build)

add_executable(mi_programa mi_programa.c)
target_link_libraries(mi_programa DataCore)
```

## Cómo utilizarlo

A continuación, se describe cómo utilizar las principales funcionalidades de **DataCore** con ejemplos prácticos basados en el archivo `DataCoreT.c`.

### Archivos de Cabecera Principales

Para usar DataCore en tu proyecto, incluye los siguientes headers:

```c
#include "DataCore.h"      // API principal y funciones de manejo de objetos
#include "structure.h"      // Estructuras de datos (tablas hash, árboles, arrays)
#include "obj.h"            // Definiciones de tipos de objetos
```

### 1. Inicialización de Objetos

Para inicializar un objeto, utiliza la función `newData` o `newObj` dependiendo del tipo de dato que quieras manejar. Por ejemplo:

```c
// Crear un objeto de tipo número entero
As asValue1 = { .Num = 31 };
Obj* data1 = newObj(TYPE_NUM, &asValue1);
// Debes colocarlo en un Entry para estructura que requieren clave valor
// como: Arbol AVL, Tabla hash
Entry e1 = newEntry("data1", data1);

// Crear un objeto de tipo número flotante
As asValue2 = { .NumF = 12.2 };
Obj* data2 = newObj(TYPE_NUMFL, &asValue2);
Entry e2 = newEntry("data2", data2);

// Crear un objeto de tipo cadena de texto
Obj* data3 = newObjString("Hola mundo");
Entry e6 = newEntry("data3", data3);
```

### 2. Inicialización de Estructuras de Datos

**DataCore** permite trabajar con diferentes estructuras de datos como tablas hash, árboles AVL y listas. Para inicializarlas, utiliza las siguientes funciones:

```c
// Inicializar una tabla hash con capacidad para 100 elementos
Obj *table = initTH(100);

// Inicializar un árbol AVL
Obj *tree = initTR();

// Inicializar una lista con capacidad inicial de 14 elementos
Obj *lis = initArray(14, false);
```

### 3. Inserción de Datos

Puedes insertar datos en las estructuras utilizando la función `insertD`. Por ejemplo:

```c
    Obj* data1 = newObjString("Hola mundo");
// Insertar entradas en una tabla hash
    insertD(table, data1);

// Insertar entradas en un árbol AVL
    insertD(tree, data1);

// Insertar entradas en una lista
    insertArray(lis, data1);
```

### 4. Búsqueda de Datos

Para buscar datos en las estructuras, utiliza la función `searchD`. Por ejemplo:

```c
// Buscar un dato en la tabla hash
Obj* data = searchD(table, "data1");

// Buscar un dato en el árbol AVL
Obj* dataTree = searchD(tree, "data1");

// Buscar un dato en la lista
Obj* dataList = searchArray(lis, "data1");
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
