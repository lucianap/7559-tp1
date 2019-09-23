set(SOURCE_FILES
        Pedido.cpp
        )
set(HEADER_FILES
        Pedido.h
        )

add_library(Pedido_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})
