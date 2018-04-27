CC=gcc
INCLUDE=
DFLAGS=
LIBS=-lstdc++ -lSDL2 -lGL -lGLEW
OFLAGS=-c
CFLAGS=-g3 -Wall -Wextra -std=c++11 -pedantic-errors $(INCLUDE) $(DFLAGS)

OBJECTS=main.o window.o game.o input_manager.o timer.o balls.o entity.o gl_program.o square.o
TARGET=evelight

all: $(OBJECTS) Makefile
	$(CC) $(CFLAGS) -o $(TARGET)  $(OBJECTS) $(LIBS)

%.o: %.cpp Makefile *.h
	$(CC) $(CFLAGS) $(OFLAGS) -o $@ $<

run: $(TARGET)
	./$(TARGET)

clean:
	rm $(OBJECTS) $(TARGET)
