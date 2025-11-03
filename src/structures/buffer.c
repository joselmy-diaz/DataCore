#include "./buffer.h"

/**
 * Crea un nuevo buffer
 * @param size Tamaño del buffer
 * @return Puntero al buffer creado o NULL si falla
 */
Obj* bufferCreate(size_t size) {
    if (size == 0) {
        fprintf(stderr, "Invalid buffer size\n");
        return NULL;
    }

    Buffer* cb = (Buffer*)malloc(sizeof(Buffer));
    if (!cb) {
        fprintf(stderr, "Memory allocation for Buffer failed\n");
        return NULL;
    }

    cb->obj.type = TYPE_BUFFER;
    cb->obj.reference = 0;
    cb->buffer = (int8_t*)malloc(size);
    if (!cb->buffer) {
        free(cb);
        fprintf(stderr, "Memory allocation for buffer data failed\n");
        return NULL;
    }

    cb->size = size;
    cb->head = 0;
    cb->tail = 0;
    bufferClear((Obj*)cb);
    return (Obj*)cb;
}

/**
 * Limpia el contenido del buffer
 * @param buffer Buffer a limpiar
 * @return true si la operación fue exitosa
 */
bool bufferClear(Obj* buffer) {
    if (!buffer || buffer->type != TYPE_BUFFER) return false;
    Buffer* cb = (Buffer*)buffer;
    
    cb->head = 0;
    cb->tail = 0;
    memset(cb->buffer, 0, cb->size);
    return true;
}

/**
 * Redimensiona un buffer
 * @param buf Buffer a redimensionar
 * @param newSize Nuevo tamaño
 * @return true si la operación fue exitosa
 */
bool BufferResize(Buffer* buf, size_t newSize) {
    if (!buf || newSize == 0) return false;
    
    int8_t* newBuf = (int8_t*)realloc(buf->buffer, newSize);
    if (!newBuf) return false;
    
    buf->buffer = newBuf;
    buf->size = newSize;
    
    // Ajustar head y tail si están fuera del nuevo tamaño
    if (buf->head > newSize) buf->head = newSize;
    if (buf->tail > newSize) buf->tail = newSize;
    
    return true;
}

/**
 * Lee un archivo y lo almacena en el buffer
 * @param buf Buffer destino
 * @param path Ruta del archivo
 * @return true si la operación fue exitosa
 */
bool readFileBuffer(Buffer* buf, const char* path) {
    if (!buf || !path) return false;

    FILE *f = fopen(path, "rb");
    if (!f) {
        perror("fopen");
        return false;
    }

    if (fseek(f, 0, SEEK_END) != 0) {
        fclose(f);
        return false;
    }

    long lsize = ftell(f);
    if (lsize < 0) { 
        fclose(f); 
        return false; 
    }

    size_t size = (size_t)lsize;
    rewind(f);

    // Asegurar que el buffer tenga suficiente espacio
    if (buf->size < size) {
        if (!BufferResize(buf, size)) {
            fclose(f);
            return false;
        }
    }

    // Leer el archivo en bloques
    size_t bytesRead = fread(buf->buffer, 1, size, f);
    fclose(f);

    if (bytesRead != size) {
        return false;
    }

    buf->head = bytesRead;
    buf->tail = 0;
    return true;
}

/**
 * Escribe el contenido del buffer en un archivo
 * @param buf Buffer origen
 * @param path Ruta del archivo
 * @return true si la operación fue exitosa
 */
bool writeFileBuffer(const Buffer* buf, const char* path) {
    if (!buf || !path || !buf->buffer) return false;

    FILE *f = fopen(path, "wb");
    if (!f) {
        perror("fopen");
        return false;
    }

    size_t written = fwrite(buf->buffer, 1, buf->head, f);
    fclose(f);

    return (written == buf->head);
}

/**
 * Escribe datos en el buffer
 * @param buf Buffer destino
 * @param data Datos a escribir
 * @param dataSize Tamaño de los datos
 * @return true si la operación fue exitosa
 */
bool bufferWrite(Buffer* buf, const void* data, size_t dataSize) {
    if (!buf || !data || dataSize == 0) return false;
    if (buf->head + dataSize > buf->size) return false;

    memcpy(buf->buffer + buf->head, data, dataSize);
    buf->head += dataSize;
    return true;
}

/**
 * Lee datos del buffer
 * @param buf Buffer origen
 * @param data Buffer destino para los datos leídos
 * @param dataSize Cantidad de datos a leer
 * @return true si la operación fue exitosa
 */
bool bufferRead(Buffer* buf, void* data, size_t dataSize) {
    if (!buf || !data || dataSize == 0) return false;
    if (buf->tail + dataSize > buf->head) return false;

    memcpy(data, buf->buffer + buf->tail, dataSize);
    buf->tail += dataSize;
    return true;
}

/**
 * Obtiene el tamaño del buffer
 * @param buf Buffer a consultar
 * @return Tamaño del buffer o 0 si es inválido
 */
size_t bufferGetSize(Buffer* buf) {
    return buf ? buf->size : 0;
}

/**
 * Libera la memoria ocupada por el buffer
 * @param buffer Buffer a liberar
 * @return true si la operación fue exitosa
 */
bool bufferFree(Obj* buffer) {
    if (!buffer || buffer->type != TYPE_BUFFER) return false;
    
    Buffer* cb = (Buffer*)buffer;
    if (cb->buffer) {
        free(cb->buffer);
        cb->buffer = NULL;
    }
    free(cb);
    return true;
}
