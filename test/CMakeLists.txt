# Archivos de prueba
add_executable(testDataCore DataCoreT.c)

add_executable(Coroutine coroutine.c)

# Vincular la librería al ejecutable de prueba
target_link_libraries(testDataCore PRIVATE DataCore)
target_link_libraries(Coroutine PRIVATE DataCore)

# Agregar una prueba al sistema de pruebas de CTest
add_test(NAME DataCore COMMAND testDataCore)
add_test(NAME Coroutine COMMAND Coroutine)
