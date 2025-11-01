#include "buffer.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

void test_circular_buffer_creation() {
    printf("=== Testing Circular Buffer Creation ===\n");
    
    // Create a circular buffer
    Obj* bufferObj = bufferCreate(true, 10);
    assert(bufferObj != NULL);
    assert(bufferObj->type == TYPE_BUFFER_CIRCULAR);
    
    BufferCircular* cb = (BufferCircular*)bufferObj;
    assert(cb->size == 10);
    assert(cb->head == 0);
    assert(cb->tail == 0);
    assert(cb->buffer != NULL);
    
    // Test initial state
    assert(BufferCEmpty(cb) == true);
    assert(BufferCSize(cb) == 0);
    assert(BufferCSpace(cb) == 9); // size - 1 for full detection
    
    printf("✓ Circular buffer creation test passed\n");
}

void test_circular_buffer_basic_operations() {
    printf("=== Testing Circular Buffer Basic Operations ===\n");
    
    Obj* bufferObj = bufferCreate(true, 5);
    BufferCircular* cb = (BufferCircular*)bufferObj;
    
    // Test single byte put/read
    assert(BufferCPut(cb, 'A', false) == true);
    assert(BufferCEmpty(cb) == false);
    assert(BufferCSize(cb) == 1);

    int8_t readByte;
    assert(BufferCReadByte(cb, &readByte) == true);
    assert(readByte == 'A');
    assert(BufferCEmpty(cb) == true);
    assert(BufferCSize(cb) == 0);    printf("✓ Single byte operations test passed\n");
    
    // Test multiple bytes
    char testData[] = "Hell"; // Only 4 bytes to fit in buffer of size 5
    assert(BufferCAdd(cb, testData, 4, false) == true);
    assert(BufferCSize(cb) == 4);

    char readData[5] = {0};
    assert(BufferCRead(cb, readData, 4) == true);
    assert(strcmp(readData, testData) == 0);
    assert(BufferCEmpty(cb) == true);    printf("✓ Multiple byte operations test passed\n");
}

void test_circular_buffer_wrap_around() {
    printf("=== Testing Circular Buffer Wrap Around ===\n");
    
    Obj* bufferObj = bufferCreate(true, 5);
    BufferCircular* cb = (BufferCircular*)bufferObj;
    
    // Fill buffer partially
    char data1[] = "ABC";
    assert(BufferCAdd(cb, data1, 3, false) == true);

    // Read part of it
    char readData[4] = {0};
    assert(BufferCRead(cb, readData, 2) == true);
    assert(strncmp(readData, "AB", 2) == 0);

    // Add more data to test wrap around
    char data2[] = "XYZ";
    assert(BufferCAdd(cb, data2, 3, false) == true);

    // Now buffer should contain: C X Y Z (with wrap around)
    assert(BufferCSize(cb) == 4);

    // Read all remaining data
    char allData[5] = {0};
    assert(BufferCRead(cb, allData, 4) == true);
    assert(strcmp(allData, "CXYZ") == 0);    printf("✓ Wrap around test passed\n");
}

void test_circular_buffer_full_conditions() {
    printf("=== Testing Circular Buffer Full Conditions ===\n");
    
    Obj* bufferObj = bufferCreate(true, 4);
    BufferCircular* cb = (BufferCircular*)bufferObj;
    
    // Fill buffer to capacity (size - 1)
    char data[] = "ABC";
    assert(BufferCAdd(cb, data, 3, false) == true);
    assert(BufferCSpace(cb) == 0);

    // Try to add one more - should fail
    assert(BufferCPut(cb, 'X', false) == false);

    // Read one byte to make space
    int8_t readByte;
    assert(BufferCReadByte(cb, &readByte) == true);
    assert(readByte == 'A');

    // Now we should be able to add one
    assert(BufferCPut(cb, 'X', false) == true);    printf("✓ Full buffer conditions test passed\n");
}

void test_circular_buffer_peek() {
    printf("=== Testing Circular Buffer Peek Operations ===\n");
    
    Obj* bufferObj = bufferCreate(true, 8);
    BufferCircular* cb = (BufferCircular*)bufferObj;
    
    // Add some data
    char testData[] = "PEEK";
    assert(BufferCAdd(cb, testData, 4, false) == true);

    // Peek at the data (should not modify buffer)
    char peekData[5] = {0};
    assert(BufferCPeek(cb, peekData, 4) == true);
    assert(strcmp(peekData, testData) == 0);

    // Buffer should still contain the data
    assert(BufferCSize(cb) == 4);

    // Now actually read it
    char readData[5] = {0};
    assert(BufferCRead(cb, readData, 4) == true);
    assert(strcmp(readData, testData) == 0);
    assert(BufferCEmpty(cb) == true);    printf("✓ Peek operations test passed\n");
}

void test_circular_buffer_at_start() {
    printf("=== Testing Circular Buffer Add At Start ===\n");
    
    Obj* bufferObj = bufferCreate(true, 6);
    BufferCircular* cb = (BufferCircular*)bufferObj;
    
    // Add data normally
    assert(BufferCPut(cb, 'B', false) == true);
    assert(BufferCPut(cb, 'C', false) == true);

    // Add at start
    assert(BufferCPut(cb, 'A', true) == true);

    // Buffer should now contain: A B C
    char readData[4] = {0};
    assert(BufferCRead(cb, readData, 3) == true);
    assert(strcmp(readData, "ABC") == 0);    printf("✓ Add at start test passed\n");
}

void test_circular_buffer_edge_cases() {
    printf("=== Testing Circular Buffer Edge Cases ===\n");
    
    Obj* bufferObj = bufferCreate(true, 3);
    BufferCircular* cb = (BufferCircular*)bufferObj;
    
    // Test null pointer handling
    assert(BufferCPut(NULL, 'A', false) == false);
    assert(BufferCRead(NULL, NULL, 1) == false);
    assert(BufferCEmpty(NULL) == true);
    assert(BufferCSize(NULL) == 0);

    // Test read from empty buffer
    int8_t byte;
    assert(BufferCReadByte(cb, &byte) == false);

    char data[2];
    assert(BufferCRead(cb, data, 1) == false);
    assert(BufferCPeek(cb, data, 1) == false);

    // Test peek beyond available data
    assert(BufferCPut(cb, 'A', false) == true);
    assert(BufferCPeek(cb, data, 2) == false); // Only 1 byte available    printf("✓ Edge cases test passed\n");
}

void test_circular_buffer_write_read_generic() {
    printf("=== Testing Circular Buffer Generic Write/Read ===\n");
    
    Obj* bufferObj = bufferCreate(true, 20);
    BufferCircular* cb = (BufferCircular*)bufferObj;
    
    // Test BufferCWrite function
    char testString[] = "Hello World!";
    size_t len = strlen(testString);

    assert(BufferCWrite(cb, testString, len) == true);
    assert(BufferCSize(cb) == len);

    // Read it back
    char readString[20] = {0};
    assert(BufferCRead(cb, readString, len) == true);
    assert(strcmp(readString, testString) == 0);
    assert(BufferCEmpty(cb) == true);    printf("✓ Generic write/read test passed\n");
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
    
    test_circular_buffer_write_read_generic();
    printf("\n");
    
    printf("🎉 All circular buffer tests passed successfully!\n");
    
    return 0;
}