CC = gcc
CPP = g++
AR = ar
TARGET = client

SOURCES_C = $(wildcard *.c)
SOURCES_CPP = $(wildcard *.cpp)
OBJS = $(patsubst %.c,%.o, $(SOURCES_C)) $(patsubst %.cpp,%.o, $(SOURCES_CPP))

LD_FLAGS 	= -lpthread -lcommon
LIB_PATH = -L. -L../common
INCLUDE = 	-I./ -I../common/include

#CFLAGS = -g
CFLAGS = -Wall
CFLAGS += $(INCLUDE) $(LIB_PATH)

all : $(TARGET)

$(TARGET) : $(SOURCES_C) $(SOURCES_CPP)
	$(CC)  $(CFLAGS) $^ -o $(TARGET) $(LD_FLAGS) 
	
.PHONY = clean
	
clean : 
	rm -rf *.o $(TARGET)
