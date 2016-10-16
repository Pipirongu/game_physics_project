#--------------------------------------------------------------------------
# exts
#--------------------------------------------------------------------------
ADD_LIBRARY(extralibs INTERFACE)

ADD_SUBDIRECTORY(mypersonalmathlibrary)
SET_TARGET_PROPERTIES(mypersonalmathlibrary PROPERTIES FOLDER extralibs/mypersonalmathlibrary)
TARGET_INCLUDE_DIRECTORIES(extralibs INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/mypersonalmathlibrary/mypersonalmathlibrary)


ADD_SUBDIRECTORY(freetype2)
SET_TARGET_PROPERTIES(freetype PROPERTIES FOLDER extralibs/freetype2)
TARGET_INCLUDE_DIRECTORIES(extralibs INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/freetype2/include)