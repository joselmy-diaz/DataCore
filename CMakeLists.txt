# Versión mínima de CMake
cmake_minimum_required(VERSION 3.10)
# Nombre del proyecto
project(DataCore)

# Establecer Clang como el compilador
set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")


# # Añadir opciones de compilación y enlace para -pg
# add_compile_options(-pg)
# add_link_options(-pg)


# Archivos fuente para el cliente y el servidor
set(SERVER_SOURCE 
    ./src/printD.c
    ./src/hast.c 
    ./src/DataCore.c
    ./src/structures/main.c 
    ./src/structures/tablaH.c 
    ./src/structures/treeH.c
    ./src/structures/array.c
)

# Crear la biblioteca
add_library(${PROJECT_NAME} STATIC ${SERVER_SOURCE})

target_include_directories(${PROJECT_NAME} PUBLIC ./include)

# Configuración de pruebas
enable_testing()
add_subdirectory(test)
