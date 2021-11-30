CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -Werror
default_target=all

main:
	$(CC) $(CFLAGS) setcal.c $(LIBS) -o setcal

clean:
	rm -f setcal
