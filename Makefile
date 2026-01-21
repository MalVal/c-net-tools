CC=gcc
CFLAGS=-Wall -Wextra -Werror -Iinclude

SRC=src/main.c tools/ping.c
OBJ=$(SRC:.c=.o)

TARGET=cnet

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

clean:
	rm -f $(OBJ) $(TARGET)
