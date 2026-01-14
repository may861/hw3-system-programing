CC = gcc
CFLAGS = -Wall -Wextra -std=c11

TARGET = bash_mini
SRC = mini_bash.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)
