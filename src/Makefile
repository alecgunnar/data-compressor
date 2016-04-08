OBJECTS = boyer_moore.o lzss.o error.o

.PHONY: objects clean

all: objects
	$(CC) $(CFLAGS) $(LDFLAGS) main.c $(OBJECTS) -o $(EXEC)

objects: $(OBJECTS)

clean:
	rm -f *.o
