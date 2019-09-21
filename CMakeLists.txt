cmake_minimum_required(VERSION 3.10)
project(7559_tp1)

set(CMAKE_CXX_STANDARD 11)

include_directories(src)


set(SOURCE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)


add_subdirectory("${PROJECT_SOURCE_DIR}/src/Cajon")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/Productor")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/Ramo")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/Utils")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/Lock")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/Logger")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/Distribuidor")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/ProcesoHijo")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/Signal")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/Menu")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/Pipes")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/Fifos")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/ProcesoInicial")

add_executable(7559_tp1 ${SOURCE} ${HEADERS})