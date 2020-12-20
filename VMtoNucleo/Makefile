CXX = gcc
CXXFLAGS = -g -Wall -Werror -Wshadow -lpthread -pthread
EXEC = Lab_Server

all: clean Lab_Server

Lab_Server:
	${CXX} ${CXXFLAGS} Lab_Server.c -o ${EXEC}

clean:
	rm -f ${OBJECTS} ${EXEC}