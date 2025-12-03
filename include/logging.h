/**
 * @file logging.h
 * @brief Sistema de logging centralizado para Kintsugi VM
 * 
 * Proporciona macros y funciones para logging consistente con niveles
 * configurables y formato estandarizado.
 * 
 * @author Kintsugi VM Team
 * @version 1.0
 * @date 2025-12-02
 */

#ifndef KINTSUGI_LOGGING_H
#define KINTSUGI_LOGGING_H

#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Niveles de logging disponibles
 */
typedef enum {
    LOG_LEVEL_DEBUG = 0,    /**< Información detallada de debug */
    LOG_LEVEL_INFO  = 1,    /**< Información general */
    LOG_LEVEL_WARN  = 2,    /**< Advertencias */
    LOG_LEVEL_ERROR = 3,    /**< Errores */
    LOG_LEVEL_NONE  = 4     /**< Sin logging */
} LogLevel;

/**
 * @brief Configuración global del sistema de logging
 */
typedef struct {
    LogLevel min_level;      /**< Nivel mínimo a mostrar */
    bool show_timestamp;     /**< Mostrar timestamp en mensajes */
    bool show_colors;        /**< Usar colores ANSI en terminal */
    FILE* output;            /**< Stream de salida (stdout por defecto) */
} LogConfig;

/**
 * @brief Inicializa el sistema de logging con configuración por defecto
 * 
 * Lee la variable de entorno VM_LOG_LEVEL para configurar el nivel:
 * - DEBUG, INFO, WARN, ERROR, NONE
 * 
 * @example
 * @code
 * VM_LOG_LEVEL=DEBUG ./kintsugiVm program.kbc
 * @endcode
 */
void log_init(void);

/**
 * @brief Configura el sistema de logging
 * 
 * @param config Configuración a aplicar
 */
void log_configure(const LogConfig* config);

/**
 * @brief Función interna de logging (no usar directamente)
 * 
 * @param level Nivel del mensaje
 * @param file Archivo fuente (__FILE__)
 * @param line Línea fuente (__LINE__)
 * @param format Formato printf
 * @param ... Argumentos variables
 */
void vm_log_internal(LogLevel level, const char* file, int line, 
                     const char* format, ...);

/**
 * @brief Obtiene el nivel de logging actual
 */
LogLevel log_get_level(void);

/**
 * @brief Establece el nivel de logging
 */
void log_set_level(LogLevel level);

// ============================================================================
// MACROS DE LOGGING (Recomendadas para uso)
// ============================================================================

/**
 * @brief Log de debug (nivel más bajo)
 * 
 * Solo se muestra si VM_LOG_LEVEL=DEBUG
 */
#define LOG_DEBUG(...) \
    vm_log_internal(LOG_LEVEL_DEBUG, __FILE__, __LINE__, __VA_ARGS__)

/**
 * @brief Log informativo
 * 
 * Información general del flujo del programa
 */
#define LOG_INFO(...) \
    vm_log_internal(LOG_LEVEL_INFO, __FILE__, __LINE__, __VA_ARGS__)

/**
 * @brief Log de advertencia
 * 
 * Situaciones anómalas que no impiden continuar
 */
#define LOG_WARN(...) \
    vm_log_internal(LOG_LEVEL_WARN, __FILE__, __LINE__, __VA_ARGS__)

/**
 * @brief Log de error
 * 
 * Errores que impiden operación normal
 */
#define LOG_ERROR(...) \
    vm_log_internal(LOG_LEVEL_ERROR, __FILE__, __LINE__, __VA_ARGS__)

// ============================================================================
// MACROS DE COMPATIBILIDAD (Para migración gradual)
// ============================================================================

/**
 * @brief Printf condicional según nivel de logging
 * 
 * Reemplaza printf() directo permitiendo control de verbosidad
 */
#define VM_PRINTF(level, ...) \
    do { \
        if (log_get_level() <= (level)) { \
            printf(__VA_ARGS__); \
        } \
    } while(0)

/**
 * @brief Fprintf condicional a stderr
 */
#define VM_FPRINTF_ERROR(...) \
    vm_log_internal(LOG_LEVEL_ERROR, __FILE__, __LINE__, __VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif // KINTSUGI_LOGGING_H
