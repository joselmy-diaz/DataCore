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
    size_t position;
} Buffer;

typedef struct {
    ObjR obj;
    int8_t *buffer;
    size_t size;
    size_t head;
    size_t tail;
} BufferCircular;

// Core buffer management functions
bool bufferClear(Obj* buffer);
Obj* bufferCreate(bool circular, int size);

// File I/O functions
bool readFileBuffer(Buffer* buf, const char* path);
bool writeFileBuffer(const Buffer* buf, const char* path);

// Generic read/write functions for normal buffer
bool bufferWrite(Buffer* buf, const void* data, size_t dataSize);
bool bufferRead(Buffer* buf, void* data, size_t dataSize);
bool bufferAppend(Buffer* buf, const void* data, size_t dataSize);

// Specific data type functions
bool bufferWriteByte(Buffer* buf, int8_t byte);
bool bufferReadByte(Buffer* buf, int8_t* byte);
bool bufferWriteInt(Buffer* buf, int32_t value);
bool bufferReadInt(Buffer* buf, int32_t* value);
bool bufferWriteString(Buffer* buf, const char* str);
bool bufferReadString(Buffer* buf, char* str, size_t maxLen);

bool bufferFree(Obj* buffer);

// Utility functions
size_t bufferGetSize(Buffer* buf);
size_t bufferGetPosition(Buffer* buf);
bool bufferSetPosition(Buffer* buf, size_t position);
bool bufferResetPosition(Buffer* buf);
bool bufferIsValid(Buffer* buf);
bool bufferFill(Buffer* buf, int8_t value);
bool bufferCopy(Buffer* dest, Buffer* src, size_t srcPos, size_t destPos, size_t length);
bool BufferResize(Buffer* buf, size_t newSize);

// Circular buffer functions
bool BufferCEmpty(BufferCircular *cb);
int16_t BufferCSize(BufferCircular *cb);
bool BufferCAdd(BufferCircular *cb, void* item, size_t length, bool atStart);
bool BufferCPut(BufferCircular *cb, int8_t item, bool atStart);
bool BufferCWrite(BufferCircular* cb, void* ptr, size_t count);
bool BufferCRead(BufferCircular* cb, void* ptr, size_t count);
bool BufferCReadByte(BufferCircular* cb, int8_t* byte);
bool BufferCPeek(BufferCircular* cb, void* ptr, size_t count);
size_t BufferCSpace(BufferCircular* cb);


#endif
