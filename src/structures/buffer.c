#include "./buffer.h"

bool bufferClear(Obj* buffer) {
    if (!buffer) return false;
    
    if (buffer->type == TYPE_BUFFER_CIRCULAR) {
        BufferCircular* cb = (BufferCircular*)buffer;
        cb->head = 0;
        cb->tail = 0;
        for (size_t i = 0; i < cb->size; i++) {
            cb->buffer[i] = 0;
        }
    } else if (buffer->type == TYPE_BUFFER) {
        Buffer* bd = (Buffer*)buffer;
        for (size_t i = 0; i < bd->size; i++) {
            bd->buffer[i] = 0;
        }
    } else {
        return false;
    }
    return true;
}

// Inicializa el buffer
Obj* bufferCreate(bool circular, int size) {
    if (circular) {
        BufferCircular* cb = (BufferCircular*)malloc(sizeof(BufferCircular));
        if (!cb) {
            fprintf(stderr, "Memory allocation for BufferCircular failed\n");
            return NULL;
        }
        cb->obj.type = TYPE_BUFFER_CIRCULAR;
        cb->buffer = (int8_t*)malloc(sizeof(int8_t) * size);
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
    } else {
        Buffer* cb = (Buffer*)malloc(sizeof(Buffer));
        if (!cb) {
            fprintf(stderr, "Memory allocation for Buffer failed\n");
            return NULL;
        }
        cb->obj.type = TYPE_BUFFER;
        cb->buffer = (int8_t*)malloc(sizeof(int8_t) * size);
        if (!cb->buffer) {
            free(cb);
            fprintf(stderr, "Memory allocation for buffer data failed\n");
            return NULL;
        }
        cb->size = size;
        bufferClear((Obj*)cb);
        return (Obj*)cb;
    }
}

// Helper: read an entire file into a Buffer (responsibility: file -> buffer)
bool BufferResize(Buffer* buf, size_t newSize) {
    if (!buf || newSize == 0) return false;
    int8_t* newBuf = (int8_t*)realloc(buf->buffer, newSize);
    if (!newBuf) return false;
    buf->buffer = newBuf;
    buf->size = (int)newSize;
    return true;
}

bool readFileBuffer (Buffer* buf, const char* path) {
    if (!path || !buf) return false;

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
    if (lsize < 0) { fclose(f); return false; }
    size_t size = (size_t)lsize;
    rewind(f);

    if ((size_t)buf->size < size) {
        if (!BufferResize(buf, size)) {
            fclose(f);
            return false;
        }
    }

    size_t remaining = size;
    size_t offset = 0;
    while (remaining > 0) {
        size_t chunk = fread(buf->buffer + offset, 1, remaining, f);
        if (chunk == 0) {
            if (ferror(f)) {
                fclose(f);
                return false;
            }
            break;
        }
        offset += chunk;
        remaining -= chunk;
    }

    fclose(f);
    return (offset == size);
}

// Write buffer contents to file
bool writeFileBuffer(const Buffer* buf, const char* path) {
    if (!buf || !path || !buf->buffer) return false;

    FILE *f = fopen(path, "wb");
    if (!f) {
        perror("fopen");
        return false;
    }

    size_t written = fwrite(buf->buffer, 1, buf->size, f);
    fclose(f);
    
    return (written == buf->size);
}

// Basic generic functions for buffer read/write operations

// Write data to buffer at specified position
bool bufferWrite(Buffer* buf, const void* data, size_t dataSize, size_t position) {
    if (!buf || !data || dataSize == 0) return false;
    if (position + dataSize > buf->size) return false;
    
    memcpy(buf->buffer + position, data, dataSize);
    return true;
}

// Read data from buffer at specified position
bool bufferRead(Buffer* buf, void* data, size_t dataSize, size_t position) {
    if (!buf || !data || dataSize == 0) return false;
    if (position + dataSize > buf->size) return false;
    
    memcpy(data, buf->buffer + position, dataSize);
    return true;
}

// Write data to the end of buffer (append)
bool bufferAppend(Buffer* buf, const void* data, size_t dataSize) {
    if (!buf || !data || dataSize == 0) return false;
    
    // Find the first available position (assuming buffer tracks used size)
    // For simplicity, we'll append at the end of current buffer size
    // In a real implementation, you might want to track the current write position
    return bufferWrite(buf, data, dataSize, 0); // This should be improved with position tracking
}

// Write a single byte to buffer at position
bool bufferWriteByte(Buffer* buf, int8_t byte, size_t position) {
    return bufferWrite(buf, &byte, sizeof(int8_t), position);
}

// Read a single byte from buffer at position
bool bufferReadByte(Buffer* buf, int8_t* byte, size_t position) {
    return bufferRead(buf, byte, sizeof(int8_t), position);
}

// Write an integer to buffer at position (little endian)
bool bufferWriteInt(Buffer* buf, int32_t value, size_t position) {
    return bufferWrite(buf, &value, sizeof(int32_t), position);
}

// Read an integer from buffer at position (little endian)
bool bufferReadInt(Buffer* buf, int32_t* value, size_t position) {
    return bufferRead(buf, value, sizeof(int32_t), position);
}

// Write a string to buffer at position
bool bufferWriteString(Buffer* buf, const char* str, size_t position) {
    if (!str) return false;
    size_t len = strlen(str);
    return bufferWrite(buf, str, len, position);
}

// Read a string from buffer at position (null-terminated)
bool bufferReadString(Buffer* buf, char* str, size_t maxLen, size_t position) {
    if (!str || maxLen == 0) return false;
    
    size_t i;
    for (i = 0; i < maxLen - 1 && position + i < buf->size; i++) {
        str[i] = buf->buffer[position + i];
        if (str[i] == '\0') break;
    }
    str[i] = '\0';
    return true;
}

// Get buffer size
size_t bufferGetSize(Buffer* buf) {
    return buf ? buf->size : 0;
}

// Check if buffer is valid
bool bufferIsValid(Buffer* buf) {
    return (buf != NULL && buf->buffer != NULL && buf->size > 0);
}

// Fill entire buffer with a specific value
bool bufferFill(Buffer* buf, int8_t value) {
    if (!buf || !buf->buffer) return false;
    
    memset(buf->buffer, value, buf->size);
    return true;
}

// Copy data from one buffer to another
bool bufferCopy(Buffer* dest, Buffer* src, size_t srcPos, size_t destPos, size_t length) {
    if (!dest || !src) return false;
    if (srcPos + length > src->size || destPos + length > dest->size) return false;
    
    memcpy(dest->buffer + destPos, src->buffer + srcPos, length);
    return true;
}

