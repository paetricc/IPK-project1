CC=gcc
CFLAGS=-pedantic -Wall -Wextra -g
NAME=hinfosvc

all:
	$(CC) $(CFLAGS) hinfosvc.c -o $(NAME)

clean:
	-rm -f *.o $(NAME)