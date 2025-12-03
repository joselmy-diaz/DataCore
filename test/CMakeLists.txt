function(add_linked_test name src)
    add_executable(${name} ${src})
    target_link_libraries(${name} PRIVATE DataCore)
    add_test(NAME ${name} COMMAND ${name})
endfunction()


if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-fsanitize=address -fno-omit-frame-pointer -g)
    add_link_options(-fsanitize=address)
endif()

add_linked_test(testDataCore      DataCoreT.c)
add_linked_test(Coroutine         coroutine.c)
add_linked_test(BufferTest        bufferTest.c)
add_linked_test(CircularBufferTest circularBufferTest.c)
