ADD_SUBDIRECTORY( include ) 

INCLUDE_DIRECTORIES(include)
ADD_DEFINITIONS(-D_REENTRANT)
ADD_DEFINITIONS(-D FANN_NO_DLL)
ADD_DEFINITIONS(-D _CRT_SECURE_NO_WARNINGS)

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

link_directories(
    ${CMAKE_BINARY_DIR}/lib
)
########### next target ###############

SET(floatfann_LIB_SRCS
floatfann.c
)

ADD_LIBRARY(floatfann STATIC ${floatfann_LIB_SRCS})

SET_TARGET_PROPERTIES(floatfann PROPERTIES VERSION ${VERSION} SOVERSION 2 )
INSTALL(TARGETS floatfann DESTINATION ${LIB_INSTALL_DIR} )


########### next target ###############

SET(doublefann_LIB_SRCS
doublefann.c
)

ADD_LIBRARY(doublefann STATIC ${doublefann_LIB_SRCS})

SET_TARGET_PROPERTIES(doublefann PROPERTIES VERSION ${VERSION} SOVERSION 2 )
INSTALL(TARGETS doublefann DESTINATION ${LIB_INSTALL_DIR} )


########### next target ###############

SET(fixedfann_LIB_SRCS
fixedfann.c
)

ADD_LIBRARY(fixedfann STATIC ${fixedfann_LIB_SRCS})

SET_TARGET_PROPERTIES(fixedfann PROPERTIES VERSION ${VERSION} SOVERSION 2 )
INSTALL(TARGETS fixedfann DESTINATION ${LIB_INSTALL_DIR} )


########### next target ###############

SET(fann_LIB_SRCS
floatfann.c
)

ADD_LIBRARY(fann STATIC ${fann_LIB_SRCS})

SET_TARGET_PROPERTIES(fann PROPERTIES VERSION ${VERSION} SOVERSION 2 )
INSTALL(TARGETS fann DESTINATION ${LIB_INSTALL_DIR} )

