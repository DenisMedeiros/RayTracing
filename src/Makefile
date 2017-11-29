src = $(wildcard *.c)
obj = $(src:.c=.o)

CCFLAGS = -Wall -O2 -g -fopenmp
LDFLAGS = -lm -lGL -lGLU -lglut 

CC = gcc $(CCFLAGS)

all: main

main: $(obj)
	$(CC) $(CCFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(obj) main
