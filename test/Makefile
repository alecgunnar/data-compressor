.PHONY: clean

all: objects shift_window_test prefix_offset_test longest_prefix_test booyer_moore_test

objects:
	cp ../$(SOURCE_DIR)/*.o .

shift_window_test:
	$(CC) $(CFLAGS) $(LDFLAGS) boyer_moore.o lzss.o shift_window_test.c -o shift_window.test
	./shift_window.test

prefix_offset_test:
	$(CC) $(CFLAGS) $(LDFLAGS) boyer_moore.o lzss.o prefix_offset_test.c -o prefix_offset.test
	./prefix_offset.test

longest_prefix_test:
	$(CC) $(CFLAGS) $(LDFLAGS) boyer_moore.o lzss.o longest_prefix_test.c -o longest_prefix.test
	./longest_prefix.test

booyer_moore_test:
	$(CC) $(CFLAGS) $(LDFLAGS) boyer_moore.o booyer_moore_test.c -o booyer_moore.test
	./booyer_moore.test

clean:
	rm -f *.test
	rm -f *.o