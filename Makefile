all: libserver.a reserver

libserver.a: main.go
	go build -buildmode=c-archive -o libserver.a main.go

reserver: libserver.a inotify.c
	gcc -o reserver inotify.c -L. -lserver -lpthread

clean:
	@rm -f reserver libserver.a libserver.h
