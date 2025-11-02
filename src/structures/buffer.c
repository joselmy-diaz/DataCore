#include "./buffer.h"

#define MAX_ZISE 123

Obj* bufferCreate(bool circular, int size) {
    if (circular) {
        BufferCircular* cb = (BufferCircular*)malloc(sizeof(BufferCircular));
        if (!cb) {
            fprintf(stderr, "Memory allocation for BufferCircular failed\n");
            return NULL;
        }
        cb->obj.type = TYPE_BUFFER_CIRCULAR;
        cb->buffer = (int8_t*)malloc(sizeof(int8_t) * size);
        cb->size = size;
        if (!cb->buffer) {
            free(cb);
            fprintf(stderr, "Memory allocation for buffer data failed\n");
            return NULL;
        }
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
        cb->size = size;
        cb->position = 0;  // Initialize position
        if (!cb->buffer) {
            free(cb);
            fprintf(stderr, "Memory allocation for buffer data failed\n");
            return NULL;
        }
        bufferClear((Obj*)cb);
        return (Obj*)cb;
    }
}

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
        bd->position = 0;  // Reset position when clearing
        for (size_t i = 0; i < bd->size; i++) {
            bd->buffer[i] = 0;
        }
    } else {
        return false;
    }
    return true;
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

// Write data to buffer at current position
bool bufferWrite(Buffer* buf, const void* data, size_t dataSize) {
    if (!buf || !data || dataSize == 0) return false;
    if (buf->position + dataSize > buf->size) return false;
    
    memcpy(buf->buffer + buf->position, data, dataSize);
    buf->position += dataSize;  // Advance position after writing
    return true;
}

// Read data from buffer at current position
bool bufferRead(Buffer* buf, void* data, size_t dataSize) {
    if (!buf || !data || dataSize == 0) return false;
    if (buf->position + dataSize > buf->size) return false;
    
    memcpy(data, buf->buffer + buf->position, dataSize);
    buf->position += dataSize;  // Advance position after reading
    return true;
}

// Write data to the end of buffer (append)
bool bufferAppend(Buffer* buf, const void* data, size_t dataSize) {
    if (!buf || !data || dataSize == 0) return false;
    
    // Write at current position (which tracks the end of written data)
    return bufferWrite(buf, data, dataSize);
}

// Write a single byte to buffer at current position
bool bufferWriteByte(Buffer* buf, int8_t byte) {
    return bufferWrite(buf, &byte, sizeof(int8_t));
}

// Read a single byte from buffer at current position
bool bufferReadByte(Buffer* buf, int8_t* byte) {
    return bufferRead(buf, byte, sizeof(int8_t));
}

// Write an integer to buffer at current position (little endian)
bool bufferWriteInt(Buffer* buf, int32_t value) {
    return bufferWrite(buf, &value, sizeof(int32_t));
}

// Read an integer from buffer at current position (little endian)
bool bufferReadInt(Buffer* buf, int32_t* value) {
    return bufferRead(buf, value, sizeof(int32_t));
}

// Write a string to buffer at current position
bool bufferWriteString(Buffer* buf, const char* str) {
    if (!str) return false;
    size_t len = strlen(str);
    return bufferWrite(buf, str, len);
}

// Read a string from buffer at current position (null-terminated)
bool bufferReadString(Buffer* buf, char* str, size_t maxLen) {
    if (!str || maxLen == 0) return false;
    
    size_t i;
    for (i = 0; i < maxLen - 1 && buf->position + i < buf->size; i++) {
        str[i] = buf->buffer[buf->position + i];
        if (str[i] == '\0') {
            buf->position += i + 1;  // Include null terminator in position advance
            break;
        }
    }
    if (i == maxLen - 1) {
        str[i] = '\0';
        buf->position += i;
    }
    return true;
}

// Get buffer size
size_t bufferGetSize(Buffer* buf) {
    return buf ? buf->size : 0;
}

// Get current position in buffer
size_t bufferGetPosition(Buffer* buf) {
    return buf ? buf->position : 0;
}

// Set position in buffer
bool bufferSetPosition(Buffer* buf, size_t position) {
    if (!buf || position > buf->size) return false;
    buf->position = position;
    return true;
}

// Reset position to beginning of buffer
bool bufferResetPosition(Buffer* buf) {
    if (!buf) return false;
    buf->position = 0;
    return true;
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


bool bufferFree(Obj* buffer) {
    if (!buffer) return false;
    if (buffer->type == TYPE_BUFFER_CIRCULAR) {
        BufferCircular* cb = (BufferCircular*)buffer;
        if (cb->buffer) free(cb->buffer);
        free(cb);
    } else if (buffer->type == TYPE_BUFFER) {
        Buffer* bd = (Buffer*)buffer;
        if (bd->buffer) free(bd->buffer);
        free(bd);
    } else {
        return false;
    }
    return true;
}