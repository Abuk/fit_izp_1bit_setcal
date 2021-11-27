CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -Werror

main:
	$(CC) $(CFLAGS) main.c -o setcal

clean:
	rm -f setcal
