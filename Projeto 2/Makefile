# BIN = ./bin
HEADERS = ./headers
COMPONENTS = ./components
SOURCES = $(wildcard $(COMPONENTS)/*.cc)

all:
	g++ main.cpp -o app
run:
	./app $(ARGS)
clean:
	rm ./app