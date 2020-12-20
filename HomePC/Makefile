CXX = gcc
CXXFLAGS = -g -Wall -Werror -Wshadow -lpthread -pthread
OBJECTS = GameController.o Game_Controller_Interface.o
EXEC = GC_Interface

all: clean GC_Interface

GC_Interface: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}

HexDump.o:
	${CXX} -c ${CXXFLAGS} Game_Controller_Interface.c -o Game_Controller_Interface.o

GameController.o:
	${CXX} -c ${CXXFLAGS} GameController.c -o GameController.o

clean:
	rm -rf ${OBJECTS} ${EXEC}