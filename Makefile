CC = gcc
CFLAGS = -Wall -Wextra -Werror -Iinclude

SRC = src/main.c tools/ping.c src/icmp.c
OBJ = $(SRC:.c=.o)

TARGET = cnet

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
