CC = gcc
CFLAGS = -Wall -Wextra -g

SRCS = main.c commander.c simulado.c manager.c reporter.c

OBJS = $(SRCS:.c=.o)

EXEC = tp

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(EXEC) $(OBJS)

.PHONY: all clean
