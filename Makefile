all: server

clean:
	rm server

server: server.c
	gcc -O3 -std=gnu17 server.c -o server -pthread

vi:
	vim \
		Makefile \
		server.c \
		server.h \
		settings-example.h \
		settings.h
