
LIB_TARGET = libcommon.a

CFLAGS	+= -Wall
CFLAGS  += $(INC_PATH)

SOURCES_C = $(wildcard ./src/*.c)
SOURCES_CPP = $(wildcard ./src/*.cpp)
OBJS = $(patsubst %.c,%.o, $(SOURCES_C)) $(patsubst %.cpp,%.o, $(SOURCES_CPP))
 
CC	= gcc
CPP	= g++
AR	= ar

RM	= @echo " RM	$@"; rm -f

INC_PATH = -I.
INC_PATH += -I./include

AFLAGS	+= -r

TARGET	= $(LIB_TARGET) $(EXEC_TARGET)

all: $(TARGET)

$(LIB_TARGET): $(OBJS)
	$(RM) $@;
	$(AR) $(AFLAGS) $@ $^

$(EXEC_TARGET): $(EXEC_OBJS) 
	$(CPP) -o $@ $^ $(LIBS) $(LDFLAGS)
.c.o:
	$(CC)  $(CFLAGS) -c -o $@ $<

.cpp.o:
	$(CPP) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf ./*.o ./*.a ./src/*.o ./src/*.a

