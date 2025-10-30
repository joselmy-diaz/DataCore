#!/bin/bash

set -e

# Nombre base de la librería y header
LIB_STATIC="libmilib.a"
LIB_DYNAMIC="libmilib.so"
HEADER="milib.h"

# Directorios de instalación (pueden ser sobreescritos por argumentos)
LIB_DIR="/usr/local/lib"
INCLUDE_DIR="/usr/local/include"

# Función de ayuda
show_help() {
    echo "Uso: $0 [OPCIONES]"
    echo ""
    echo "Script de instalación/desinstalación de librerías DataCore"
    echo ""
    echo "Opciones:"
    echo "  --lib-dir DIR        Directorio de instalación de librerías (default: /usr/local/lib)"
    echo "  --include-dir DIR    Directorio de instalación de headers (default: /usr/local/include)"
    echo "  -h, --help           Mostrar esta ayuda"
    echo ""
}

# Procesar argumentos
while [[ $# -gt 0 ]]; do
    case $1 in
        --lib-dir)
            LIB_DIR="$2"
            shift 2
            ;;
        --include-dir)
            INCLUDE_DIR="$2"
            shift 2
            ;;
        -h|--help)
            show_help
            exit 0
            ;;
        *)
            echo "Error: Opción desconocida: $1"
            show_help
            exit 1
            ;;
    esac
done

# Función para instalar
install_lib() {
    local libname="$1"
    echo "Instalando $libname..."
    
    # Verificar que el archivo de librería existe
    if [ ! -f "$libname" ]; then
        echo "Error: El archivo de librería '$libname' no existe."
        echo "Por favor, compila la librería primero."
        exit 1
    fi
    
    # Verificar que el archivo de header existe
    if [ ! -f "$HEADER" ]; then
        echo "Error: El archivo de header '$HEADER' no existe."
        exit 1
    fi
    
    # Crear directorios si no existen
    if [ ! -d "$LIB_DIR" ]; then
        echo "Creando directorio de librerías: $LIB_DIR"
        sudo mkdir -p "$LIB_DIR"
    fi
    
    if [ ! -d "$INCLUDE_DIR" ]; then
        echo "Creando directorio de headers: $INCLUDE_DIR"
        sudo mkdir -p "$INCLUDE_DIR"
    fi

    # Copiar archivos
    echo "Copiando $libname a $LIB_DIR/"
    sudo cp "$libname" "$LIB_DIR/"
    
    echo "Copiando $HEADER a $INCLUDE_DIR/"
    sudo cp "$HEADER" "$INCLUDE_DIR/"

    # Actualizar cache de librerías
    echo "Actualizando cache de librerías..."
    sudo ldconfig

    echo "✓ Instalación completada exitosamente."
}

# Función para desinstalar
uninstall_lib() {
    local libname="$1"
    echo "Desinstalando $libname..."
    
    local lib_path="$LIB_DIR/$libname"
    local header_path="$INCLUDE_DIR/$HEADER"
    
    # Verificar y eliminar librería
    if [ -f "$lib_path" ]; then
        echo "Eliminando $lib_path"
        sudo rm -f "$lib_path"
    else
        echo "Aviso: El archivo '$lib_path' no existe (ya fue eliminado o no instalado)."
    fi
    
    # Verificar y eliminar header
    if [ -f "$header_path" ]; then
        echo "Eliminando $header_path"
        sudo rm -f "$header_path"
    else
        echo "Aviso: El archivo '$header_path' no existe (ya fue eliminado o no instalado)."
    fi

    # Actualizar cache de librerías
    echo "Actualizando cache de librerías..."
    sudo ldconfig

    echo "✓ Desinstalación completada."
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
    echo "Error: Opción de librería inválida."
    exit 1
fi

# Ejecutar según acción
if [ "$action" == "1" ]; then
    install_lib "$LIB_SELECTED"
elif [ "$action" == "2" ]; then
    uninstall_lib "$LIB_SELECTED"
else
    echo "Error: Opción de acción inválida."
    exit 1
fi
