CC=g++
CFLAGS=-pedantic -Wall -Wextra -g
NAME=hinfosvc

all:
	$(CC) $(CFLAGS) *.cpp -o $(NAME)

clean:
	-rm -f *.o $(NAME)