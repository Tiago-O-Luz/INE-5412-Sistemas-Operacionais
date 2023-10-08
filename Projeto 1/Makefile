# BIN = ./bin
HEADERS = ./headers
COMPONENTS = ./components
SOURCES = $(wildcard $(COMPONENTS)/*.cc)

all:
	g++ main.cpp $(SOURCES) -I $(HEADERS) -o app
run:
	./app
clean:
	rm ./app