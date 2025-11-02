#include "buffer.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

void test_buffer_basic_operations() {
    printf("=== Testing Basic Buffer Operations ===\n");
    
    // Create a normal buffer
    Obj* bufferObj = bufferCreate(false, 1024);
    assert(bufferObj != NULL);
    
    Buffer* buf = (Buffer*)bufferObj;
    
    // Test buffer validity
    assert(bufferIsValid(buf) == true);
    
    // Test buffer size
    assert(bufferGetSize(buf) == 1024);
    
    printf("✓ Buffer creation and basic checks passed\n");
}

void test_buffer_write_read() {
    printf("=== Testing Buffer Write/Read Operations ===\n");
    
    Obj* bufferObj = bufferCreate(false, 1024);
    Buffer* buf = (Buffer*)bufferObj;
    
    // Test byte operations
    bufferResetPosition(buf);  // Start at beginning
    int8_t testByte = 42;
    assert(bufferWriteByte(buf, testByte) == true);
    
    bufferResetPosition(buf);  // Reset to read from beginning
    int8_t readByte;
    assert(bufferReadByte(buf, &readByte) == true);
    assert(readByte == testByte);
    
    printf("✓ Byte write/read test passed\n");
    
    // Test integer operations
    bufferSetPosition(buf, 10);  // Set position to 10
    int32_t testInt = 0x12345678;
    assert(bufferWriteInt(buf, testInt) == true);
    
    bufferSetPosition(buf, 10);  // Reset position to read from same place
    int32_t readInt;
    assert(bufferReadInt(buf, &readInt) == true);
    assert(readInt == testInt);
    
    printf("✓ Integer write/read test passed\n");
    
    // Test string operations
    bufferSetPosition(buf, 20);  // Set position to 20
    const char* testString = "Hello Buffer!";
    assert(bufferWriteString(buf, testString) == true);
    
    bufferSetPosition(buf, 20);  // Reset position to read from same place
    char readString[50];
    assert(bufferReadString(buf, readString, sizeof(readString)) == true);
    assert(strcmp(readString, testString) == 0);
    
    printf("✓ String write/read test passed\n");
    
    // Test generic data write/read
    struct TestData {
        int a;
        float b;
        char c;
    } testData = {123, 45.67f, 'X'};
    
    bufferSetPosition(buf, 100);  // Set position to 100
    assert(bufferWrite(buf, &testData, sizeof(testData)) == true);
    
    struct TestData readData;
    memset(&readData, 0, sizeof(readData));
    
    // Read back the data
    bufferSetPosition(buf, 100);  // Reset position to read from same place
    assert(bufferRead(buf, &readData, sizeof(readData)) == true);
    assert(readData.a == testData.a);
    assert(readData.b == testData.b);
    assert(readData.c == testData.c);
}

void test_buffer_utility_functions() {
    printf("=== Testing Buffer Utility Functions ===\n");
    
    Obj* bufferObj = bufferCreate(false, 100);
    Buffer* buf = (Buffer*)bufferObj;
    
    // Test fill function
    assert(bufferFill(buf, 0xAA) == true);
    
    // Verify fill worked
    bufferResetPosition(buf);
    int8_t testByte;
    for (int i = 0; i < 10; i++) {
        assert(bufferReadByte(buf, &testByte) == true);
        assert(testByte == (int8_t)0xAA);
    }
    
    printf("✓ Buffer fill test passed\n");
    
    // Test buffer copy
    Obj* srcBufferObj = bufferCreate(false, 50);
    Buffer* srcBuf = (Buffer*)srcBufferObj;
    
    // Fill source with different pattern
    assert(bufferFill(srcBuf, 0x55) == true);
    
    // Copy data
    assert(bufferCopy(buf, srcBuf, 0, 50, 25) == true);
    
    // Verify copy
    bufferSetPosition(buf, 50);
    for (int i = 50; i < 75; i++) {
        assert(bufferReadByte(buf, &testByte) == true);
        assert(testByte == (int8_t)0x55);
    }
    
    printf("✓ Buffer copy test passed\n");
}

void test_buffer_edge_cases() {
    printf("=== Testing Buffer Edge Cases ===\n");
    
    Obj* bufferObj = bufferCreate(false, 10);
    Buffer* buf = (Buffer*)bufferObj;
    
    // Test boundary conditions
    int8_t testByte = 99;
    
    // Write at last valid position
    bufferSetPosition(buf, 9);
    assert(bufferWriteByte(buf, testByte) == true);
    
    // Try to write beyond buffer - should fail (position is now at 10)
    assert(bufferWriteByte(buf, testByte) == false);
    
    // Try to write with size overflow
    char largeData[20];
    bufferResetPosition(buf);
    assert(bufferWrite(buf, largeData, sizeof(largeData)) == false);
    
    printf("✓ Boundary condition tests passed\n");
    
    // Test null pointer handling
    assert(bufferWrite(NULL, &testByte, 1) == false);
    assert(bufferRead(buf, NULL, 1) == false);
    assert(bufferIsValid(NULL) == false);
    
    printf("✓ Null pointer tests passed\n");
}

void test_buffer_file_operations() {
    printf("=== Testing Buffer File Operations ===\n");
    
    Obj* bufferObj = bufferCreate(false, 1024);
    Buffer* buf = (Buffer*)bufferObj;
    
    // Fill buffer with test data
    const char* testData = "This is test data for file operations!";
    size_t dataLen = strlen(testData);
    bufferResetPosition(buf);
    assert(bufferWrite(buf, testData, dataLen) == true);
    
    // Write buffer to file
    const char* filename = "/tmp/buffer_test.dat";
    assert(writeFileBuffer(buf, filename) == true);
    
    printf("✓ File write test passed\n");
    
    // Create new buffer and read from file
    Obj* readBufferObj = bufferCreate(false, 1024);
    Buffer* readBuf = (Buffer*)readBufferObj;
    
    assert(readFileBuffer(readBuf, filename) == true);
    
    // Verify data
    char readData[100];
    bufferResetPosition(readBuf);
    assert(bufferRead(readBuf, readData, dataLen) == true);
    readData[dataLen] = '\0'; // Null terminate for comparison
    
    assert(strcmp(readData, testData) == 0);
    
    printf("✓ File read test passed\n");
    
    // Clean up
    remove(filename);
}

int main() {
    printf("Starting Buffer Tests...\n\n");
    
    test_buffer_basic_operations();
    printf("\n");
    
    test_buffer_write_read();
    printf("\n");
    
    test_buffer_utility_functions();
    printf("\n");
    
    test_buffer_edge_cases();
    printf("\n");
    
    test_buffer_file_operations();
    printf("\n");
    
    printf("🎉 All buffer tests passed successfully!\n");
    
    return 0;
}