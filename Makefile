OBJS = mysh.o execute.o history.o wildcard.o alias.o parse.o
OUT = mysh
FLAGS = -g -c
CC = gcc

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT)

mysh.o: mysh.c
	$(CC) $(FLAGS) mysh.c

execute.o: execute.c
	$(CC) $(FLAGS) execute.c

history.o: history.c
	$(CC) $(FLAGS) history.c

wildcard.o: wildcard.c
	$(CC) $(FLAGS) wildcard.c

alias.o: alias.c
	$(CC) $(FLAGS) alias.c

parse.o: parse.c
	$(CC) $(FLAGS) parse.c

clean:
	rm -f $(OBJS) $(OUT)