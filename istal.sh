#!/bin/bash

set -e

# Nombre base de la librería y header
LIB_STATIC="libmilib.a"
LIB_DYNAMIC="libmilib.so"
HEADER="milib.h"

# Directorios de instalación
LIB_DIR="/usr/local/lib"
INCLUDE_DIR="/usr/local/include"

# Función para instalar
install_lib() {
    local libname="$1"
    echo "Instalando $libname..."

    sudo cp "$libname" "$LIB_DIR/"
    sudo cp "$HEADER" "$INCLUDE_DIR/"

    sudo ldconfig

    echo "Instalación completada."
}

# Función para desinstalar
uninstall_lib() {
    local libname="$1"
    echo "Desinstalando $libname..."

    sudo rm -f "$LIB_DIR/$libname"
    sudo rm -f "$INCLUDE_DIR/$HEADER"

    sudo ldconfig

    echo "Desinstalación completada."
}

# Preguntar acción
echo "¿Qué quieres hacer?"
echo "1) Instalar"
echo "2) Desinstalar"
read -p "Opción [1-2]: " action

# Preguntar tipo de librería
echo "¿Qué tipo de librería quieres manejar?"
echo "1) Estática (.a)"
echo "2) Dinámica (.so)"
read -p "Opción [1-2]: " libtype

# Determinar nombre de librería
if [ "$libtype" == "1" ]; then
    LIB_SELECTED="$LIB_STATIC"
elif [ "$libtype" == "2" ]; then
    LIB_SELECTED="$LIB_DYNAMIC"
else
    echo "Opción de librería inválida."
    exit 1
fi

# Ejecutar según acción
if [ "$action" == "1" ]; then
    install_lib "$LIB_SELECTED"
elif [ "$action" == "2" ]; then
    uninstall_lib "$LIB_SELECTED"
else
    echo "Opción de acción inválida."
    exit 1
fi
