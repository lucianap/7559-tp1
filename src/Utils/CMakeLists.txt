set(SOURCE_FILES
        Utils.cpp
        )
set(HEADER_FILES
        Utils.h
        )

add_library(Utils_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})
