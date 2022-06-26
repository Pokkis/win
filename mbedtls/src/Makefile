CC = gcc
CPP = g++
AR = ar
TARGET = crypt

SOURCES_C = $(wildcard *.c)
SOURCES_CPP = $(wildcard *.cpp)
OBJS = $(patsubst %.c,%.o, $(SOURCES_C)) $(patsubst %.cpp,%.o, $(SOURCES_CPP))

LD_FLAGS 	= -lmbedcrypto
LIB_PATH = -L. -L../lib
INCLUDE = 	-I./ -I../include -I../include/mbedtls

#CFLAGS = -g
CFLAGS = -Wall
CFLAGS += $(INCLUDE) $(LIB_PATH)

all : $(TARGET)

$(TARGET) : $(SOURCES_C) $(SOURCES_CPP)
	$(CC)  $(CFLAGS) $^ -o $(TARGET) $(LD_FLAGS) 
	
.PHONY = clean
	
clean : 
	rm -rf *.o $(TARGET)
