CC = gcc
CFLAGS = -Wall -Wextra -g
SRC_DIR = src

SRCS = $(SRC_DIR)/main.c \
       $(SRC_DIR)/commander.c \
       $(SRC_DIR)/simulado.c \
       $(SRC_DIR)/manager.c \
       $(SRC_DIR)/reporter.c

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
