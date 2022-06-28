cmake_minimum_required(VERSION 3.24)

#project name
PROJECT("client" C)

#head file path
INCLUDE_DIRECTORIES(../common/include)

#link_directories(../common/Debug)

AUX_SOURCE_DIRECTORY(. DIR_SRCS)

ADD_EXECUTABLE(client ${DIR_SRCS})

#set(LIBS common)

find_library(LIBS common ../common/Debug)
if (${LIBS} STREQUAL "LIBS-NOTFOUND")
    message (FATAL_ERROR "required ../common/Debug library but not found!")
else()
    message (STATUS "common library found in ${LIBS}")
endif()


message(${LIBS})

target_link_libraries(client ${LIBS})


#方式一
#SET(SRCS src)
#add_library(common ${SRCS}/file_parse_h264.c ${SRCS}/share_memory.c)
#ADD_EXECUTABLE(common ${SRCS}/file_parse_h264.c ${SRCS}/share_memory.c)

#方式二
#AUX_SOURCE_DIRECTORY(. DIR_SRCS)

#方式三
#file(GLOB DIR_SRCS "*.c" "src/*.c")

#方式四
#file(GLOB_RECURSE DIR_SRCS "src/*.c")

#方式五
#message(${DIR_SRCS})
#message("build with debug mode")
#message(WARNING "this is warnning message：" ${DIR_SRCS})

#add_library(common ${DIR_SRCS} ${DIR_SRCS1})

#set environment variable
#SET(SRCS ${DIR_SRCS})

#link_directories(lib)

#set extern libraries
#SET(LIBS mbedcrypto)

#add executable file
#ADD_EXECUTABLE(mbedtls ${SRCS})

#add link library
#TARGET_LINK_LIBRARIES(mbedtls ${LIBS})