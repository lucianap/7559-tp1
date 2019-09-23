cmake_minimum_required(VERSION 3.10)
project(7559_tp1)

set(CMAKE_CXX_STANDARD 11)

include_directories(src)
add_subdirectory(src)


set(SOURCE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)


add_subdirectory("src/Cajon")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/Productor")
add_subdirectory("src/Ramo")
add_subdirectory("src/Utils")
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

target_link_libraries(7559_tp1 Ramo_lib Cajon_lib Utils_lib)


add_subdirectory("${PROJECT_SOURCE_DIR}/tests")
