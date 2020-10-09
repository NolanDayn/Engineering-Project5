# makefile template found at
# http://nuclear.mutantstargoat.com/articles/make/

src = $(wildcard *.c)
obj = $(src:.c=.o)

LDFLAGS = -lGL -lpng -lz -lm
all: client server

client: client.c
	$(CC) -o $@ $^ $(LDFLAGS)

server: server.c
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(obj) client server
