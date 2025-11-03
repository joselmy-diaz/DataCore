#include "buffer.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

/* Forward declarations for the C-style circular buffer API used by these tests.
   The real implementations live in the library; the header present in the
   project does not expose the BufferC* symbols, so declare them here to avoid
   implicit-function-declaration errors during test compilation. */


void test_circular_buffer_creation() {
    printf("=== Testing Circular Buffer Creation ===\n");
    
    // Create a circular buffer
    Obj* bufferObj = bufferCreate(10);
    assert(bufferObj != NULL);
    assert(bufferObj->type == TYPE_BUFFER);
    
    Buffer* cb = (Buffer*)bufferObj;
    assert(cb->size == 10);
    assert(cb->head == 0);
    assert(cb->tail == 0);
    assert(cb->buffer != NULL);
    
    // Test initial state
    assert(BufferCEmpty(cb) == true);
    assert(BufferCSize(cb) == 0);
    assert(BufferCSpace(cb) == 9); // size - 1 for full detection
    
    bufferFree(bufferObj);
    printf("✓ Circular buffer creation test passed\n");
}

void test_circular_buffer_basic_operations() {
    printf("=== Testing Circular Buffer Basic Operations ===\n");
    
    Obj* bufferObj = bufferCreate(5);
    Buffer* cb = (Buffer*)bufferObj;
    
    // Test single byte write/read
    int8_t testByte = 42;
    assert(BufferCPut(cb, testByte, false) == true);
    assert(BufferCSize(cb) == 1);
    assert(BufferCEmpty(cb) == false);
    
    int8_t readByte;
    assert(BufferCRead(cb, &readByte, 1) == true);
    assert(readByte == testByte);
    assert(BufferCEmpty(cb) == true);
    
    bufferFree(bufferObj);
    printf("✓ Circular buffer basic operations test passed\n");
}

void test_circular_buffer_wrap_around() {
    printf("=== Testing Circular Buffer Wrap Around ===\n");
    
    Obj* bufferObj = bufferCreate(4);
    Buffer* cb = (Buffer*)bufferObj;
    
    // Fill buffer
    for(int i = 0; i < 3; i++) {
        assert(BufferCPut(cb, i, false) == true);
    }
    
    // Read some data
    int8_t readByte;
    assert(BufferCRead(cb, &readByte, 1) == true);
    assert(readByte == 0);
    
    // Write more data to trigger wrap around
    assert(BufferCPut(cb, 42, false) == true);
    assert(BufferCSize(cb) == 3);
    
    // Read all data
    int8_t data[3];
    assert(BufferCRead(cb, data, 3) == true);
    assert(data[0] == 1);
    assert(data[1] == 2);
    assert(data[2] == 42);
    
    bufferFree(bufferObj);
    printf("✓ Circular buffer wrap around test passed\n");
}

void test_circular_buffer_full_conditions() {
    printf("=== Testing Circular Buffer Full Conditions ===\n");
    
    Obj* bufferObj = bufferCreate(4);
    Buffer* cb = (Buffer*)bufferObj;
    
    // Fill buffer to capacity (size-1)
    for(int i = 0; i < 3; i++) {
        assert(BufferCPut(cb, i, false) == true);
    }
    
    // Try to write when full
    assert(BufferCPut(cb, 42, false) == false);
    assert(BufferCSize(cb) == 3);
    
    // Read one byte
    int8_t readByte;
    assert(BufferCRead(cb, &readByte, 1) == true);
    
    // Now we should be able to write again
    assert(BufferCPut(cb, 42, false) == true);
    
    bufferFree(bufferObj);
    printf("✓ Circular buffer full conditions test passed\n");
}

void test_circular_buffer_peek() {
    printf("=== Testing Circular Buffer Peek ===\n");
    
    Obj* bufferObj = bufferCreate(5);
    Buffer* cb = (Buffer*)bufferObj;
    
    // Write some data
    int8_t testData[] = {1, 2, 3};
    assert(BufferCWrite(cb, testData, 3) == true);
    
    // Peek data
    int8_t peekData[3];
    assert(BufferCPeek(cb, peekData, 3) == true);
    for(int i = 0; i < 3; i++) {
        assert(peekData[i] == testData[i]);
    }
    
    // Verify data is still in buffer
    assert(BufferCSize(cb) == 3);
    
    bufferFree(bufferObj);
    printf("✓ Circular buffer peek test passed\n");
}

void test_circular_buffer_at_start() {
    printf("=== Testing Circular Buffer Add At Start ===\n");
    
    Obj* bufferObj = bufferCreate(5);
    Buffer* cb = (Buffer*)bufferObj;
    
    // Add at end first
    assert(BufferCPut(cb, 1, false) == true);
    assert(BufferCPut(cb, 2, false) == true);
    
    // Add at start
    assert(BufferCPut(cb, 0, true) == true);
    
    // Read all data
    int8_t data[3];
    assert(BufferCRead(cb, data, 3) == true);
    assert(data[0] == 0);
    assert(data[1] == 1);
    assert(data[2] == 2);
    
    bufferFree(bufferObj);
    printf("✓ Circular buffer add at start test passed\n");
}

void test_circular_buffer_edge_cases() {
    printf("=== Testing Circular Buffer Edge Cases ===\n");
    
    Obj* bufferObj = bufferCreate(3);
    Buffer* cb = (Buffer*)bufferObj;
    
    // Test reading from empty buffer
    int8_t data;
    assert(BufferCRead(cb, &data, 1) == false);
    
    // Test writing to NULL buffer
    assert(BufferCPut(NULL, 1, false) == false);
    
    // Test reading with NULL pointer
    assert(BufferCRead(cb, NULL, 1) == false);
    
    // Test peeking beyond available data
    assert(BufferCPeek(cb, &data, 1) == false);
    
    bufferFree(bufferObj);
    printf("✓ Circular buffer edge cases test passed\n");
}

int main() {
    printf("Starting Circular Buffer Tests...\n\n");
    
    test_circular_buffer_creation();
    printf("\n");
    
    test_circular_buffer_basic_operations();
    printf("\n");
    
    test_circular_buffer_wrap_around();
    printf("\n");
    
    test_circular_buffer_full_conditions();
    printf("\n");
    
    test_circular_buffer_peek();
    printf("\n");
    
    test_circular_buffer_at_start();
    printf("\n");
    
    test_circular_buffer_edge_cases();
    printf("\n");
    
    printf("🎉 All circular buffer tests passed successfully!\n");
    
    return 0;
}