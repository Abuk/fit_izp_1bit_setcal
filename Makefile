CC=gcc
CFLAGS=-lstdc++ -std=c99 -Wall -Wextra -Werror
default_target=all
LIBS= utils/loader.c utils/parser.c utils/types.c

main:
	$(CC) $(CFLAGS) setcal.c $(LIBS) -o setcal

clean:
	rm -f setcal
