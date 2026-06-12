CC ?= gcc
CFLAGS ?= -O2 -Wall -Wextra

append_hello_mod_test: append_hello_mod_test.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f append_hello_mod_test
