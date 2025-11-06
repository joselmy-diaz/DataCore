#ifndef BUFFER_MANAGER_H
#define BUFFER_MANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "obj.h"


typedef struct {
    ObjR obj;
    int8_t *buffer;
    size_t size;
    size_t head;
    size_t tail;
} Buffer;

// Core buffer management functions
bool bufferClear(Obj* buffer);
Obj* bufferCreate(size_t size);

// File I/O functions
bool readFileBuffer(Buffer* buf, const char* path);
bool writeFileBuffer(const Buffer* buf, const char* path);

// Generic read/write functions for normal buffer
bool bufferWrite(Buffer* buf, const void* data, size_t dataSize);
bool bufferRead(Buffer* buf, void* data, size_t dataSize);


bool bufferFree(Obj* buffer);

// Utility functions
size_t bufferGetSize(Buffer* buf);
// bool bufferIsValid(Buffer* buf);
// bool bufferFill(Buffer* buf, int8_t value);
// bool bufferCopy(Buffer* dest, Buffer* src, size_t srcPos, size_t destPos, size_t length);
bool BufferResize(Buffer* buf, size_t newSize);

// Circular buffer functions
bool BufferCRead(Buffer* buf, void* data, size_t dataSize);
bool BufferCWrite (Buffer* cb, void* ptr, size_t count);
bool BufferCEmpty(Buffer *cb);
size_t BufferCSize(Buffer *cb);
bool BufferCAdd(Buffer *cb, void* item, size_t length, bool atStart);
bool BufferCPut(Buffer *cb, int8_t item, bool atStart);
bool BufferCPeek(Buffer *cb, void* ptr, size_t count);
size_t BufferCSpace(Buffer *cb);


#endif
