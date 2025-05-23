# Makefile for BaseBuddy

CC = gcc
CFLAGS = -Wall -O2
TARGET = basebuddy
SRC = basebuddy.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)
