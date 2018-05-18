CC=gcc
INCLUDE=
DFLAGS=
LIBS=-lstdc++ -lSDL2 -lGL -lGLEW -lm
OFLAGS=-c
CFLAGS=-g3 -Wall -Wextra -std=c++11 -pedantic-errors $(INCLUDE) $(DFLAGS)

OBJECTS=main.o balls.o window.o game.o input_manager.o timer.o camera.o gl_program.o \
		rectangle.o filled_rectangle.o \
		lodepng/lodepng.o texture.o texture_manager.o \
		aabb.o sprite.o multi_sprite.o renderer.o texter.o \
		entity.o ball.o player.o

TARGET=evelight

all: $(OBJECTS) Makefile
	$(CC) $(CFLAGS) -o $(TARGET)  $(OBJECTS) $(LIBS)

%.o: %.cpp Makefile *.h
	$(CC) $(CFLAGS) $(OFLAGS) -o $@ $<

run: $(TARGET)
	./$(TARGET)

clean:
	rm $(OBJECTS) $(TARGET)
