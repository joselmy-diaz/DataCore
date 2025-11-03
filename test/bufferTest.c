#include "buffer.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

void test_buffer_creation() {
    printf("=== Testing Buffer Creation ===\n");
    
    // Test creación de buffer con tamaño válido
    Obj* bufferObj = bufferCreate(100);
    assert(bufferObj != NULL);
    assert(bufferObj->type == TYPE_BUFFER);
    
    Buffer* buf = (Buffer*)bufferObj;
    assert(buf->size == 100);
    assert(buf->head == 0);
    assert(buf->tail == 0);
    assert(buf->buffer != NULL);
    
    // Test buffer vacío
    assert(buf->head == buf->tail);
    
    bufferFree(bufferObj);
    
    // Test creación con tamaño 0
    assert(bufferCreate(0) == NULL);
    
    printf("✓ Buffer creation tests passed\n");
}

void test_buffer_write_read() {
    printf("=== Testing Buffer Write/Read Operations ===\n");
    
    Obj* bufferObj = bufferCreate(100);
    Buffer* buf = (Buffer*)bufferObj;
    
    // Test escritura básica
    const char* testData = "Hello, World!";
    size_t dataLen = strlen(testData) + 1;
    
    bool writeResult = bufferWrite(buf, testData, dataLen);
    assert(writeResult == true);
    assert(buf->head == dataLen);
    
    // Test lectura básica
    char readBuffer[20] = {0};
    bool readResult = bufferRead(buf, readBuffer, dataLen);
    assert(readResult == true);
    assert(strcmp(readBuffer, testData) == 0);
    
    bufferFree(bufferObj);
    printf("✓ Buffer write/read tests passed\n");
}

void test_buffer_clear() {
    printf("=== Testing Buffer Clear Operation ===\n");
    
    Obj* bufferObj = bufferCreate(100);
    Buffer* buf = (Buffer*)bufferObj;
    
    // Escribir algunos datos
    const char* testData = "Test Data";
    bufferWrite(buf, testData, strlen(testData) + 1);
    
    // Limpiar buffer
    bool clearResult = bufferClear(bufferObj);
    assert(clearResult == true);
    assert(buf->head == 0);
    assert(buf->tail == 0);
    
    // Verificar que los datos se limpiaron
    char firstByte = buf->buffer[0];
    assert(firstByte == 0);
    
    bufferFree(bufferObj);
    printf("✓ Buffer clear tests passed\n");
}

void test_buffer_resize() {
    printf("=== Testing Buffer Resize Operation ===\n");
    
    Obj* bufferObj = bufferCreate(50);
    Buffer* buf = (Buffer*)bufferObj;
    
    // Escribir datos
    const char* testData = "Initial Data";
    size_t dataLen = strlen(testData) + 1;
    bufferWrite(buf, testData, dataLen);
    
    // Aumentar tamaño
    bool resizeResult = BufferResize(buf, 100);
    assert(resizeResult == true);
    assert(buf->size == 100);
    assert(buf->head == dataLen);
    
    // Verificar que los datos se mantienen
    char readBuffer[20] = {0};
    buf->tail = 0;
    bufferRead(buf, readBuffer, dataLen);
    assert(strcmp(readBuffer, testData) == 0);
    
    // Reducir tamaño
    resizeResult = BufferResize(buf, 25);
    assert(resizeResult == true);
    assert(buf->size == 25);
    
    bufferFree(bufferObj);
    printf("✓ Buffer resize tests passed\n");
}

void test_buffer_file_operations() {
    printf("=== Testing Buffer File Operations ===\n");
    
    Obj* bufferObj = bufferCreate(100);
    Buffer* buf = (Buffer*)bufferObj;
    
    // Escribir datos en el buffer
    const char* testData = "Test file data";
    bufferWrite(buf, testData, strlen(testData) + 1);
    
    // Escribir a archivo
    bool writeResult = writeFileBuffer(buf, "test.tmp");
    assert(writeResult == true);
    
    // Limpiar buffer
    bufferClear(bufferObj);
    
    // Leer de archivo
    bool readResult = readFileBuffer(buf, "test.tmp");
    assert(readResult == true);
    
    // Verificar datos
    char readBuffer[20] = {0};
    bufferRead(buf, readBuffer, strlen(testData) + 1);
    assert(strcmp(readBuffer, testData) == 0);
    
    // Limpiar
    remove("test.tmp");
    bufferFree(bufferObj);
    
    printf("✓ Buffer file operation tests passed\n");
}

int main() {
    printf("Starting Buffer Tests...\n\n");
    
    test_buffer_creation();
    printf("\n");
    
    test_buffer_write_read();
    printf("\n");
    
    test_buffer_clear();
    printf("\n");
    
    test_buffer_resize();
    printf("\n");
    
    test_buffer_file_operations();
    printf("\n");
    
    printf("🎉 All buffer tests passed successfully!\n");
    return 0;
}