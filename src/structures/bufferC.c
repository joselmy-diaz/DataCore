#include "buffer.h"

// Helper function to check if buffer is full
static bool isBufferFull(Buffer* cb) {
    return ((cb->head + 1) % cb->size) == cb->tail;
}

bool BufferCWrite (Buffer* cb, void* ptr, size_t count) {
    if (!cb || !ptr) return false;
    for (size_t i = 0; i < count; i++) {
        if (!BufferCPut(cb, ((int8_t*)ptr)[i], false)) return false;
    }
    return true;
}

bool BufferCPut (Buffer *cb, int8_t item, bool atStart) {
    if (!cb || isBufferFull(cb)) return false;  // Buffer full, can't add

    if (atStart) {
        // Add at the beginning (move tail backward)
        cb->tail = (cb->tail - 1 + cb->size) % cb->size;
        cb->buffer[cb->tail] = item;
    } else {
        // Add at the end (normal circular buffer operation)
        cb->buffer[cb->head] = item;
        cb->head = (cb->head + 1) % cb->size;
    }
    return true;
}

// Verifica si el búfer está vacío
bool BufferCEmpty(Buffer *cb) {
    if (!cb) return true;
    return (cb->head == cb->tail);
}

bool BufferCAdd (Buffer *cb, void* item, size_t length, bool atStart) {
    if (!cb || !item) return false;
    for (size_t i = 0; i < length; i++) {
        if (!BufferCPut(cb, ((int8_t*)item)[i], atStart)) return false;
    }
    return true;
}

// Returns the number of elements currently in the buffer
size_t BufferCSize(Buffer *cb) {
    if (!cb) return 0;
    return (cb->head - cb->tail + cb->size) % cb->size;
}

// Read a single byte from the buffer (removes it)
bool BufferCReadByte(Buffer* cb, int8_t* byte) {
    if (!cb || !byte || BufferCEmpty(cb)) return false;
    
    *byte = cb->buffer[cb->tail];
    cb->tail = (cb->tail + 1) % cb->size;
    return true;
}

bool BufferCRead(Buffer* cb, void* ptr, size_t count) {
    if (!cb || !ptr) return false;
    
    for (size_t i = 0; i < count; i++) {
        if (BufferCEmpty(cb)) return false;  // Buffer empty, can't read
        ((int8_t*)ptr)[i] = cb->buffer[cb->tail];
        cb->tail = (cb->tail + 1) % cb->size;
    }
    return true;
}

// Peek at data without removing it
bool BufferCPeek(Buffer* cb, void* ptr, size_t count) {
    if (!cb || !ptr || count == 0) return false;
    
    size_t tempTail = cb->tail;
    for (size_t i = 0; i < count; i++) {
        if (tempTail == cb->head) return false;  // Not enough data
        ((int8_t*)ptr)[i] = cb->buffer[tempTail];
        tempTail = (tempTail + 1) % cb->size;
    }
    return true;
}

// Get available space in buffer
size_t BufferCSpace(Buffer* cb) {
    if (!cb) return 0;
    return cb->size - BufferCSize(cb) - 1; // -1 to distinguish full from empty
}


