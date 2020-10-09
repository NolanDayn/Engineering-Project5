CXX = gcc
CXXFLAGS = -g -Wall -Werror -Wshadow
OBJECTS = VMtoNucleo.o SerialConfig.o 
EXEC = VMtoNucleo

all: clean VMtoNucleo

VMtoNucleo: ${OBJECTS} 
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}

SerialConfig.o:
	${CXX} -c ${CXXFLAGS} SerialConfig.c -o SerialConfig.o

clean:
	rm -rf ${OBJECTS} ${EXEC}
