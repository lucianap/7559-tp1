cmake_minimum_required(VERSION 3.10)
project(7559_tp1)

set(CMAKE_CXX_STANDARD 11)

include_directories(src)


set(SOURCE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp src/Signal/SIGUSR1_Handler.h src/Remito/Remito.cpp src/Remito/Remito.h src/Guardador/Guardador.cpp src/Guardador/Guardador.h src/TipoProceso/TipoProceso.h src/TipoPedido/TipoPedido.h src/Restaurador/Restaurador.cpp src/Restaurador/Restaurador.h)


add_subdirectory("${PROJECT_SOURCE_DIR}/src/Cajon")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/Productor")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/Pedido")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/Ramo")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/Remito")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/RegistroVenta")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/Utils")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/Logger")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/Distribuidor")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/FileManager")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/PuntoVenta")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/ProcesoHijo")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/Signal")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/Menu")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/Pipes")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/ProcesoInicial")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/ProcesoClientes")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/Guardador")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/Status")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/Informe")

add_executable(7559_tp1 ${SOURCE} ${HEADERS})

target_link_libraries(7559_tp1 Ramo_lib Cajon_lib Utils_lib Pedido_lib Informe_lib RegistroVenta_lib)


add_subdirectory("${PROJECT_SOURCE_DIR}/tests")
