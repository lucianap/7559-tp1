set(SOURCE_FILES
        RegistroVenta.cpp
        )
set(HEADER_FILES
        RegistroVenta.h
        )

add_library(RegistroVenta_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})
