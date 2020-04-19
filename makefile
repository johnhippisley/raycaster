CC=g++
CFLAGS=-g -lSDL2
FILES=*.cpp
EXEC=raycaster

raycaster: $(FILES)
	$(CC) $(FILES) $(CFLAGS) -o $(EXEC)
