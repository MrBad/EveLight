CC=gcc
AR=gcc-ar
INCLUDE=
DFLAGS=
LIBS=-lstdc++ -lSDL2 -lGL -lGLEW -lm
OFLAGS=-c
CFLAGS=-O3 -Wall -Wextra -std=c++11 -pedantic-errors $(INCLUDE) $(DFLAGS)
EVELIB=evelib.a
EVELIB_OBJS=window.o game.o input_manager.o timer.o camera.o gl_program.o \
		texture.o texture_manager.o \
		renderer.o texter.o \
		aabb.o quad_tree.o \
		rectangle.o filled_rectangle.o sprite.o multi_sprite.o \
		entity.o \
		lodepng/lodepng.o

#TARGETS=eve_push quad_test quad_query
TARGETS=q1 quad_query quad_test eve_push

all: $(TARGETS)

q1: $(EVELIB) q1.o
	$(CC) $(CFLAGS) -o q1 q1.o $(EVELIB) $(LIBS)

# quad_test exe - a QuadTree test program
QUAD_TEST_OBJS=quad_test.o
quad_test: $(QUAD_TEST_OBJS) $(EVELIB)
	$(CC) $(CFLAGS) -o quad_test $(QUAD_TEST_OBJS) $(EVELIB) $(LIBS)

# quad_test exe - a QuadTree test program
QUAD_QUERY_OBJS=quad_query.o
quad_query: $(QUAD_QUERY_OBJS) $(EVELIB)
	$(CC) $(CFLAGS) -o quad_query $(QUAD_QUERY_OBJS) $(EVELIB) $(LIBS)

# eve_push exe - a test program
EVE_PUSH_OBJS=eve_push.o balls.o player.o ball.o
eve_push: $(EVE_PUSH_OBJS) $(EVELIB)
	$(CC) $(CFLAGS) -o eve_push $(EVE_PUSH_OBJS) $(EVELIB) $(LIBS)

%.o: %.cpp Makefile *.h
	$(CC) $(CFLAGS) $(OFLAGS) -o $@ $<

evelib.a: $(EVELIB_OBJS)
	$(AR) $(ARFLAGS) evelib.a $(EVELIB_OBJS)

OBJECTS=$(QUAD_TEST_OBJS) $(EVE_PUSH_OBJS) 
clean:
	rm $(OBJECTS) $(TARGETS)

distclean:
	rm $(EVELIB_OBJS) $(EVELIB) $(OBJECTS) $(TARGETS)

