#MathLibrary HEADER AND SOURCE
FILE(GLOB HEAD ${CMAKE_CURRENT_SOURCE_DIR}/mypersonalmathlibrary/*.h)
FILE(GLOB SRC ${CMAKE_CURRENT_SOURCE_DIR}/mypersonalmathlibrary/*.cc)
LIST(APPEND SRCSBASE ${HEAD} ${SRC})
SOURCE_GROUP(mypersonalmathlibrary FILES ${SRCSBASE})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/mypersonalmathlibrary)

ADD_LIBRARY(mypersonalmathlibrary STATIC ${SRCSBASE})