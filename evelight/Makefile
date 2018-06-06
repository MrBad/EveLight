CC=gcc
AR=gcc-ar
INCLUDE=
DFLAGS=
LIBS=-lstdc++ -lSDL2 -lGL -lGLEW -lm
OFLAGS=-c
CFLAGS=-O3 -Wall -Wextra -std=c++11 -pedantic-errors $(INCLUDE) $(DFLAGS)
EVELIB=evelib.a
OBJS=window.o game.o input_manager.o timer.o camera.o gl_program.o \
		texture.o texture_manager.o \
		renderer.o texter.o \
		aabb.o quad_tree.o \
		rectangle.o filled_rectangle.o sprite.o multi_sprite.o grid.o \
		entity.o \
		lodepng/lodepng.o

TARGET=evelib.a

all: $(TARGET)

%.o: %.cpp Makefile *.h
	$(CC) $(CFLAGS) $(OFLAGS) -o $@ $<

evelib.a: $(OBJS)
	$(AR) $(ARFLAGS) $(TARGET) $(OBJS)

clean:
	rm $(OBJS) $(EVELIB)
