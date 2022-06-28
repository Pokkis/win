cmake_minimum_required(VERSION 3.24)

#project name
PROJECT(common)

#head file path
INCLUDE_DIRECTORIES(
	include
)
#source directory
AUX_SOURCE_DIRECTORY(src DIR_SRCS)

#set environment variable
SET(SRCS
	${DIR_SRCS}
)

link_directories(
    lib
)

#set extern libraries
SET(LIBS
	mbedcrypto
)

#add executable file
ADD_EXECUTABLE(mbedtls ${SRCS})

#add link library
TARGET_LINK_LIBRARIES(mbedtls ${LIBS})