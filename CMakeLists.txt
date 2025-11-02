# Versión mínima de CMake
cmake_minimum_required(VERSION 3.10)
# Nombre del proyecto
project(DataCore)

# Establecer Clang como el compilador
set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")

# Archivos fuente para el cliente y el servidor
set(SERVER_SOURCE 
    ./src/printD.c
    ./src/hast.c 
    ./src/DataCore.c
    ./src/structures/main.c 
    ./src/structures/tablaH.c 
    ./src/structures/treeH.c
    ./src/structures/array.c
    ./src/structures/buffer.c
    ./src/structures/bufferC.c
)

# Crear la biblioteca
add_library(${PROJECT_NAME} STATIC ${SERVER_SOURCE})

# Habilitar Position Independent Code para compatibilidad con bibliotecas compartidas
set_target_properties(${PROJECT_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_include_directories(${PROJECT_NAME} PUBLIC ./include)

# Configuración de pruebas
add_subdirectory(test)
