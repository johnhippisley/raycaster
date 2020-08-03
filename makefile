CC=g++
CFLAGS=-ggdb -lSDL2 -Wall
FILES=$(shell find src -name "*.cpp")
EXEC=raycaster

bleak: $(FILES)
	$(CC) $(FILES) $(CFLAGS) -o $(EXEC)

run: $(EXEC)
	./$(EXEC)
