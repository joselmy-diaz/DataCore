# DataCore

**DataCore** es un módulo diseñado para gestionar estructuras de datos dinámicas en el proyecto Kintsugi. Proporciona una interfaz para trabajar con diferentes tipos de estructuras como tablas hash, árboles AVL, arreglos y cadenas de texto. Este módulo está optimizado para manejar datos de manera eficiente y flexible, permitiendo su inserción, búsqueda, y liberación de memoria.

## Utilidad

El propósito principal de DataCore es facilitar la manipulación de datos en estructuras complejas, proporcionando funciones reutilizables y bien definidas. Esto incluye:

- **Inserción de datos**: Permite agregar elementos a estructuras como tablas hash, árboles AVL y arreglos.
- **Búsqueda de datos**: Soporta búsquedas por clave o índice en las estructuras.
- **Gestión de memoria**: Incluye funciones para liberar memoria de manera segura y eficiente.
- **Creación de objetos**: Proporciona métodos para inicializar objetos y entradas clave-valor.

## Cómo utilizarlo

1. **Inicialización de un objeto**:
   Utiliza la función `newData` para crear un nuevo objeto con un tipo y valor específico:
   ```c
   Obj* obj = newData(OBJ_HASH_TABLE, as);
   ```
