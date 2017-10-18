src = $(wildcard *.c)
obj = $(src:.c=.o)


LDFLAGS = -lm

all: main

main: $(obj)
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(obj) main
