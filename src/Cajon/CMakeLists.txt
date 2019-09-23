set(SOURCE_FILES
        Cajon.cpp
        )
set(HEADER_FILES
        Cajon.h
        )

add_library(Cajon_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})
