CC=g++
STDFLAGS=-std=c++20
WFLAGS=-Wextra -Wall -Werror

termchat-server: build/server.o build/client.o build/channel.o
	${CC} ${STDFLAGS} ${WFLAGS} build/server.o build/client.o build/channel.o -o build/termchat-server

build/server.o: src/server.cpp src/client.o
	${CC} ${STDFLAGS} ${WFLAGS} -c src/server.cpp -o build/server.o

build/client.o: src/client.cpp src/include/client.hpp
	${CC} ${STDFLAGS} ${WFLAGS} -c src/client.cpp -o build/client.o

build/channel.o: src/channel.cpp src/include/channel.hpp
	${CC} ${STDFLAGS} ${WFLAGS} -c src/channel.cpp -o build/channel.o

clean:
	rm -v build/*
