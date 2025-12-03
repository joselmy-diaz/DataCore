#include "logging.h"
#include <stdlib.h>
#include <string.h>

// Configuración global
static LogConfig g_log_config = {
    .min_level = LOG_LEVEL_INFO,
    .show_timestamp = false,
    .show_colors = true,
    .output = NULL  // Se inicializa a stdout en log_init()
};

// Códigos de color ANSI
static const char* ANSI_RESET   = "\033[0m";
static const char* ANSI_RED     = "\033[31m";
static const char* ANSI_YELLOW  = "\033[33m";
static const char* ANSI_BLUE    = "\033[34m";
static const char* ANSI_GRAY    = "\033[90m";

// Prefijos de nivel
static const char* level_prefix[] = {
    "DEBUG",
    "INFO",
    "WARN",
    "ERROR"
};

// Emojis por nivel
static const char* level_emoji[] = {
    "🔍",  // DEBUG
    "ℹ️ ",  // INFO
    "⚠️ ",  // WARN
    "❌"   // ERROR
};

void log_init(void) {
    if (g_log_config.output == NULL) {
        g_log_config.output = stdout;
    }
    
    // Leer variable de entorno VM_LOG_LEVEL
    const char* env_level = getenv("VM_LOG_LEVEL");
    if (env_level) {
        if (strcmp(env_level, "DEBUG") == 0) {
            g_log_config.min_level = LOG_LEVEL_DEBUG;
        } else if (strcmp(env_level, "INFO") == 0) {
            g_log_config.min_level = LOG_LEVEL_INFO;
        } else if (strcmp(env_level, "WARN") == 0) {
            g_log_config.min_level = LOG_LEVEL_WARN;
        } else if (strcmp(env_level, "ERROR") == 0) {
            g_log_config.min_level = LOG_LEVEL_ERROR;
        } else if (strcmp(env_level, "NONE") == 0) {
            g_log_config.min_level = LOG_LEVEL_NONE;
        }
    }
    
    // Detectar si estamos en terminal para colores
    const char* no_color = getenv("NO_COLOR");
    if (no_color != NULL) {
        g_log_config.show_colors = false;
    }
}

void log_configure(const LogConfig* config) {
    if (config) {
        g_log_config = *config;
        if (g_log_config.output == NULL) {
            g_log_config.output = stdout;
        }
    }
}

LogLevel log_get_level(void) {
    return g_log_config.min_level;
}

void log_set_level(LogLevel level) {
    g_log_config.min_level = level;
}

void vm_log_internal(LogLevel level, const char* file, int line, 
                     const char* format, ...) {
    // Verificar si debemos mostrar este nivel
    if (level < g_log_config.min_level) {
        return;
    }
    
    FILE* out = g_log_config.output;
    
    // Timestamp opcional
    if (g_log_config.show_timestamp) {
        time_t now = time(NULL);
        struct tm* t = localtime(&now);
        fprintf(out, "[%02d:%02d:%02d] ", t->tm_hour, t->tm_min, t->tm_sec);
    }
    
    // Color según nivel
    const char* color = ANSI_RESET;
    if (g_log_config.show_colors) {
        switch (level) {
            case LOG_LEVEL_DEBUG: color = ANSI_GRAY; break;
            case LOG_LEVEL_INFO:  color = ANSI_BLUE; break;
            case LOG_LEVEL_WARN:  color = ANSI_YELLOW; break;
            case LOG_LEVEL_ERROR: color = ANSI_RED; break;
            default: color = ANSI_RESET;
        }
    }
    
    // Prefijo con emoji y nivel
    fprintf(out, "%s%s [%s]%s ", 
            color, 
            level_emoji[level], 
            level_prefix[level],
            g_log_config.show_colors ? ANSI_RESET : "");
    
    // Mensaje del usuario
    va_list args;
    va_start(args, format);
    vfprintf(out, format, args);
    va_end(args);
    
    // Nueva línea y debug info
    if (g_log_config.min_level == LOG_LEVEL_DEBUG) {
        fprintf(out, " (%s:%d)", file, line);
    }
    fprintf(out, "\n");
    
    // Flush para errores
    if (level >= LOG_LEVEL_ERROR) {
        fflush(out);
    }
}
