CXX = gcc
CXXFLAGS = -g -Wall -Werror -Wshadow
OBJECTS = VMtoNucleo.o 
EXEC = VMtoNucleo

all: clean VMtoNucleo

VMtoNucleo: ${OBJECTS} 
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}

clean:
	rm -rf ${OBJECTS} ${EXEC}
